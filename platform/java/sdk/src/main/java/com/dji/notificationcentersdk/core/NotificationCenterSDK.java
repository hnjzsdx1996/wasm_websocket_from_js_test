package com.dji.notificationcentersdk.core;

import com.dji.notificationcentersdk.generated.*;
import com.dji.notificationcentersdk.core.listeners.*;

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
     * 用户友好的监听方法 - 飞机位置
     */
    public long listenAircraftLocation(String deviceSn, NotificationFrequency frequency, 
                                     AircraftLocationListener listener) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager == null) {
            throw new IllegalStateException("BusinessManager not available");
        }
        
        return businessManager.ListenAircraftLocation(
            new AircraftLocationCallback() {
                @Override
                public void invoke(AircraftLocation message) {
                    if (listener != null) {
                        listener.onAircraftLocationReceived(deviceSn, message);
                    }
                }
            },
            new SDKSubscribeResultCallback() {
                @Override
                public void invoke(NotificationCenterErrorCode errorCode) {
                    if (listener != null) {
                        listener.onSubscribeResult(deviceSn, errorCode);
                    }
                }
            },
            deviceSn,
            frequency
        );
    }
    
    /**
     * 用户友好的监听方法 - 飞机姿态
     */
    public long listenAircraftAttitude(String deviceSn, NotificationFrequency frequency, 
                                     AircraftAttitudeListener listener) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager == null) {
            throw new IllegalStateException("BusinessManager not available");
        }
        
        return businessManager.ListenAircraftAttitude(
            new AircraftAttitudeCallback() {
                @Override
                public void invoke(AircraftAttitude message) {
                    if (listener != null) {
                        listener.onAircraftAttitudeReceived(deviceSn, message);
                    }
                }
            },
            new SDKSubscribeResultCallback() {
                @Override
                public void invoke(NotificationCenterErrorCode errorCode) {
                    if (listener != null) {
                        listener.onSubscribeResult(deviceSn, errorCode);
                    }
                }
            },
            deviceSn,
            frequency
        );
    }
    
    /**
     * 用户友好的监听方法 - 飞机速度
     */
    public long listenAircraftSpeed(String deviceSn, NotificationFrequency frequency, 
                                  AircraftSpeedListener listener) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager == null) {
            throw new IllegalStateException("BusinessManager not available");
        }
        
        return businessManager.ListenAircraftSpeed(
            new AircraftSpeedCallback() {
                @Override
                public void invoke(AircraftSpeed message) {
                    if (listener != null) {
                        listener.onAircraftSpeedReceived(deviceSn, message);
                    }
                }
            },
            new SDKSubscribeResultCallback() {
                @Override
                public void invoke(NotificationCenterErrorCode errorCode) {
                    if (listener != null) {
                        listener.onSubscribeResult(deviceSn, errorCode);
                    }
                }
            },
            deviceSn,
            frequency
        );
    }
    
    /**
     * 用户友好的监听方法 - 设备OSD
     */
    public long listenDeviceOsd(String deviceSn, NotificationFrequency frequency, 
                              DeviceOsdListener listener) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager == null) {
            throw new IllegalStateException("BusinessManager not available");
        }
        
        return businessManager.ListenDeviceOsd(
            new DeviceOsdCallback() {
                @Override
                public void invoke(DeviceOsd message) {
                    if (listener != null) {
                        listener.onDeviceOsdReceived(deviceSn, message);
                    }
                }
            },
            new SDKSubscribeResultCallback() {
                @Override
                public void invoke(NotificationCenterErrorCode errorCode) {
                    if (listener != null) {
                        listener.onSubscribeResult(deviceSn, errorCode);
                    }
                }
            },
            deviceSn,
            frequency
        );
    }
    
    /**
     * 用户友好的监听方法 - 飞机电池信息
     */
    public long listenAircraftBatteryInfo(String deviceSn, NotificationFrequency frequency, 
                                        AircraftBatteryInfoListener listener) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager == null) {
            throw new IllegalStateException("BusinessManager not available");
        }
        
        return businessManager.ListenAircraftBatteryInfo(
            new AircraftBatteryInfoCallback() {
                @Override
                public void invoke(AircraftBatteryInfo message) {
                    if (listener != null) {
                        listener.onAircraftBatteryInfoReceived(deviceSn, message);
                    }
                }
            },
            new SDKSubscribeResultCallback() {
                @Override
                public void invoke(NotificationCenterErrorCode errorCode) {
                    if (listener != null) {
                        listener.onSubscribeResult(deviceSn, errorCode);
                    }
                }
            },
            deviceSn,
            frequency
        );
    }
    
    /**
     * 用户友好的监听方法 - 飞机控制代码
     */
    public long listenAircraftControlCode(String deviceSn, NotificationFrequency frequency, 
                                        AircraftControlCodeListener listener) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager == null) {
            throw new IllegalStateException("BusinessManager not available");
        }
        
        return businessManager.ListenAircraftControlCode(
            new AircraftControlCodeCallback() {
                @Override
                public void invoke(AircraftControlCode message) {
                    if (listener != null) {
                        listener.onAircraftControlCodeReceived(deviceSn, message);
                    }
                }
            },
            new SDKSubscribeResultCallback() {
                @Override
                public void invoke(NotificationCenterErrorCode errorCode) {
                    if (listener != null) {
                        listener.onSubscribeResult(deviceSn, errorCode);
                    }
                }
            },
            deviceSn,
            frequency
        );
    }
    
    /**
     * 用户友好的监听方法 - 飞机风速
     */
    public long listenAircraftWindSpeed(String deviceSn, NotificationFrequency frequency, 
                                      AircraftWindSpeedListener listener) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager == null) {
            throw new IllegalStateException("BusinessManager not available");
        }
        
        return businessManager.ListenAircraftWindSpeed(
            new AircraftWindSpeedCallback() {
                @Override
                public void invoke(AircraftWindSpeed message) {
                    if (listener != null) {
                        listener.onAircraftWindSpeedReceived(deviceSn, message);
                    }
                }
            },
            new SDKSubscribeResultCallback() {
                @Override
                public void invoke(NotificationCenterErrorCode errorCode) {
                    if (listener != null) {
                        listener.onSubscribeResult(deviceSn, errorCode);
                    }
                }
            },
            deviceSn,
            frequency
        );
    }
    
    /**
     * 用户友好的监听方法 - 飞机模式代码
     */
    public long listenAircraftModeCode(String deviceSn, NotificationFrequency frequency, 
                                     AircraftModeCodeListener listener) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager == null) {
            throw new IllegalStateException("BusinessManager not available");
        }
        
        return businessManager.ListenAircraftModeCode(
            new AircraftModeCodeCallback() {
                @Override
                public void invoke(AircraftModeCode message) {
                    if (listener != null) {
                        listener.onAircraftModeCodeReceived(deviceSn, message);
                    }
                }
            },
            new SDKSubscribeResultCallback() {
                @Override
                public void invoke(NotificationCenterErrorCode errorCode) {
                    if (listener != null) {
                        listener.onSubscribeResult(deviceSn, errorCode);
                    }
                }
            },
            deviceSn,
            frequency
        );
    }
    
    /**
     * 用户友好的监听方法 - 无人机在机场状态
     */
    public long listenDroneInDock(String deviceSn, NotificationFrequency frequency, 
                                DroneInDockListener listener) {
        BusinessManager businessManager = getBusinessManager();
        if (businessManager == null) {
            throw new IllegalStateException("BusinessManager not available");
        }
        
        return businessManager.ListenDroneInDock(
            new DroneInDockCallback() {
                @Override
                public void invoke(DroneInDock message) {
                    if (listener != null) {
                        listener.onDroneInDockReceived(deviceSn, message);
                    }
                }
            },
            new SDKSubscribeResultCallback() {
                @Override
                public void invoke(NotificationCenterErrorCode errorCode) {
                    if (listener != null) {
                        listener.onSubscribeResult(deviceSn, errorCode);
                    }
                }
            },
            deviceSn,
            frequency
        );
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