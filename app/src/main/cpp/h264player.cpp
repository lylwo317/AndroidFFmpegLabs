#include <h264player.h>
#include <jni.h>

void H264Player::setANativeWindow() {

}


void H264Player::writeH264Data(const char *buffer) {

}

extern "C"
JNIEXPORT void JNICALL
Java_com_kevin_androidffmpegplayer_H264Player_nativeInit(JNIEnv *env, jobject thiz) {

}

static void setmNativeObject(JNIEnv env, long pointer){
    env.GetObjectClass()
}