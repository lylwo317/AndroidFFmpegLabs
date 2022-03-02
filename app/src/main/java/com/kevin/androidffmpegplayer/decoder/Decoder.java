package com.kevin.androidffmpegplayer.decoder;

/**
 * 解码器
 * Created by XieJiaHua on 19-12-18
 */
public interface Decoder {
    int INDEX_NONE = -1;
    long PTS_NONE = Long.MIN_VALUE;
    long PTS_UNKNOWN = Long.MAX_VALUE;

    /**
     * 启动解码
     */
    void start();


    boolean isStart();

    /**
     * 解码器是否需要重建了。重建了，就丢弃当前已经获取到的解码buffer index
     * @return
     */
    boolean reInitCodec();

    /**
     * 进行解码
     * @return
     */
    boolean feedInputBuffer();

    /**
     * 获取解码后的数据
     * @return
     */
    FrameInfo drainOutputBuffer();

    /**
     * 渲染帧
     * @param frameInfo
     * @param offsetUs
     */
    void renderFrame(FrameInfo frameInfo, long offsetUs);

    /**
     * 停止解码
     */
    void stop();

    /**
     * 清空解码器缓冲区
     */
    void flush();

    /**
     * 释放解码器资源
     */
    void release();
}
