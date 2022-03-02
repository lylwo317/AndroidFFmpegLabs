//
// Created by kevin on 2022/3/2.
//

#ifndef ANDROIDFFMPEGPLAYER_STREAM_DATA_H
#define ANDROIDFFMPEGPLAYER_STREAM_DATA_H
#include <cstdint>

class BufferInfo{
public:
long presentationTimeUs;

};

template <class _Tp>
class BufferData {
public:
    std::shared_ptr<_Tp> ptr;
    bool isLock = false;
};

#endif //ANDROIDFFMPEGPLAYER_STREAM_DATA_H
