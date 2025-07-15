#pragma once

using ListenId = int64_t;

// 客户端错误码
enum NotificationCenterErrorCode : int64_t {
    // 大于0，就是无错误，为ListenID
    NotificationCenterErrorCode_NoError = 0,
    NotificationCenterErrorCode_InvalidParameter = -1, // 参数错误

};

// 服务端错误码
enum NotificationServerErrorCode {
    NotificationServerErrorCode_NoError = 0,
    NotificationServerErrorCode_ErrorNotSupported = -1, // 服务端不支持此 topic
};

enum NotifactionFrequency {
    NotifactionFrequency_Any = 0, // 服务端自定决定推送频率
    NotifactionFrequency_OnChanged = 1, // 按变推送
    NotifactionFrequency_Push_1s = 1000, // 定频 1s 推送 1 次
    NotifactionFrequency_Push_2s = 2000, // 定频 2s 推送 1 次
    NotifactionFrequency_Push_3s = 3000, // 定频 3s 推送 1 次
    NotifactionFrequency_Push_4s = 4000, // 定频 4s 推送 1 次
    NotifactionFrequency_Push_5s = 5000, // 定频 5s 推送 1 次
    NotifactionFrequency_Push_10s = 10000, // 定频 10s 推送 1 次
    NotifactionFrequency_Push_20s = 20000, // 定频 20s 推送 1 次
    NotifactionFrequency_Push_30s = 30000, // 定频 30s 推送 1 次
};

using OnSubscribeMessageCallback = std::function<void(const std::string& message)>;
using OnSubscribeResultCallback = std::function<void(const NotificationCenterErrorCode& error_code)>;



