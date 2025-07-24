package com.dji.notificationcentersdk.core.listeners;

import com.dji.notificationcentersdk.generated.AircraftBatteryInfo;
import com.dji.notificationcentersdk.generated.NotificationCenterErrorCode;

/**
 * 飞机电池信息监听器接口
 * 提供用户友好的回调方法
 */
public interface AircraftBatteryInfoListener {
    
    /**
     * 接收到飞机电池信息时的回调
     * @param deviceSn 设备序列号
     * @param batteryInfo 飞机电池信息
     */
    void onAircraftBatteryInfoReceived(String deviceSn, AircraftBatteryInfo batteryInfo);
    
    /**
     * 订阅结果回调
     * @param deviceSn 设备序列号
     * @param errorCode 错误码
     */
    void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode);
} 