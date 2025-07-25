package com.dji.notificationcentersdk.core;

import com.dji.notificationcentersdk.generated.*;

/**
 * NotificationCenter 客户端接口
 * 提供符合文档模板的监听方法
 */
public class NotificationCenterClient {
    
    private final NotificationCenterSDK sdk;
    private static boolean initialized = false;
    
    /**
     * 私有构造函数，防止直接实例化
     */
    private NotificationCenterClient() {
        this.sdk = NotificationCenterSDK.getInstance();
    }
    
    /**
     * 获取客户端实例（单例模式）
     */
    public static synchronized NotificationCenterClient getInstance() {
        if (!initialized) {
            initialized = true;
        }
        return new NotificationCenterClient();
    }
    
    /**
     * 初始化SDK
     * @param initInfo 初始化信息
     */
    public void init(SdkInitializeInfo initInfo) {
        sdk.init(initInfo);
    }
    
    /**
     * 检查SDK是否已初始化
     * @return 是否已初始化
     */
    public boolean isInit() {
        return sdk.isInit();
    }
    
    /**
     * 设置WebSocket事件监听器
     * @param listener 连接监听器
     */
    public void setWebsocketEventListener(ConnectionListener listener) {
        sdk.setWebsocketEventListener(listener);
    }
    
    /**
     * 连接到WebSocket服务器
     * @param url WebSocket服务器地址
     */
    public void connect(String url) {
        sdk.connect(url);
    }
    
    /**
     * 消息回调函数，收到位置消息时会被调用，参数为消息内容
     */
    @FunctionalInterface
    public interface OnSubscribeMessageCallback<T> {
        void invoke(T message);
    }
    
    /**
     * 订阅结果回调函数，订阅成功或失败时会被调用，参数为结果信息
     */
    @FunctionalInterface
    public interface OnSubscribeResultCallback {
        void invoke(NotificationCenterErrorCode errorCode);
    }
    
    /**
     * 订阅飞机位置消息
     *
     * @param onMessagesCallback    消息回调函数，收到位置消息时会被调用，参数为消息内容
     * @param onResultCallback      订阅结果回调函数，订阅成功或失败时会被调用，参数为结果信息
     * @param deviceSn              设备序列号（SN），用于唯一标识目标飞机
     * @param freq                  消息推送频率，详见 NotificationFrequency 枚举定义
     * @return long  返回监听ID，成功为 ListenId，失败返回 -1
     */
    public long ListenAircraftLocation(
            OnSubscribeMessageCallback<AircraftLocation> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenAircraftLocation(
                new AircraftLocationCallback() {
                    @Override
                    public void invoke(AircraftLocation message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅飞机速度信息
     */
    public long ListenAircraftSpeed(
            OnSubscribeMessageCallback<AircraftSpeed> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenAircraftSpeed(
                new AircraftSpeedCallback() {
                    @Override
                    public void invoke(AircraftSpeed message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅飞机姿态信息
     */
    public long ListenAircraftAttitude(
            OnSubscribeMessageCallback<AircraftAttitude> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenAircraftAttitude(
                new AircraftAttitudeCallback() {
                    @Override
                    public void invoke(AircraftAttitude message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅飞机模式信息
     */
    public long ListenAircraftModeCode(
            OnSubscribeMessageCallback<AircraftModeCode> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenAircraftModeCode(
                new AircraftModeCodeCallback() {
                    @Override
                    public void invoke(AircraftModeCode message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅飞机控制模式信息
     */
    public long ListenAircraftControlCode(
            OnSubscribeMessageCallback<AircraftControlCode> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenAircraftControlCode(
                new AircraftControlCodeCallback() {
                    @Override
                    public void invoke(AircraftControlCode message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅飞机风速估计信息
     */
    public long ListenAircraftWindSpeed(
            OnSubscribeMessageCallback<AircraftWindSpeed> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenAircraftWindSpeed(
                new AircraftWindSpeedCallback() {
                    @Override
                    public void invoke(AircraftWindSpeed message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅飞机电池信息
     */
    public long ListenAircraftBatteryInfo(
            OnSubscribeMessageCallback<AircraftBatteryInfo> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenAircraftBatteryInfo(
                new AircraftBatteryInfoCallback() {
                    @Override
                    public void invoke(AircraftBatteryInfo message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅设备OSD信息
     */
    public long ListenDeviceOsd(
            OnSubscribeMessageCallback<DeviceOsd> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenDeviceOsd(
                new DeviceOsdCallback() {
                    @Override
                    public void invoke(DeviceOsd message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅无人机在机场状态
     */
    public long ListenDroneInDock(
            OnSubscribeMessageCallback<DroneInDock> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenDroneInDock(
                new DroneInDockCallback() {
                    @Override
                    public void invoke(DroneInDock message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅设备在线状态
     */
    public long ListenDeviceOnlineStatus(
            OnSubscribeMessageCallback<DeviceOnlineStatus> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenDeviceOnlineStatus(
                new DeviceOnlineStatusCallback() {
                    @Override
                    public void invoke(DeviceOnlineStatus message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    

    

    
    /**
     * 订阅飞机载荷列表信息
     */
    public long ListenAircraftPayloadsList(
            OnSubscribeMessageCallback<AircraftPayloadsList> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenAircraftPayloadsList(
                new AircraftPayloadsListCallback() {
                    @Override
                    public void invoke(AircraftPayloadsList message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 订阅机场位置信息
     */
    public long ListenDockLocation(
            OnSubscribeMessageCallback<DockLocation> onMessagesCallback, 
            OnSubscribeResultCallback onResultCallback,
            String deviceSn, 
            NotificationFrequency freq) {
        
        try {
            BusinessManager businessManager = sdk.getBusinessManager();
            if (businessManager == null) {
                throw new IllegalStateException("BusinessManager not available");
            }
            
            return businessManager.ListenDockLocation(
                new DockLocationCallback() {
                    @Override
                    public void invoke(DockLocation message) {
                        if (onMessagesCallback != null) {
                            onMessagesCallback.invoke(message);
                        }
                    }
                },
                new SDKSubscribeResultCallback() {
                    @Override
                    public void invoke(NotificationCenterErrorCode errorCode) {
                        if (onResultCallback != null) {
                            onResultCallback.invoke(errorCode);
                        }
                    }
                },
                deviceSn,
                freq
            );
        } catch (Exception e) {
            if (onResultCallback != null) {
                onResultCallback.invoke(NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter);
            }
            return -1;
        }
    }
    
    /**
     * 取消监听
     * @param listenId 监听ID
     */
    public void cancelListen(long listenId) {
        sdk.cancelListen(listenId);
    }
} 