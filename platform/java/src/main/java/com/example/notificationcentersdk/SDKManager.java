package com.example.notificationcentersdk;

public class SDKManager {
    static {
        System.loadLibrary("notificationcenter_jni");
    }

    private long nativePtr;

    public SDKManager() {
        this.nativePtr = nativeCreate();
    }

    public void destroy() {
        if (nativePtr != 0) {
            nativeDestroy(nativePtr);
            nativePtr = 0;
        }
    }

    @Override
    protected void finalize() throws Throwable {
        try {
            destroy();
        } finally {
            super.finalize();
        }
    }

    public void configure(String config) {
        nativeConfigure(nativePtr, config);
    }

    public void setConnectionListener(ConnectionListener listener) {
        nativeSetConnectionListener(nativePtr, listener);
    }

    public void connect(String url) {
        nativeConnect(nativePtr, url);
    }

    public BusinessManager getBusinessManager() {
        long businessManagerPtr = nativeGetBusinessManager(nativePtr);
        return new BusinessManager(businessManagerPtr);
    }

    public int poll() {
        return nativePoll(nativePtr);
    }

    private native long nativeCreate();
    private native void nativeDestroy(long nativePtr);
    private native void nativeConfigure(long nativePtr, String config);
    private native void nativeSetConnectionListener(long nativePtr, ConnectionListener listener);
    private native void nativeConnect(long nativePtr, String url);
    private native long nativeGetBusinessManager(long nativePtr);
    private native int nativePoll(long nativePtr);
} 