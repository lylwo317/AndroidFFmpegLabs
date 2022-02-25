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
%}

%apply(char *STRING, size_t LENGTH) { (char* data, int data_len) };
//%rename  KxFFmpeg;
%include "h264player.h"
%include <stdint.i>
