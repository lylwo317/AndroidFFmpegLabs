#ifndef H264_PLAYER
#define H264_PLAYER

#include <android/native_window.h>
#include <vector>
#include <queue>
#include <deque>
#include <thread>
#include "stream_data.h"
#include "buffer_info.h"
#include "block_queue.h"


extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
}

/**
 * 第一阶段：先实现解码渲染
 * 第二阶段：实现解码后存储到待渲染队列。由外部控制渲染时机
 */
class FFmpegDecoder {
private:
    std::mutex _sync;
    bool _isStart = false;
    bool _getfirsti = false;
    bool _firstNeedI = true;

    std::thread _decodeThread;

   volatile bool _stop_decode = false;

    ANativeWindow* _window = nullptr;
    // 解码器
    AVCodec *_codec = nullptr;
    // 上下文
    AVCodecContext *_ctx = nullptr;
    // 解析器上下文
    AVCodecParserContext *_parserCtx = nullptr;
    // 存放解码前的数据(h264)
    AVPacket *_pkt = nullptr;
    // 存放解码后的数据(yuv)
    AVFrame *_frame = nullptr;

    AVCodecID _avCodecID = AV_CODEC_ID_NONE;

    //等待dequeue，然后写入数据
    std::vector<std::shared_ptr<InputBufferData>> _availableInputBufferQueue;
    //等待解码器解码
    BlockingQueue<std::shared_ptr<InputBufferData>> _queueInputBufferQueue;

    //等待解码器解码填充已经解码好的数据
    BlockingQueue<std::shared_ptr<OutputBufferData>> _availableOutputBufferQueue;
    //等待渲染
    std::vector<std::shared_ptr<OutputBufferData>> _waitForRenderOutputBufferQueue;

    const char * get_h264_nalu(const char* data, size_t len, unsigned char *sps, unsigned int *p_sps_len, unsigned char *pps,unsigned int *p_pps_len, bool *sync);
    void parseAndDecode();
    void putToAvailableInputBufferQueue(const std::shared_ptr<InputBufferData>& buffer);
    int decode(AVCodecContext *ctx, AVPacket *pkt, AVFrame *frame);
    bool initAvCodec();
    void destroyAvCodec();
public:
    ~FFmpegDecoder();
    //Surface, FORMAT
    void configure(ANativeWindow *ptr, AVCodecID avCodecId);

    /**
     * 开启解码器
     */
    void start();

    /**
     * 检测解码器状态
     * @return
     */
    bool isStart();

    /**
     * 停止解码器
     */
    void stop();

    /**
     * 需要重新配置
     */
    void reset();

    int dequeueInputBuffer();

    void queueInputBuffer(int index, char* data_from, int data_len, int64_t pts);

    int dequeueOutputBuffer(BufferInfo* bufferInfo);

    void releaseOutputBuffer(int index);

};

#endif
