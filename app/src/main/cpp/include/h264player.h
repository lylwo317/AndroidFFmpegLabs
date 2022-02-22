#ifndef H264_PLAYER
#define H264_PLAYER

class H264Player {

    //设置需要渲染到的窗口
    void setANativeWindow();

//    void start();

    //写入h264并进行渲染播放
    void writeH264Data(const char* buffer);
};

#endif
