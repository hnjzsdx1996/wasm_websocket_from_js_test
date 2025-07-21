package com.example.notificationcentersdk;

public class BusinessManager {
    private long nativePtr;

    // a protected constructor
    protected BusinessManager(long nativePtr) {
        this.nativePtr = nativePtr;
    }

    public long listenAircraftLocation(String deviceSn, int freq) {
        return nativeListenAircraftLocation(nativePtr, deviceSn, freq);
    }

    public void cancelObserve(long listenId) {
        nativeCancelObserve(nativePtr, listenId);
    }

    private native long nativeListenAircraftLocation(long nativePtr, String deviceSn, int freq);
    private native void nativeCancelObserve(long nativePtr, long listenId);
} 