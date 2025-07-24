package com.dji.notificationcentersdk.core.listeners;

import com.dji.notificationcentersdk.generated.AircraftSpeed;
import com.dji.notificationcentersdk.generated.NotificationCenterErrorCode;

/**
 * 飞机速度监听器接口
 * 提供用户友好的回调方法
 */
public interface AircraftSpeedListener {
    
    /**
     * 接收到飞机速度信息时的回调
     * @param deviceSn 设备序列号
     * @param speed 飞机速度信息
     */
    void onAircraftSpeedReceived(String deviceSn, AircraftSpeed speed);
    
    /**
     * 订阅结果回调
     * @param deviceSn 设备序列号
     * @param errorCode 错误码
     */
    void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode);
} 