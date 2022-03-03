#include <ffmpeg_decoder.h>
#include <jni.h>

#include "Log.h"
#include <unistd.h>
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/error.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}
#include <cstdint>
#include <thread>

#define ERROR_BUF(ret) \
    char errbuf[1024]; \
    av_strerror(ret, errbuf, sizeof (errbuf));

const char * FFmpegDecoder::get_h264_nalu(const char* data, size_t len, unsigned char *sps, unsigned int *p_sps_len, unsigned char *pps,unsigned int *p_pps_len, bool *sync)
{
    bool issync = false;
    int data_len_src = len;
    const char *data_buff = const_cast<char *>(data);
    const char *data_buff_src = data;
    unsigned int sps_len = 0, pps_len = 0;

    while(data_len_src >= 0)
    {
        /* SPS */
        if (*(unsigned char *)(data_buff + 0) == 0x00 &&
            *(unsigned char *)(data_buff + 1) == 0x00 &&
            *(unsigned char *)(data_buff + 2) == 0x00 &&
            *(unsigned char *)(data_buff + 3) == 0x01)
        {
            unsigned char type = (((*(unsigned char *)(data_buff + 4)) & 0x1F));
            if (type != 0x7 && type != 0x8) {
                goto endTo;
            }
            data_buff_src = data_buff;
            while (true)
            {
                /* PPS *///到下一个0001
                if (*(unsigned char *)(data_buff + sps_len + 0) == 0x00 &&
                    *(unsigned char *)(data_buff + sps_len + 1) == 0x00 &&
                    *(unsigned char *)(data_buff + sps_len + 2) == 0x00 &&
                    *(unsigned char *)(data_buff + sps_len + 3) == 0x01)
                {//计算出sps的长度，这里已经到pps了
                    unsigned char type2 = (((*(unsigned char *)(data_buff + sps_len + 4)) & 0x1F));
                    if (type2 == type) {
                        goto spsEnd;
                    }

                    if (type == 0x7) {//0001 -- 0001
                        memcpy(sps, data_buff, sps_len);
                        *p_sps_len = sps_len;
                    }else{
                        memcpy(pps, data_buff, sps_len);
                        *p_pps_len = sps_len;
                    }

                    data_buff = data_buff + sps_len;
                    while (true)
                    {
                        /*IDR*/
                        if (*(unsigned char *)(data_buff + pps_len + 0) == 0x00 &&
                            *(unsigned char *)(data_buff + pps_len + 1) == 0x00 &&
                            *(unsigned char *)(data_buff + pps_len + 2) == 0x00 &&
                            *(unsigned char *)(data_buff + pps_len + 3) == 0x01 &&
                            (((*(unsigned char *)(data_buff + pps_len + 4)) & 0x1F) == 0x05))

                        {
                            if (type2 == 0x7) {
                                memcpy(sps, data_buff, pps_len);
                                *p_sps_len = pps_len;
                            }else{
                                memcpy(pps, data_buff, pps_len);
                                *p_pps_len = pps_len;
                            }
                            data_buff = data_buff + pps_len;
//                            *(int *)(data_buff) = htonl(len - ((int)(data_buff - (unsigned char*)data_buff_src) + 4));
                            _getfirsti = true;
                            issync = true;
                            break;
                        }
                        pps_len++;
                    }
                    break;
                }
                spsEnd:
                sps_len++;
            }
            break;
        }
        endTo:
        /* SLICE */
        if (*(unsigned char *)(data_buff + 0) == 0x00 &&
            *(unsigned char *)(data_buff + 1) == 0x00 &&
            *(unsigned char *)(data_buff + 2) == 0x00 &&
            *(unsigned char *)(data_buff + 3) == 0x01 &&
            (((*(unsigned char *)(data_buff + 4)) & 0x1F) == 0x01))
        {
            if (!_getfirsti)
            {
                return nullptr;
            }
            issync = false;
//            data_buff = data_buff +
//            *(int *)(data_buff) = htonl(len - ((int)(data_buff - (unsigned char*)data_buff_src) + 4));
            break;
        }
        data_buff++;
        data_len_src--;
    }

//            *p_sps_len = sps_len;
//            *p_pps_len = pps_len;
    *sync = issync;

    return data_buff;
}

