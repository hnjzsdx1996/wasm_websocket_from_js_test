package com.dji.notificationcentersdk.core.listeners;

import com.dji.notificationcentersdk.generated.AircraftAttitude;
import com.dji.notificationcentersdk.generated.NotificationCenterErrorCode;

/**
 * 飞机姿态监听器接口
 * 提供用户友好的回调方法
 */
public interface AircraftAttitudeListener {
    
    /**
     * 接收到飞机姿态信息时的回调
     * @param deviceSn 设备序列号
     * @param attitude 飞机姿态信息
     */
    void onAircraftAttitudeReceived(String deviceSn, AircraftAttitude attitude);
    
    /**
     * 订阅结果回调
     * @param deviceSn 设备序列号
     * @param errorCode 错误码
     */
    void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode);
} 