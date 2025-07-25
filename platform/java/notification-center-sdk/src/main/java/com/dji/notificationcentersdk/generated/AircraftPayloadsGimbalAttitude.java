/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.3.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.dji.notificationcentersdk.generated;

public class AircraftPayloadsGimbalAttitude {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected AircraftPayloadsGimbalAttitude(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(AircraftPayloadsGimbalAttitude obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected static long swigRelease(AircraftPayloadsGimbalAttitude obj) {
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
        notificationcenterJNI.delete_AircraftPayloadsGimbalAttitude(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setPayloads_gimbal_attitude(AircraftPayloadGimbalAttitudeMap value) {
    notificationcenterJNI.AircraftPayloadsGimbalAttitude_payloads_gimbal_attitude_set(swigCPtr, this, AircraftPayloadGimbalAttitudeMap.getCPtr(value), value);
  }

  public AircraftPayloadGimbalAttitudeMap getPayloads_gimbal_attitude() {
    long cPtr = notificationcenterJNI.AircraftPayloadsGimbalAttitude_payloads_gimbal_attitude_get(swigCPtr, this);
    return (cPtr == 0) ? null : new AircraftPayloadGimbalAttitudeMap(cPtr, false);
  }

  public AircraftPayloadsGimbalAttitude() {
    this(notificationcenterJNI.new_AircraftPayloadsGimbalAttitude(), true);
  }

}