/**
* @param render 渲染器
* @param data   解码和转换处理后的图像数据
* @param len    data 的长度
* @param width  视频的宽
* @param height 视频的高
*/
void render_rend(AVFrame* dst_frame, int width, int height, ANativeWindow *window) {
    if (window == NULL) {
        LOGE("window == null");
        return;
    }
    //获取 ANativeWindow_Buffer , 如果获取失败 , 直接释放相关资源退出
    ANativeWindow_Buffer aNativeWindow_Buffer;
    //设置缓冲区(宽，高，像素格式)
    ANativeWindow_setBuffersGeometry(window, width, height, WINDOW_FORMAT_RGBA_8888);
    //绘制
    int32_t locked = ANativeWindow_lock(window, &aNativeWindow_Buffer, NULL);

    if (locked == 0) {
        //由于window的 stride 和 frame 的 stride 不同，需要逐行拷贝
        uint8_t *dst = (uint8_t *) aNativeWindow_Buffer.bits;
        uint8_t *src = dst_frame->data[0];
        int dest_stride = aNativeWindow_Buffer.stride * 4;
        int src_stride = dst_frame->linesize[0];
        int height = dst_frame->height, h;
        for (h = 0; h < height; h++) {
            memcpy(dst + h * dest_stride,
                   src + h * src_stride,
                   (size_t) src_stride);
        }

/*
        //向 ANativeWindow_Buffer 填充 RGBA 像素格式的图像数据
        uint8_t *dst_data = static_cast<uint8_t *>(aNativeWindow_Buffer.bits);

        //参数中的 uint8_t *data 数据中 , 每一行有 linesize 个 , 拷贝的目标也要逐行拷贝
        //  aNativeWindow_Buffer.stride 是每行的数据个数 , 每个数据都包含一套 RGBA 像素数据 ,
        //      RGBA 数据每个占1字节 , 一个 RGBA 占 4 字节
        //  每行的数据个数 * 4 代表 RGBA 数据个数
        int dst_linesize = aNativeWindow_Buffer.stride * 4;

        //获取 ANativeWindow_Buffer 中数据的地址
        //      一次拷贝一行 , 有 像素高度 行数
        for(int i = 0; i < aNativeWindow_Buffer.height; i++){
            //计算拷贝的指针地址
            //  每次拷贝的目的地址 : dst_data + ( i * dst_linesize )
            //  每次拷贝的源地址 : data + ( i * linesize )
            memcpy(dst_data + ( i * dst_linesize ), data + ( i * linesize ), dst_linesize);
        }
*/

        ANativeWindow_unlockAndPost(window);
    } else {
        LOGD("failed to lock window");
    }

}

