package com.dji.notificationcentersdk.core.listeners;

import com.dji.notificationcentersdk.generated.AircraftControlCode;
import com.dji.notificationcentersdk.generated.NotificationCenterErrorCode;

/**
 * 飞机控制代码监听器接口
 * 提供用户友好的回调方法
 */
public interface AircraftControlCodeListener {
    
    /**
     * 接收到飞机控制代码时的回调
     * @param deviceSn 设备序列号
     * @param controlCode 飞机控制代码
     */
    void onAircraftControlCodeReceived(String deviceSn, AircraftControlCode controlCode);
    
    /**
     * 订阅结果回调
     * @param deviceSn 设备序列号
     * @param errorCode 错误码
     */
    void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode);
} 