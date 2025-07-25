/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (https://www.swig.org).
 * Version 4.3.1
 *
 * Do not make changes to this file unless you know what you are doing - modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.dji.notificationcentersdk.generated;

public class AircraftPayloadsList {
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  protected AircraftPayloadsList(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr(AircraftPayloadsList obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected static long swigRelease(AircraftPayloadsList obj) {
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
        notificationcenterJNI.delete_AircraftPayloadsList(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public void setPayloads_list(StringVector value) {
    notificationcenterJNI.AircraftPayloadsList_payloads_list_set(swigCPtr, this, StringVector.getCPtr(value), value);
  }

  public StringVector getPayloads_list() {
    long cPtr = notificationcenterJNI.AircraftPayloadsList_payloads_list_get(swigCPtr, this);
    return (cPtr == 0) ? null : new StringVector(cPtr, false);
  }

  public AircraftPayloadsList() {
    this(notificationcenterJNI.new_AircraftPayloadsList(), true);
  }

}
