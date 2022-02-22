package com.kevin.androidffmpegplayer;

import android.util.Log;

/**
 * 1. 读取test.264文件，解析每一帧
 * 2. 送入解码器
 * 3. 将解码后的frame（YUV）转成 RGB
 * 4. 拷贝RGB到ANativeWindow（显示）
 *
 * Created by XieJiaHua on 2022/2/17.
 */
class H264Player {
    static final String TAG = H264Player.class.getSimpleName();

    private long mNativeObject;

    static {
        try {
            System.loadLibrary("kxffmpeg");
        } catch (UnsatisfiedLinkError error) {
            Log.e(TAG, "Load kxffmpeg libray failed", error);
        }
    }

    public native void nativeInit();

    /**
     * 读取文件，如何将文件按照原定计划。
     *
     * @param filePath
     */
    public void setSource(String filePath){

    }
}
