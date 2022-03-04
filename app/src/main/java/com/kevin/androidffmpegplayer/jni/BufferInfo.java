/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.kevin.androidffmpegplayer.jni;

public class BufferInfo {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected BufferInfo(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(BufferInfo obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  @SuppressWarnings("deprecation")
  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        KxFFmpegJNI.delete_BufferInfo(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setPresentationTimeUs(int value) {
    KxFFmpegJNI.BufferInfo_presentationTimeUs_set(swigCPtr, this, value);
  }

  public int getPresentationTimeUs() {
    return KxFFmpegJNI.BufferInfo_presentationTimeUs_get(swigCPtr, this);
  }

  public BufferInfo() {
    this(KxFFmpegJNI.new_BufferInfo(), true);
  }

}