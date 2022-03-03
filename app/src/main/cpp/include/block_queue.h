//
// Created by kevin on 2022/3/2.
//

#ifndef ANDROIDFFMPEGPLAYER_BLOCK_QUEUE_H
#define ANDROIDFFMPEGPLAYER_BLOCK_QUEUE_H

#include <mutex>
#include <condition_variable>
#include <deque>

template <typename T> class BlockingQueue {
    std::condition_variable _cvCanPop;
    std::mutex _sync;
    std::queue<T> _qu;
    bool _bShutdown = false;

public:
    void push(const T& item)
    {
        {
            std::unique_lock<std::mutex> lock(_sync);
            _qu.push(item);
        }
        _cvCanPop.notify_one();
    }

    void requestShutdown() {
        {
            std::unique_lock<std::mutex> lock(_sync);
            _bShutdown = true;
        }
        _cvCanPop.notify_all();
    }

    bool pop(T &item) {
        std::unique_lock<std::mutex> lock(_sync);
        for (;;) {
            if (_qu.empty()) {
                if (_bShutdown) {
                    return false;
                }
            }
            else {
                break;
            }
            _cvCanPop.wait(lock);
        }
        item = std::move(_qu.front());
        _qu.pop();
        return true;
    }
};

#endif //ANDROIDFFMPEGPLAYER_BLOCK_QUEUE_H
