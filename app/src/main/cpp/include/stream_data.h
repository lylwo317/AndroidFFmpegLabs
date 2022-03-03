//
// Created by kevin on 2022/3/2.
//

#ifndef ANDROIDFFMPEGPLAYER_STREAM_DATA_H
#define ANDROIDFFMPEGPLAYER_STREAM_DATA_H
#include <cstdint>

template <class _Tp>
class BufferData {
public:
    int index = -1;
    std::shared_ptr<_Tp> ptr = std::make_shared<_Tp>();
    size_t size = 0;
    bool isLock = false;
};

#endif //ANDROIDFFMPEGPLAYER_STREAM_DATA_H
