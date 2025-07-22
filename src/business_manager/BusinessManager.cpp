#include "BusinessManager.h"

#include <utility>

#include "logger.h"

#include "topic_message_all_define.h"

BusinessManager::BusinessManager(std::weak_ptr<TopicManager> topic_mgr) : topic_mgr_(std::move(topic_mgr)) {
    NC_LOG_INFO("[BusinessManager] ctor");
}

void BusinessManager::CancelObserve(int64_t listen_id) {
    auto strong_topic_mgr = topic_mgr_.lock();
    if (strong_topic_mgr == nullptr) {
        NC_LOG_INFO("[BusinessManager] CancelObserve: %d, failed", listen_id);
        return;
    }
    NC_LOG_INFO("[BusinessManager] CancelObserve: %d", listen_id);
    strong_topic_mgr->CancelObserve(listen_id);
}

ListenId BusinessManager::ListenAircraftLocation(const AircraftLocationMsgCallback &on_messages_callback, const OnSubscribeResultCallback &on_result_callback, const std::string &device_sn, NotifactionFrequency freq) {
    auto strong_topic_mgr = topic_mgr_.lock();
    if (strong_topic_mgr == nullptr) {
        NC_LOG_INFO("[BusinessManager] ListenAircraftLocation failed, no topic_mgr_");
        return NotificationCenterErrorCode_NotConnected;
    }

    // 使用新的Observe接口，自动处理订阅逻辑
    const SubscribeTopicTuple tuple{device_sn, "aircraft_location"};
    
    WeakDummy(weak_ptr);
    auto listen_id = strong_topic_mgr->Observe(tuple, freq, [this, weak_ptr, on_msg_cb = on_messages_callback](const std::shared_ptr<PublishTopicWrapper>& message)->void {
        WeakDummyReturn(weak_ptr);
        if (on_msg_cb && message) {
            const auto msg = std::make_shared<PublishAircraftLocationTopic>(message);
            NC_LOG_INFO("[BusinessManager] ListenAircraftLocation observe: %s", msg->ToJsonString().c_str());
            on_msg_cb(msg->msg);
        }
    }, [this, weak_ptr, on_result_cb = on_result_callback](int err)->void {
        WeakDummyReturn(weak_ptr);
        on_result_cb(static_cast<NotificationCenterErrorCode>(err));
    });

    return listen_id;
}
