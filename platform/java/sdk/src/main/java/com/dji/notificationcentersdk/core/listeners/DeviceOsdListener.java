package com.dji.notificationcentersdk.core.listeners;

import com.dji.notificationcentersdk.generated.DeviceOsd;
import com.dji.notificationcentersdk.generated.NotificationCenterErrorCode;

/**
 * 设备OSD监听器接口
 * 提供用户友好的回调方法
 */
public interface DeviceOsdListener {
    
    /**
     * 接收到设备OSD信息时的回调
     * @param deviceSn 设备序列号
     * @param deviceOsd 设备OSD信息
     */
    void onDeviceOsdReceived(String deviceSn, DeviceOsd deviceOsd);
    
    /**
     * 订阅结果回调
     * @param deviceSn 设备序列号
     * @param errorCode 错误码
     */
    void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode);
} 