#pragma once

#include <cstdint>
#include <functional>
#include "topic_message_all_define.h"

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

using AircraftAttitudeMsgCallback = std::function<void(const AircraftAttitudeMsg& msg)>;
using AircraftBatteryInfoMsgCallback = std::function<void(const AircraftBatteryInfoMsg& msg)>;
using AircraftControlCodeMsgCallback = std::function<void(const AircraftControlCodeMsg& msg)>;
using AircraftLocationMsgCallback = std::function<void(const AircraftLocationMsg& msg)>;
using AircraftModeCodeMsgCallback = std::function<void(const AircraftModeCodeMsg& msg)>;
using AircraftPayloadsCameraLiveviewWorldRegionMsgCallback = std::function<void(const AircraftPayloadsCameraLiveviewWorldRegionMsg& msg)>;
using AircraftPayloadsGimbalAttitudeMsgCallback = std::function<void(const AircraftPayloadsGimbalAttitudeMsg& msg)>;
using AircraftPayloadsListMsgCallback = std::function<void(const AircraftPayloadsListMsg& msg)>;
using AircraftSpeedMsgCallback = std::function<void(const AircraftSpeedMsg& msg)>;
using AircraftWindSpeedMsgCallback = std::function<void(const AircraftWindSpeedMsg& msg)>;
using DeviceOsdMsgCallback = std::function<void(const DeviceOsdMsg& msg)>;
using DockLocationMsgCallback = std::function<void(const DockLocationMsg& msg)>;
using DroneInDockMsgCallback = std::function<void(const DroneInDockMsg& msg)>;



