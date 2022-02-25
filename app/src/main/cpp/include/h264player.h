#ifndef H264_PLAYER
#define H264_PLAYER

//#include <android/native_window.h>

class H264Player {
public:
    void start();
    //设置需要渲染到的窗口
    void setANativeWindow(long ptr);
//    void start();
    //写入h264并进行渲染播放
    void writeH264Data(const char* buffer);
};

#endif
