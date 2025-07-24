package com.dji.notificationcentersdk.core.listeners;

import com.dji.notificationcentersdk.generated.AircraftModeCode;
import com.dji.notificationcentersdk.generated.NotificationCenterErrorCode;

/**
 * 飞机模式代码监听器接口
 * 提供用户友好的回调方法
 */
public interface AircraftModeCodeListener {
    
    /**
     * 接收到飞机模式代码时的回调
     * @param deviceSn 设备序列号
     * @param modeCode 飞机模式代码
     */
    void onAircraftModeCodeReceived(String deviceSn, AircraftModeCode modeCode);
    
    /**
     * 订阅结果回调
     * @param deviceSn 设备序列号
     * @param errorCode 错误码
     */
    void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode);
} 