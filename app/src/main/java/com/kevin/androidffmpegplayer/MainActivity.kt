package com.kevin.androidffmpegplayer

import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.Settings
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import java.io.File


class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val file = File("/mnt/sdcard/test.264")
        if (file.exists()) {
            val h264Player = H264Player()
            h264Player.setSource(file.absolutePath)
        }
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.R || Environment.isExternalStorageManager()) {
            Toast.makeText(this, "已获得访问所有文件的权限", Toast.LENGTH_SHORT).show()
        } else {
            val intent = Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION)
            startActivity(intent)
        }

    }
}