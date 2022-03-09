package com.kevin.androidffmpegplayer

import android.Manifest
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import android.view.SurfaceView
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.kevin.androidffmpegplayer.frame.FrameEntity
import com.kevin.androidffmpegplayer.frame.FrameHeader
import com.kevin.ffmpeg.jni.AVCodecID
import com.kevin.ffmpeg.jni.BufferInfo
import com.kevin.ffmpeg.jni.FFmpegDecoder
import java.io.File
import java.nio.ByteBuffer


class MainActivity : AppCompatActivity() {
    private var surface: Surface? = null
    private var thread: Thread? = null
    val decoder = FFmpegDecoder()
    companion object{
        val TAG:String = MainActivity.javaClass.simpleName
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val starBtn = findViewById<Button>(R.id.btn_start)
        val surfaceView = findViewById<SurfaceView>(R.id.surface_view)
        surfaceView.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceCreated(p0: SurfaceHolder) {
                surface = p0.surface
                decoder.configureFromJava(surface, AVCodecID.AV_CODEC_ID_H264)
            }

            override fun surfaceChanged(p0: SurfaceHolder, p1: Int, p2: Int, p3: Int) {
                surface = p0.surface
            }

            override fun surfaceDestroyed(p0: SurfaceHolder) {
            }
        })
        starBtn.setOnClickListener {
            startPlay()
        }
        val stopBtn = findViewById<Button>(R.id.btn_stop)
        stopBtn.setOnClickListener {
            stopPlay()
        }
        //1.先请求判断是否具有对应权限
        if (ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
            != PackageManager.PERMISSION_GRANTED
        ) //根据返回的结果，判断对应的权限是否有。
        {
            ActivityCompat.requestPermissions(
                this, arrayOf(Manifest.permission.WRITE_EXTERNAL_STORAGE),
                0
            )
        }
    }

    private fun startPlay(){
        val file = File("/sdcard/h264_aac.raw")

        if (file.exists() && thread?.isAlive != true) {
            val readBytes = file.readBytes()
            val buffer = ByteBuffer.wrap(readBytes)


            decoder.configureFromJava(surface, AVCodecID.AV_CODEC_ID_H264)
            decoder.start()

            Log.d(TAG, "start play")
            thread = object : Thread() {
                override fun run() {
                    startDecode(this, buffer, decoder)
                }
            }

            thread?.start()
        }
    }

    private fun stopPlay() {
        thread?.interrupt()
    }

    fun startDecode(thread: Thread, buffer: ByteBuffer, decoder: FFmpegDecoder){
        try {
            while (buffer.remaining() > 0 && !thread.isInterrupted) {
                val frameEntity = FrameEntity()
                val headerBytes = ByteArray(FrameHeader.LENGTH)
                buffer.get(headerBytes)
                val respondHeader: FrameHeader = FrameHeader.parseToP2pFrameHeader(headerBytes)
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
                            val wrap = ByteBuffer.allocateDirect(frameEntity.frame.size)
                            wrap.put(frameEntity.frame)
                            decoder.queueInputBuffer(index, wrap, wrap.limit(), frameEntity.frameHeader.presentationTimeMs)
                            writeInput = true
                        }

                        val bufferInfo = BufferInfo()
                        index = decoder.dequeueOutputBuffer(bufferInfo)
                        if (index >= 0) {
                            Log.d("Read Packet", "buffer info: pts = " + bufferInfo.presentationTimeUs)
                            decoder.releaseOutputBuffer(index)
                            Thread.sleep(40)
                        }
                    }
                }
                Log.d("Read Packet", "frame header " + frameEntity.frameHeader)
            }
        } catch (e: InterruptedException) {
            Log.d("TAG", "hello")
        }
        decoder.stop()
    }
}