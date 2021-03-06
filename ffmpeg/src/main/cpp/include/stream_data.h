//
// Created by kevin on 2022/3/2.
//

#ifndef ANDROIDFFMPEGPLAYER_STREAM_DATA_H
#define ANDROIDFFMPEGPLAYER_STREAM_DATA_H
#include <cstdint>
#include <vector>
extern "C"{
#include "libavcodec/avcodec.h"
};

class BufferData {
public:
    int index = -1;
    size_t size = 0;
    int64_t pts = 0;
    bool isLock = false;
};

class InputBufferData : public BufferData {
public:
    std::vector<char> data;
};

class OutputBufferData : public BufferData {
public:
    std::shared_ptr<AVFrame> ptr = nullptr;
    OutputBufferData(){
        AVFrame* frame = av_frame_alloc();
        ptr = std::shared_ptr<AVFrame>(frame, [](AVFrame* _ptr) {
            av_freep(&_ptr->data[0]);
            av_frame_free(&_ptr);
        });
    }
};

#endif //ANDROIDFFMPEGPLAYER_STREAM_DATA_H
