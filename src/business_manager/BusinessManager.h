#pragma once
#include <functional>
#include <memory>
#include "BusinessManagerDefine.h"
#include "../topic_engine/TopicManager.h"

// 业务逻辑：消息订阅/取消订阅，处理业务逻辑，提供接口
class BusinessManager : public AsyncCaptureProtect{
public:
    explicit BusinessManager(std::weak_ptr<TopicManager> topic_mgr);

    void CancelObserve(int64_t listen_id);

    ListenId ListenAircraftAttitude(const AircraftAttitudeMsgCallback& on_messages_callback, const OnSubscribeResultCallback& on_result_callback, const std::string& device_sn, NotifactionFrequency freq);
    ListenId ListenAircraftLocation(const AircraftLocationMsgCallback& on_messages_callback, const OnSubscribeResultCallback& on_result_callback, const std::string& device_sn, NotifactionFrequency freq);
    ListenId ListenAircraftSpeed(const AircraftSpeedMsgCallback& on_messages_callback, const OnSubscribeResultCallback& on_result_callback, const std::string& device_sn, NotifactionFrequency freq);

private:
    std::weak_ptr<TopicManager> topic_mgr_;
};