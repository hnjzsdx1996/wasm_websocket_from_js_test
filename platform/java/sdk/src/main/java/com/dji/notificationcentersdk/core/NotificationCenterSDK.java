package com.dji.notificationcentersdk.core;

import com.dji.notificationcentersdk.generated.*;

/**
 * NotificationCenter SDK 包装类
 * 提供自动JNI库加载功能
 */
public class NotificationCenterSDK {
    
    private static boolean initialized = false;
    private SDKManager sdkManager;
    
    /**
     * 私有构造函数，防止直接实例化
     */
    private NotificationCenterSDK() {
        // 确保JNI库已加载
        if (!JniLibraryLoader.isLibraryLoaded()) {
            JniLibraryLoader.loadLibrary();
        }
        
        // 创建SDK管理器实例
        sdkManager = new SDKManager();
    }
    
    /**
     * 获取SDK实例（单例模式）
     */
    public static synchronized NotificationCenterSDK getInstance() {
        if (!initialized) {
            // 确保JNI库已加载
            if (!JniLibraryLoader.isLibraryLoaded()) {
                JniLibraryLoader.loadLibrary();
            }
            initialized = true;
        }
        return new NotificationCenterSDK();
    }
    
    /**
     * 初始化SDK
     */
    public void init(SdkInitializeInfo info) {
        sdkManager.init(info);
    }
    
    /**
     * 检查SDK是否已初始化
     */
    public boolean isInit() {
        return sdkManager.isInit();
    }
    
    /**
     * 设置WebSocket事件监听器
     */
    public void setWebsocketEventListener(ConnectionListener listener) {
        sdkManager.setWebsocketEventListener(listener);
    }
    
    /**
     * 连接到WebSocket服务器
     */
    public void connect(String url) {
        sdkManager.connect(url);
    }
    
    /**
     * 获取业务管理器
     */
    public BusinessManager getBusinessManager() {
        return sdkManager.getBusinessManager();
    }
    
    /**
     * 轮询事件（静态方法）
     */
    public static long poll() {
        // 确保JNI库已加载
        if (!JniLibraryLoader.isLibraryLoaded()) {
            JniLibraryLoader.loadLibrary();
        }
        return SDKManager.poll();
    }
    
    /**
     * 获取底层的SDK管理器实例
     */
    public SDKManager getSDKManager() {
        return sdkManager;
    }
} 