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
#include "ffmpeg_decoder.h"
#include <android/native_window_jni.h>
#include <libavcodec/codec_id.h>
%}

%ignore FFmpegDecoder::setANativeWindow;
%ignore FFmpegDecoder::configure;
%typemap(in) jobject surface {
    $1 =(jobject) ANativeWindow_fromSurface(jenv, $input);
}
%extend FFmpegDecoder {
        void configureFromJava(jobject surface, AVCodecID codecId) {
            $self->configure((ANativeWindow*)surface, codecId);
        }
}
%apply(const char *STRING, size_t LENGTH) { (const char* data, int data_len) };
%apply(char *STRING, size_t LENGTH) { (char* data, int data_len) };
//%rename  KxFFmpeg;
%include "libavcodec/codec_id.h"
%include "ffmpeg_decoder.h"
%include <stdint.i>
