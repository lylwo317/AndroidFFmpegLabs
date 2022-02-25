#include <h264player.h>
#include <jni.h>

#include "Log.h"
extern "C"{
#include <libavcodec/avcodec.h>
}

void H264Player::start() {
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

    int ret = 0;

    // 获取解码器
    //    codec = avcodec_find_decoder_by_name("h264");
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
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
        goto end;
    }

    // 创建AVPacket
    pkt = av_packet_alloc();
    if (!pkt) {
        LOGE("av_packet_alloc error");
        goto end;
    }

    // 创建AVFrame
    frame = av_frame_alloc();
    if (!frame) {
        LOGE("av_frame_alloc error");
        goto end;
    }

    // 打开解码器
    ret = avcodec_open2(ctx, codec, nullptr);
    if (ret < 0) {
//        ERROR_BUF(ret);
        LOGE("avcodec_open2 error");
        goto end;
    }

    end:
    av_packet_free(&pkt);
    av_frame_free(&frame);
    av_parser_close(parserCtx);
    avcodec_free_context(&ctx);
}

void H264Player::writeH264Data(const char *buffer) {

}

void H264Player::setANativeWindow(long ptr) {

}