int FFmpegDecoder::decode(AVCodecContext *ctx,
                  AVPacket *pkt,
                  AVFrame *src_frame) {
    // 发送压缩数据到解码器
    int ret = avcodec_send_packet(ctx, pkt);
    if (ret < 0) {
        ERROR_BUF(ret);
        LOGE("avcodec_send_packet error %s", errbuf);
        return ret;
    }

    while (true) {
        // 获取解码后的数据
        ret = avcodec_receive_frame(ctx, src_frame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return 0;
        } else if (ret < 0) {
            ERROR_BUF(ret);
            LOGE("avcodec_receive_frame error. %s", errbuf);
            return ret;
        }

        LOGD("解码成功");
        LOGD("frame->linesize[0] %d", src_frame->linesize[0]);

        std::shared_ptr<BufferData<AVFrame>> availableFrame;
        ret = _availableOutputBufferQueue.pop(availableFrame);
        if (!ret) {
            return -1;
        }
        //1. 准备一个容器来装转码后的数据
//        AVFrame* src_frame = av_frame_alloc();
        AVFrame* dst_frame = availableFrame->ptr.get();
        //在解码上下文使用extradata解析出第一帧图像后，ctx的width和height,pix_format 写入了实际的视频宽高，像素格式
        dst_frame->width = ctx->width;
        dst_frame->height = ctx->height;
        //2. 转码为ARGB，来给NativeWindow显示
        dst_frame->format = AV_PIX_FMT_RGBA;
        //3. 根据输入图像和输出图像的信息（宽、高、像素），初始化格式转换上下文
        //应该重复使用该上下文，不要每一帧都初始化一次
        struct SwsContext * swsCtx = sws_getContext(
                src_frame->width, src_frame->height,(enum AVPixelFormat) src_frame->format,
                src_frame->width, src_frame->height,(enum AVPixelFormat) dst_frame->format,
                SWS_FAST_BILINEAR, NULL, NULL, NULL);
        //4. 初始化之前准备的dst_frame的buffer
        int buffer_size = av_image_get_buffer_size(
                (enum AVPixelFormat) dst_frame->format,
                src_frame->width,
                src_frame->height, 1);
        uint8_t * buffer = (uint8_t *) av_malloc(sizeof(uint8_t) * buffer_size );
        //5. 绑定dst_frame和新申请的buffer
        av_image_fill_arrays(dst_frame->data, dst_frame->linesize, buffer,
                             (enum AVPixelFormat) dst_frame->format,
                             dst_frame->width, dst_frame->height, 1);
        //6. 转码
        sws_scale(swsCtx , (const uint8_t *const *) src_frame->data,
                  src_frame->linesize, 0, src_frame->height,
                  dst_frame->data, dst_frame->linesize);

        _waitForRenderOutputBufferQueue.push_back(availableFrame);

/*
        AVFrame* src_frame = frame;
        //1. 准备一个容器来装转码后的数据
        AVFrame* dst_frame = av_frame_alloc();
        //在解码上下文使用extradata解析出第一帧图像后，ctx的width和height,pix_format 写入了实际的视频宽高，像素格式
        dst_frame->width = ctx->width;
        dst_frame->height = ctx->height;
        //2. 转码为ARGB，来给NativeWindow显示
        dst_frame->format = AV_PIX_FMT_RGBA;
        //3. 根据输入图像和输出图像的信息（宽、高、像素），初始化格式转换上下文
        //应该重复使用该上下文，不要每一帧都初始化一次
        struct SwsContext * swsCtx = sws_getContext(
                src_frame->width, src_frame->height,(enum AVPixelFormat) src_frame->format,
                src_frame->width, src_frame->height,(enum AVPixelFormat) dst_frame->format,
                SWS_FAST_BILINEAR, NULL, NULL, NULL);
        //4. 初始化之前准备的dst_frame的buffer
        int buffer_size = av_image_get_buffer_size(
                (enum AVPixelFormat) dst_frame->format,
                src_frame->width,
                src_frame->height, 1);
        uint8_t * buffer = (uint8_t *) av_malloc(sizeof(uint8_t) * buffer_size );
        //5. 绑定dst_frame和新申请的buffer
        av_image_fill_arrays(dst_frame->data, dst_frame->linesize, buffer,
                             (enum AVPixelFormat) dst_frame->format,
                             dst_frame->width, dst_frame->height, 1);
        //6. 转码
        sws_scale(swsCtx , (const uint8_t *const *) src_frame->data,
                  src_frame->linesize, 0, src_frame->height,
                  dst_frame->data, dst_frame->linesize);
        render_rend(dst_frame, dst_frame->width, dst_frame->height, _window);
        usleep(40 * 1000);
        av_frame_free(&dst_frame);
*/
    }
}

void FFmpegDecoder::configure(ANativeWindow *ptr, AVCodecID avCodecId) {
    if(_window){
        ANativeWindow_release(_window);
        _window = nullptr;
    }
    _window = ptr;
    _avCodecID = avCodecId;
}

