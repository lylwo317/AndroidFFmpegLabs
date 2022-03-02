/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.kevin.androidffmpegplayer.jni;

public class KxFFmpegJNI {

  static {
      try {
          System.loadLibrary("kxffmpeg");
      } catch (UnsatisfiedLinkError e) {
          System.err.println("libkxffmpeg.so failed to load. \n" + e);
          System.exit(1);
      }
  }

  public final static native int AV_CODEC_ID_Y41P_get();
  public final static native int AV_CODEC_ID_FIRST_AUDIO_get();
  public final static native int AV_CODEC_ID_PCM_S16LE_get();
  public final static native int AV_CODEC_ID_PCM_S64LE_get();
  public final static native int AV_CODEC_ID_ADPCM_IMA_QT_get();
  public final static native int AV_CODEC_ID_ADPCM_AFC_get();
  public final static native int AV_CODEC_ID_AMR_NB_get();
  public final static native int AV_CODEC_ID_RA_144_get();
  public final static native int AV_CODEC_ID_ROQ_DPCM_get();
  public final static native int AV_CODEC_ID_SDX2_DPCM_get();
  public final static native int AV_CODEC_ID_MP2_get();
  public final static native int AV_CODEC_ID_FFWAVESYNTH_get();
  public final static native int AV_CODEC_ID_FIRST_SUBTITLE_get();
  public final static native int AV_CODEC_ID_DVD_SUBTITLE_get();
  public final static native int AV_CODEC_ID_MICRODVD_get();
  public final static native int AV_CODEC_ID_FIRST_UNKNOWN_get();
  public final static native int AV_CODEC_ID_TTF_get();
  public final static native int AV_CODEC_ID_BINTEXT_get();
  public final static native int AV_CODEC_ID_PROBE_get();
  public final static native int AV_CODEC_ID_MPEG2TS_get();
  public final static native int AV_CODEC_ID_MPEG4SYSTEMS_get();
  public final static native int AV_CODEC_ID_FFMETADATA_get();
  public final static native int AV_CODEC_ID_WRAPPED_AVFRAME_get();
  public final static native int avcodec_get_type(int jarg1);
  public final static native String avcodec_get_name(int jarg1);
  public final static native void FFmpegDecoder_startDecoder(long jarg1, FFmpegDecoder jarg1_);
  public final static native boolean FFmpegDecoder_isStart(long jarg1, FFmpegDecoder jarg1_);
  public final static native boolean FFmpegDecoder_writePacket(long jarg1, FFmpegDecoder jarg1_, byte[] jarg2);
  public final static native void FFmpegDecoder_readFrame(long jarg1, FFmpegDecoder jarg1_);
  public final static native void FFmpegDecoder_stopDecoder(long jarg1, FFmpegDecoder jarg1_);
  public final static native void FFmpegDecoder_configureFromJava(long jarg1, FFmpegDecoder jarg1_, Object jarg2, int jarg3);
  public final static native long new_FFmpegDecoder();
  public final static native void delete_FFmpegDecoder(long jarg1);
}