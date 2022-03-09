%module KxFFmpeg

%pragma(java) jniclasscode=%{
  static {
      try {
          System.loadLibrary("decoder_jni");
      } catch (UnsatisfiedLinkError e) {
          System.err.println("libdecoder_jni.so failed to load. \n" + e);
          System.exit(1);
      }
  }
%}

%{
#include "ffmpeg_decoder.h"
#include <android/native_window_jni.h>
#include <libavcodec/avcodec.h>
#include "buffer_info.h"
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
%include <stdint.i>//转换int64_t这种
%typemap(in)    (char* data_from) {
    $1 = (char *)JCALL1(GetDirectBufferAddress, jenv, $input);
}
%typemap(freearg) char *data_from ""
%typemap(jni)       (char* data_from) "jobject" //对应的jni类型
%typemap(jtype)     (char* data_from) "java.nio.ByteBuffer"//对应java中间人的类型
%typemap(jstype)    (char* data_from) "java.nio.ByteBuffer"//对应java类型
%typemap(javain)    (char* data_from) "$javainput"//Conversion from jstype to jtype
%typemap(javaout)   (char* data_from) {//Conversion from jtype to jstype
    return $jnicall;
}
%include "codec_id.h"
%include "ffmpeg_decoder.h"
%include "buffer_info.h"