void FFmpegDecoder::start() {

    int ret = 0;


    // 获取解码器
    //    codec = avcodec_find_decoder_by_name("h264");
    codec = avcodec_find_decoder(_avCodecID);
    if (!codec) {
        LOGE("decoder not found");
        return;
    }

    // 初始化解析器上下文
    parserCtx = av_parser_init(codec->id);
    if (!parserCtx) {
        LOGE("av_parser_init error");
        return;
    }

    // 创建上下文
    ctx = avcodec_alloc_context3(codec);
    if (!ctx) {
        LOGE("avcodec_alloc_context3 error");
        stop();
        return;
    }

    // 创建AVPacket
    pkt = av_packet_alloc();
    if (!pkt) {
        LOGE("av_packet_alloc error");
        stop();
        return;
    }

    // 创建AVFrame
    frame = av_frame_alloc();
    if (!frame) {
        LOGE("av_frame_alloc error");
        stop();
        return;
    }

    // 打开解码器
    ret = avcodec_open2(ctx, codec, nullptr);
    if (ret < 0) {
//        ERROR_BUF(ret);
        LOGE("avcodec_open2 error");
        stop();
        return;
    }
    for (auto &&item : _availableInputBufferQueue){
        if (item == nullptr) {
            item = std::make_shared<BufferData<std::vector<char>>>();
        }
    }
    for (int i = 0; i < 2; ++i) {
        auto item = std::make_shared<BufferData<AVFrame>>();
        item->index = i;
        _availableOutputBufferQueue.push(item);
    }

    //启动解码线程
    _t = std::thread(&FFmpegDecoder::parseAndDecode, this);
    _isStart = true;
}

bool FFmpegDecoder::isStart() {
    return _isStart;
}

void FFmpegDecoder::stop() {
    //停止解码线程
    if (_isStart) {
        _stop_decode = true;
        _queueInputBufferQueue.requestShutdown();
        _t.join();
        av_packet_free(&pkt);
        av_frame_free(&frame);
        av_parser_close(parserCtx);
        avcodec_free_context(&ctx);
        _isStart = false;
    }
}

/**
 * 这里应该通过队列传输到另外一个线程，由该线程进行解码。
 * 但是为了验证方便，这里先直接写入然后渲染，并且加入sleep延迟
 * @param data
 * @param data_len
 * @return
 */
