package com.dji.notificationcentersdk.core.listeners;

import com.dji.notificationcentersdk.generated.AircraftLocation;
import com.dji.notificationcentersdk.generated.NotificationCenterErrorCode;

/**
 * 飞机位置监听器接口
 * 提供用户友好的回调方法
 */
public interface AircraftLocationListener {
    
    /**
     * 接收到飞机位置信息时的回调
     * @param deviceSn 设备序列号
     * @param location 飞机位置信息
     */
    void onAircraftLocationReceived(String deviceSn, AircraftLocation location);
    
    /**
     * 订阅结果回调
     * @param deviceSn 设备序列号
     * @param errorCode 错误码
     */
    void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode);
} 