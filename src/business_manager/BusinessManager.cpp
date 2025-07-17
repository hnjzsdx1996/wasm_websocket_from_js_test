#include "BusinessManager.h"

#include <utility>

#include "logger.h"

#include "topic_engine/topic_message_all_define.h"

BusinessManager::BusinessManager(std::weak_ptr<TopicManager> topic_mgr) : topic_mgr_(std::move(topic_mgr)) {
    NC_LOG_INFO("[BusinessManager] ctor");
}

void BusinessManager::Send(const std::string &msg) {
    auto strong_topic_mgr = topic_mgr_.lock();
    if (strong_topic_mgr == nullptr) {
        NC_LOG_INFO("[BusinessManager] Send failed");
        return;
    }
    auto topic_message = std::make_shared<PublishTopicWrapper>();
    topic_message->FromJsonString(msg);
    topic_message->need_replay = true;
    strong_topic_mgr->SendMessage(topic_message, [](int err, std::shared_ptr<TopicMessageWrapper> msg)->void {
        if (err != 0) {
            NC_LOG_INFO("[BusinessManager] Send err: %d", err);
            return;
        }
        NC_LOG_INFO("[BusinessManager] Send success: %s", msg->ToJsonString().c_str());
    });
}

void BusinessManager::Observe(std::function<void(const std::string &)> callback) {
    auto strong_topic_mgr = topic_mgr_.lock();
    if (strong_topic_mgr == nullptr) {
        NC_LOG_INFO("[BusinessManager] Send failed");
        return;
    }
    strong_topic_mgr->ObserveAll([cb = std::move(callback)](int err, std::shared_ptr<PublishTopicWrapper> topic_message)->void {
        if (err != 0) {
            NC_LOG_INFO("[BusinessManager] Observe err: %d");
            return;
        }
        if (cb && topic_message) {
            cb(topic_message->ToJsonString());
        }
    });
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

ListenId BusinessManager::ListenAircraftLocation(const OnSubscribeMessageCallback &on_messages_callback, const OnSubscribeResultCallback &on_result_callback, const std::string &device_sn, NotifactionFrequency freq) {
    auto strong_topic_mgr = topic_mgr_.lock();
    if (strong_topic_mgr == nullptr) {
        NC_LOG_INFO("[BusinessManager] ListenAircraftLocation failed, no topic_mgr_");
        return NotificationCenterErrorCode_NotConnected;
    }

    // 1.先监听上此 topic 消息
    auto subscribe_topic_msg = std::make_shared<SubscribeTopicWrapper>(device_sn, "aircraft_location", freq);
    NC_LOG_INFO("[BusinessManager] ListenAircraftLocation subscribe: %s", subscribe_topic_msg->ToJsonString().c_str());
    WeakDummy(weak_ptr);
    auto listen_id = strong_topic_mgr->Observe(subscribe_topic_msg->items[0].topics[0], [this, weak_ptr, on_msg_cb = on_messages_callback](int err, const std::shared_ptr<PublishTopicWrapper>& message)->void {
        WeakDummyReturn(weak_ptr);
        if (err != TopicManager_NoError) {
            return;
        }
        // todo:根据 sn 过滤消息
        if (on_msg_cb && message) {
            // todo:sdk 转换成对应的数据结构
            auto msg = std::make_shared<PublishAircraftLocationTopic>(message);
            NC_LOG_INFO("[BusinessManager] ListenAircraftLocation observe: %s", message->ToJsonString().c_str());
            on_msg_cb(message->ToJsonString());
        }
    });


    // 2.发起订阅
    // todo:sdk 如果是已经成功订阅的消息，不需要再次订阅
    auto ret = strong_topic_mgr->SendMessage(subscribe_topic_msg, [this, weak_ptr, listen_id, on_result_cb = on_result_callback](int err, const std::shared_ptr<TopicMessageWrapper>& message)->void {
        WeakDummyReturn(weak_ptr);
        if (err == TopicManager_NoError) {
            // 订阅成功
            on_result_cb(NotificationCenterErrorCode_NoError);
            return;
        }
        auto strong_topic_mgr = topic_mgr_.lock();
        if (strong_topic_mgr == nullptr) {
            NC_LOG_INFO("[BusinessManager] ListenAircraftLocation subscribe error, no topic_mgr_");
            return;
        }
        strong_topic_mgr->CancelObserve(listen_id);
        on_result_cb(NotificationCenterErrorCode_SubscribeError);
    });

    if (ret != TopicManager_NoError) {
        return NotificationCenterErrorCode_SendError;
    }
    return listen_id;
}
