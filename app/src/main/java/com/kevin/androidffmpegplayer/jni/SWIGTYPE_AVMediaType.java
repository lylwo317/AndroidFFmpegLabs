/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.1
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.kevin.androidffmpegplayer.jni;

public final class SWIGTYPE_AVMediaType {
  public final int swigValue() {
    return swigValue;
  }

  public String toString() {
    return swigName;
  }

  public static SWIGTYPE_AVMediaType swigToEnum(int swigValue) {
    if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
      return swigValues[swigValue];
    for (int i = 0; i < swigValues.length; i++)
      if (swigValues[i].swigValue == swigValue)
        return swigValues[i];
    throw new IllegalArgumentException("No enum " + SWIGTYPE_AVMediaType.class + " with value " + swigValue);
  }

  private SWIGTYPE_AVMediaType(String swigName) {
    this.swigName = swigName;
    this.swigValue = swigNext++;
  }

  private SWIGTYPE_AVMediaType(String swigName, int swigValue) {
    this.swigName = swigName;
    this.swigValue = swigValue;
    swigNext = swigValue+1;
  }

  private SWIGTYPE_AVMediaType(String swigName, SWIGTYPE_AVMediaType swigEnum) {
    this.swigName = swigName;
    this.swigValue = swigEnum.swigValue;
    swigNext = this.swigValue+1;
  }

  private static SWIGTYPE_AVMediaType[] swigValues = {  };
  private static int swigNext = 0;
  private final int swigValue;
  private final String swigName;
}

