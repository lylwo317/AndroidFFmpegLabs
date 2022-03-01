%module KxFFmpeg

%pragma(java) jniclasscode=%{
  static {
      try {
          System.loadLibrary("kxffmpeg");
      } catch (UnsatisfiedLinkError e) {
          System.err.println("libkxffmpeg.so failed to load. \n" + e);
          System.exit(1);
      }
  }
%}

%{
#include "h264player.h"
#include <android/native_window_jni.h>
%}

%ignore FFmpegDecoder::setANativeWindow;
%typemap(in) jobject surface {
    $1 =(jobject) ANativeWindow_fromSurface(jenv, $input);
}
%extend FFmpegDecoder {
        void setWindow(jobject surface) {
            $self->setANativeWindow((ANativeWindow*)surface);
        }
}
%apply(const char *STRING, size_t LENGTH) { (const char* data, int data_len) };
%apply(char *STRING, size_t LENGTH) { (char* data, int data_len) };
//%rename  KxFFmpeg;
%include "h264player.h"
%include <stdint.i>
