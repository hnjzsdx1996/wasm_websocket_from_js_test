#pragma once

using ListenId = int64_t;

// 客户端错误码
enum NotificationCenterErrorCode : int64_t {
    // 大于0，就是无错误，为ListenID
    NotificationCenterErrorCode_NoError = 0,
    NotificationCenterErrorCode_InvalidParameter = -1, // 参数错误
    NotificationCenterErrorCode_NotConnected = -2, // 链路未建立
    NotificationCenterErrorCode_SendError = -3, // TopicManager 错误
    NotificationCenterErrorCode_SubscribeError = -4, // 向服务器订阅失败

};

// 服务端错误码
enum NotificationServerErrorCode {
    NotificationServerErrorCode_NoError = 0,
    NotificationServerErrorCode_ErrorNotSupported = -1, // 服务端不支持此 topic
};

using OnSubscribeMessageCallback = std::function<void(const std::string& message)>;
using OnSubscribeResultCallback = std::function<void(const NotificationCenterErrorCode& error_code)>;



