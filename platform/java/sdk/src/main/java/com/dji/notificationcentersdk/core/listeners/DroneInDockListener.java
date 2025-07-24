package com.dji.notificationcentersdk.core.listeners;

import com.dji.notificationcentersdk.generated.DroneInDock;
import com.dji.notificationcentersdk.generated.NotificationCenterErrorCode;

/**
 * 无人机在机场状态监听器接口
 * 提供用户友好的回调方法
 */
public interface DroneInDockListener {
    
    /**
     * 接收到无人机在机场状态信息时的回调
     * @param deviceSn 设备序列号
     * @param droneInDock 无人机在机场状态信息
     */
    void onDroneInDockReceived(String deviceSn, DroneInDock droneInDock);
    
    /**
     * 订阅结果回调
     * @param deviceSn 设备序列号
     * @param errorCode 错误码
     */
    void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode);
} 