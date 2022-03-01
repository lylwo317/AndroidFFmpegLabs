#ifndef H264_PLAYER
#define H264_PLAYER

#include <android/native_window.h>


extern "C"{
#include <libavcodec/avcodec.h>
#include <libavcodec/packet.h>
#include <libavutil/frame.h>
}

/**
 * 第一阶段：先实现解码渲染
 * 第二阶段：实现解码后存储到待渲染队列。由外部控制渲染时机
 */
class FFmpegDecoder {
private:
    bool _isStart = false;
    bool _getfirsti = false;
    bool _firstNeedI = true;
    ANativeWindow* _window = nullptr;
    // 解码器
    AVCodec *codec = nullptr;
    // 上下文
    AVCodecContext *ctx = nullptr;
    // 解析器上下文
    AVCodecParserContext *parserCtx = nullptr;
    // 存放解码前的数据(h264)
    AVPacket *pkt = nullptr;
    // 存放解码后的数据(yuv)
    AVFrame *frame = nullptr;
    const char * get_h264_nalu(const char* data, int len, unsigned char *sps, unsigned int *p_sps_len, unsigned char *pps,unsigned int *p_pps_len, bool *sync);
    int decode(AVCodecContext *ctx, AVPacket *pkt, AVFrame *frame);
public:
    //设置需要渲染到的窗口
    /**
     * 开启解码器
     */
    void startDecoder();

    /**
     * 检测解码器状态
     * @return
     */
    bool isStart();

    /**
     * 写入数据进行解码
     * @param data
     * @param data_len
     * @return
     */
    bool writePacket(char* data, int data_len);

    /**
     * 读取解码后的帧
     */
    void readFrame();

    /**
     * 停止解码器
     */
    void stopDecoder();

    void setANativeWindow(ANativeWindow *ptr);
};

#endif
