/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.3.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.dji.notificationcentersdk.generated;

public class AircraftBatteryInfoCallback {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected AircraftBatteryInfoCallback(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(AircraftBatteryInfoCallback obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected static long swigRelease(AircraftBatteryInfoCallback obj) {
    long ptr = 0;
    if (obj != null) {
      if (!obj.swigCMemOwn)
        throw new RuntimeException("Cannot release ownership as memory is not owned");
      ptr = obj.swigCPtr;
      obj.swigCMemOwn = false;
      obj.delete();
    }
    return ptr;
  }

  @SuppressWarnings({"deprecation", "removal"})
  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        notificationcenterJNI.delete_AircraftBatteryInfoCallback(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  protected void swigDirectorDisconnect() {
    swigCMemOwn = false;
    delete();
  }

  public void swigReleaseOwnership() {
    swigCMemOwn = false;
    notificationcenterJNI.AircraftBatteryInfoCallback_change_ownership(this, swigCPtr, false);
  }

  public void swigTakeOwnership() {
    swigCMemOwn = true;
    notificationcenterJNI.AircraftBatteryInfoCallback_change_ownership(this, swigCPtr, true);
  }

  public void invoke(AircraftBatteryInfo message) {
    notificationcenterJNI.AircraftBatteryInfoCallback_invoke(swigCPtr, this, AircraftBatteryInfo.getCPtr(message), message);
  }

  public AircraftBatteryInfoCallback() {
    this(notificationcenterJNI.new_AircraftBatteryInfoCallback(), true);
    notificationcenterJNI.AircraftBatteryInfoCallback_director_connect(this, swigCPtr, true, true);
  }

}