bool FFmpegDecoder::writePacket(char *data, int data_len) {

    int ret = 0;
    int inLen = data_len;
    char* inData = data;
    parserCtx->flags |= PARSER_FLAG_COMPLETE_FRAMES;//说明是完整的帧，不需要扫描startCode
    while (inLen > 0) {
        // 经过解析器解析，切割数据。以便解码器解码
        unsigned char vps_nalu[1024] = {0};
        unsigned int vps_len = 0;
        unsigned char sps_nalu[1024] = {0};
        unsigned int sps_len = 0;
        unsigned char pps_nalu[1024] = {0};
        unsigned int pps_len = 0;
        bool issync = false;
        get_h264_nalu(inData, inLen, sps_nalu, &sps_len, pps_nalu, &pps_len, &issync);
        if (!issync && _firstNeedI) {
            return false;
        }
        if (_firstNeedI) {
            _firstNeedI = false;
        }
        if (sps_len != 0) {//矫正sps pps的位置
            memcpy(inData, (uint8_t *) sps_nalu, (size_t) sps_len);
            memcpy(inData + sps_len, (uint8_t *) pps_nalu, (size_t) pps_len);
        }
        ret = av_parser_parse2(parserCtx, ctx,
                               &pkt->data, &pkt->size,
                               (uint8_t *) inData, inLen,
                               AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

        if (ret < 0) {
            ERROR_BUF(ret);
            LOGE("av_parser_parse2 error");
            goto end;
        }

        // 跳过已经解析过的数据
        inData += ret;
        // 减去已经解析过的数据大小
        inLen -= ret;

//    qDebug() << inEnd << pkt->size << ret;

        // 解码
        if (pkt->size > 0 && decode(ctx, pkt, frame) < 0) {
            goto end;
        }
    }

    return true;

    end:
    stop();
    return false;
}

void FFmpegDecoder::readFrame() {

}

int FFmpegDecoder::dequeueInputBuffer() {
    for (int i = 0; i < sizeof(_availableInputBufferQueue) / sizeof(_availableInputBufferQueue[0]); ++i) {
        if (_availableInputBufferQueue[i] != nullptr && !_availableInputBufferQueue[i]->isLock){
            _availableInputBufferQueue[i]->isLock = true;
            return i;
        }
    }
    return -1;
}

void FFmpegDecoder::queueInputBuffer(int index, char *data, int data_len) {
    std::shared_ptr<BufferData<std::vector<char>>> buffer = _availableInputBufferQueue[index];
     auto ptr = buffer->ptr;
    if (ptr->size() < data_len + AV_INPUT_BUFFER_PADDING_SIZE) {
        ptr->resize(data_len + AV_INPUT_BUFFER_PADDING_SIZE);
    }
    std::memcpy(ptr->data(), data, data_len);
    buffer->size = data_len;

    //移动
    _availableInputBufferQueue[index] = nullptr;
    _queueInputBufferQueue.push(buffer);
}
void FFmpegDecoder::putToAvailableInputBufferQueue(std::shared_ptr<BufferData<std::vector<char>>> buffer){
    for (auto & i : _availableInputBufferQueue) {
        if (i == nullptr) {
            buffer->isLock = false;
            i = buffer;
            return;
        }
    }
}

void FFmpegDecoder::parseAndDecode() {
    //1.从阻塞队列pop
    //2.获取后解析然后解码
    //3.将解码数据放到outputBuffer
    while (!_stop_decode) {
        int ret = 0;
        std::shared_ptr<BufferData<std::vector<char>>> buffer;
        ret = _queueInputBufferQueue.pop(buffer);
        if (!ret) {
            continue;
        }

        size_t inLen = buffer->size;
        char* inData = buffer->ptr->data();
        parserCtx->flags |= PARSER_FLAG_COMPLETE_FRAMES;//说明是完整的帧，不需要扫描startCode
        while (inLen > 0) {
            // 经过解析器解析，切割数据。以便解码器解码
            unsigned char vps_nalu[1024] = {0};
            unsigned int vps_len = 0;
            unsigned char sps_nalu[1024] = {0};
            unsigned int sps_len = 0;
            unsigned char pps_nalu[1024] = {0};
            unsigned int pps_len = 0;
            bool issync = false;
            get_h264_nalu(inData, inLen, sps_nalu, &sps_len, pps_nalu, &pps_len, &issync);
            if (!issync && _firstNeedI) {
                //需要是I帧，这个不是I帧，就跳过不解码先
                //buffer归还
                break;
            }
            if (_firstNeedI) {
                _firstNeedI = false;
            }
            if (sps_len != 0) {//矫正sps pps的位置
                memcpy(inData, (uint8_t *) sps_nalu, (size_t) sps_len);
                memcpy(inData + sps_len, (uint8_t *) pps_nalu, (size_t) pps_len);
            }
            ret = av_parser_parse2(parserCtx, ctx,
                                   &pkt->data, &pkt->size,
                                   (uint8_t *) inData, inLen,
                                   AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);

            if (ret < 0) {
                ERROR_BUF(ret);
                LOGE("av_parser_parse2 error");
                break;
            }

            // 跳过已经解析过的数据
            inData += ret;
            // 减去已经解析过的数据大小
            inLen -= ret;

//    qDebug() << inEnd << pkt->size << ret;

            // 解码
            if (pkt->size > 0 && decode(ctx, pkt, frame) < 0) {
                break;
            }
        }
        putToAvailableInputBufferQueue(buffer);
    }
}

int FFmpegDecoder::dequeueOutputBuffer(BufferInfo* bufferInfo) {
    for (auto &&item : _waitForRenderOutputBufferQueue){
        if (!item->isLock) {
            item->isLock = true;
            return item->index;
        }
    }
    return -1;
}

void FFmpegDecoder::releaseOutputBuffer(int index) {

    AVFrame* dst_frame = nullptr;
    std::shared_ptr<BufferData<AVFrame>> dst = nullptr;
    for (int i = 0; i < _waitForRenderOutputBufferQueue.size(); ++i) {
        auto item = _waitForRenderOutputBufferQueue[i];
        if (item->index == index) {
            dst = item;
            dst_frame = item->ptr.get();
            _waitForRenderOutputBufferQueue.erase(_waitForRenderOutputBufferQueue.begin() + i);
            break;
        }
    }
    render_rend(dst_frame, dst_frame->width, dst_frame->height, _window);

    dst->isLock = false;
    _availableOutputBufferQueue.push(dst);
}

void FFmpegDecoder::reset() {

}


