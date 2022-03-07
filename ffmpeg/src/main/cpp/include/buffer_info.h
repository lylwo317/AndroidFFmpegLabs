//
// Created by kevin on 2022/3/2.
//

#ifndef ANDROIDFFMPEGPLAYER_BUFFER_INFO_H
#define ANDROIDFFMPEGPLAYER_BUFFER_INFO_H
#include <cstdint>

class BufferInfo{
public:
    int64_t presentationTimeUs = 0;
    int32_t width = -1;
    int32_t height = -1;

};

#endif //ANDROIDFFMPEGPLAYER_BUFFER_INFO_H
