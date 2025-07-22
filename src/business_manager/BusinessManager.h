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

private:
    // 通用模板方法，用于处理所有监听接口
    template<typename MsgType, typename MsgCallbackType, typename TopicType>
    ListenId ListenTopic(
        const MsgCallbackType& on_messages_callback, 
        const OnSubscribeResultCallback& on_result_callback, 
        const std::string& device_sn, 
        const std::string& topic_name,
        NotifactionFrequency freq) {
        
        auto strong_topic_mgr = topic_mgr_.lock();
        if (strong_topic_mgr == nullptr) {
            NC_LOG_INFO("[BusinessManager] ListenTopic failed, no topic_mgr_");
            return NotificationCenterErrorCode_NotConnected;
        }

        // 使用新的Observe接口，自动处理订阅逻辑
        const SubscribeTopicTuple tuple{device_sn, topic_name};

        WeakDummy(weak_ptr);
        auto listen_id = strong_topic_mgr->Observe(tuple, freq, [this, weak_ptr, on_msg_cb = on_messages_callback](const std::shared_ptr<PublishTopicWrapper>& message)->void {
            WeakDummyReturn(weak_ptr);
            if (on_msg_cb && message) {
                const auto msg = std::make_shared<TopicType>(message);
                NC_LOG_INFO("[BusinessManager] ListenTopic observe: %s", msg->ToJsonString().c_str());
                on_msg_cb(msg->msg);
            }
        }, [this, weak_ptr, on_result_cb = on_result_callback](int err)->void {
            WeakDummyReturn(weak_ptr);
            on_result_cb(static_cast<NotificationCenterErrorCode>(err));
        });

        return listen_id;
    }

    // 辅助宏，用于简化监听接口的定义
    #define DEFINE_LISTEN_METHOD(MethodName, MsgType, MsgCallbackType, TopicType, TopicName) \
        ListenId MethodName(const MsgCallbackType& on_messages_callback, const OnSubscribeResultCallback& on_result_callback, const std::string& device_sn, NotifactionFrequency freq) { \
            return ListenTopic<MsgType, MsgCallbackType, TopicType>( \
                on_messages_callback, on_result_callback, device_sn, TopicName, freq); \
        }

    std::weak_ptr<TopicManager> topic_mgr_;

public:
    // 具体的监听接口，使用宏定义
    DEFINE_LISTEN_METHOD(ListenAircraftAttitude, AircraftAttitudeMsg, AircraftAttitudeMsgCallback, PublishAircraftAttitudeTopic, "aircraft_attitude")
    DEFINE_LISTEN_METHOD(ListenAircraftLocation, AircraftLocationMsg, AircraftLocationMsgCallback, PublishAircraftLocationTopic, "aircraft_location")
    DEFINE_LISTEN_METHOD(ListenAircraftSpeed, AircraftSpeedMsg, AircraftSpeedMsgCallback, PublishAircraftSpeedTopic, "aircraft_speed")

    #undef DEFINE_LISTEN_METHOD
};