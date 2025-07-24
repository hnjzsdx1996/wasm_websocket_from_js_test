package com.dji.notificationcentersdk.core;

import com.dji.notificationcentersdk.generated.*;

/**
 * NotificationCenter SDK 包装类
 * 提供自动JNI库加载功能和自动poll
 */
public class NotificationCenterSDK {
    
    private static boolean initialized = false;
    private SDKManager sdkManager;
    private Thread pollThread;
    private volatile boolean isRunning = false;
    private static final long POLL_INTERVAL = 100; // 100ms
    
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
        // 启动自动poll线程
        startPollThread();
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
     * 取消监听
     */
    public void cancelListen(long listenId) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager != null) {
            businessManager.cancelObserve((int)listenId);
        }
    }

    /**
     * 获取底层的SDK管理器实例
     */
    public SDKManager getSDKManager() {
        return sdkManager;
    }
    
    /**
     * 启动自动poll线程
     */
    private void startPollThread() {
        if (pollThread != null && pollThread.isAlive()) {
            return; // 已经运行了
        }
        
        isRunning = true;
        pollThread = new Thread(() -> {
            while (isRunning) {
                try {
                    long taskCount = SDKManager.poll();
                    if (taskCount > 0) {
                        System.out.println("[SDK] Poll executed " + taskCount + " tasks");
                    }
                    Thread.sleep(POLL_INTERVAL);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                    break;
                } catch (Exception e) {
                    System.err.println("[SDK] Poll error: " + e.getMessage());
                }
            }
        }, "NotificationCenterSDK-PollThread");
        
        pollThread.setDaemon(true);
        pollThread.start();
        System.out.println("[SDK] Auto-poll thread started");
    }
    
    /**
     * 停止自动poll线程
     */
    private void stopPollThread() {
        isRunning = false;
        if (pollThread != null) {
            pollThread.interrupt();
            try {
                pollThread.join(1000); // 等待最多1秒
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            pollThread = null;
            System.out.println("[SDK] Auto-poll thread stopped");
        }
    }
    
    /**
     * 轮询事件（静态方法）- 保留用于兼容性，但不推荐使用
     * @deprecated 使用自动poll，无需手动调用
     */
    @Deprecated
    public static long poll() {
        // 确保JNI库已加载
        if (!JniLibraryLoader.isLibraryLoaded()) {
            JniLibraryLoader.loadLibrary();
        }
        return SDKManager.poll();
    }
} 