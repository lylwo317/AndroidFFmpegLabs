package com.kevin.androidffmpegplayer

import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.Settings
import android.util.Log
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import com.kevin.androidffmpegplayer.frame.FrameEntity
import com.kevin.androidffmpegplayer.frame.P2pFrameHeader
import com.kevin.androidffmpegplayer.jni.AVCodecID
import com.kevin.androidffmpegplayer.jni.BufferInfo
import com.kevin.androidffmpegplayer.jni.FFmpegDecoder
import java.io.File
import java.nio.ByteBuffer


class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R || Environment.isExternalStorageManager()) {
            Toast.makeText(this, "已获得访问所有文件的权限", Toast.LENGTH_SHORT).show()
            val file = File("/sdcard/h264_aac.raw")

            if (file.exists()) {
                val readBytes = file.readBytes()
                val buffer = ByteBuffer.wrap(readBytes)

                val decoder = FFmpegDecoder()
                val surfaceView = findViewById<SurfaceView>(R.id.surface_view)
                surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
                    override fun surfaceCreated(p0: SurfaceHolder) {
                        decoder.configureFromJava(p0.surface, AVCodecID.AV_CODEC_ID_H264)
                        decoder.start()

                        val thread = object : Thread() {
                            override fun run() {
                                startDecode(buffer, decoder)
                            }
                        }

                        thread.start()
                    }

                    override fun surfaceChanged(p0: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
                    }

                    override fun surfaceDestroyed(p0: SurfaceHolder) {
                    }
                })

//            startDecode(buffer, h264Player)
//            h264Player.setANativeWindow(10L);
            }
        } else {
            val intent = Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION)
            startActivity(intent)
        }
    }

    fun startDecode(buffer: ByteBuffer, decoder: FFmpegDecoder){
        while (buffer.remaining() > 0) {
            val frameEntity = FrameEntity()
            val headerBytes = ByteArray(P2pFrameHeader.LENGTH)
            buffer.get(headerBytes)
            val respondHeader: P2pFrameHeader = P2pFrameHeader.parseToP2pFrameHeader(headerBytes)
            frameEntity.frameHeader = respondHeader
            if (respondHeader.len > 0) {//parse body
                val bodyBytes = ByteArray(respondHeader.len)
                buffer.get(bodyBytes)
                frameEntity.frame = bodyBytes
            }
            if (frameEntity.isVideoFrame) {
//                decoder.writePacket(frameEntity.frame)
                var writeInput = false

                while (!writeInput) {
                    var index = -1
                    index = decoder.dequeueInputBuffer()
                    if (index >= 0) {
                        decoder.queueInputBuffer(index, frameEntity.frame)
                        writeInput = true
                    }

                    val bufferInfo = BufferInfo()
                    index = decoder.dequeueOutputBuffer(bufferInfo)
                    if (index >= 0) {
                        decoder.releaseOutputBuffer(index)
                        Thread.sleep(40)
                    }
                }
            }
            Log.d("Read Packet", "frame header " + frameEntity.frameHeader)
        }
        decoder.stop()
    }
}