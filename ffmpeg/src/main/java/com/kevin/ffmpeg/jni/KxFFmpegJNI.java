/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.kevin.ffmpeg.jni;

public class KxFFmpegJNI {

  static {
      try {
          System.loadLibrary("decoder_jni");
      } catch (UnsatisfiedLinkError e) {
          System.err.println("libdecoder_jni.so failed to load. \n" + e);
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
  public final static native void delete_FFmpegDecoder(long jarg1);
  public final static native void FFmpegDecoder_start(long jarg1, FFmpegDecoder jarg1_);
  public final static native boolean FFmpegDecoder_isStart(long jarg1, FFmpegDecoder jarg1_);
  public final static native void FFmpegDecoder_stop(long jarg1, FFmpegDecoder jarg1_);
  public final static native void FFmpegDecoder_reset(long jarg1, FFmpegDecoder jarg1_);
  public final static native int FFmpegDecoder_dequeueInputBuffer(long jarg1, FFmpegDecoder jarg1_);
  public final static native void FFmpegDecoder_queueInputBuffer(long jarg1, FFmpegDecoder jarg1_, int jarg2, java.nio.ByteBuffer jarg3, int jarg4, long jarg5);
  public final static native int FFmpegDecoder_dequeueOutputBuffer(long jarg1, FFmpegDecoder jarg1_, long jarg2, BufferInfo jarg2_);
  public final static native void FFmpegDecoder_releaseOutputBuffer(long jarg1, FFmpegDecoder jarg1_, int jarg2);
  public final static native void FFmpegDecoder_configureFromJava(long jarg1, FFmpegDecoder jarg1_, java.lang.Object jarg2, int jarg3);
  public final static native long new_FFmpegDecoder();
  public final static native void BufferInfo_presentationTimeUs_set(long jarg1, BufferInfo jarg1_, long jarg2);
  public final static native long BufferInfo_presentationTimeUs_get(long jarg1, BufferInfo jarg1_);
  public final static native void BufferInfo_width_set(long jarg1, BufferInfo jarg1_, int jarg2);
  public final static native int BufferInfo_width_get(long jarg1, BufferInfo jarg1_);
  public final static native void BufferInfo_height_set(long jarg1, BufferInfo jarg1_, int jarg2);
  public final static native int BufferInfo_height_get(long jarg1, BufferInfo jarg1_);
  public final static native long new_BufferInfo();
  public final static native void delete_BufferInfo(long jarg1);
}