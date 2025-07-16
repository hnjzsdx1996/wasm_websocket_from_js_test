#include "BusinessManager.h"

#include <utility>

#include "logger.h"

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
    strong_topic_mgr->ObserveAll([cb = std::move(callback)](int err, std::shared_ptr<TopicMessageWrapper> topic_message)->void {
        if (err != 0) {
            NC_LOG_INFO("[BusinessManager] Observe err: %d");
            return;
        }
        if (cb && topic_message) {
            cb(topic_message->ToJsonString());
        }
    });
}

int64_t BusinessManager::ObserveDeviceOsd(const std::function<void(const std::shared_ptr<DeviceOsdTopic>&)>& cb) {
    auto strong_topic_mgr = topic_mgr_.lock();
    if (strong_topic_mgr == nullptr) {
        NC_LOG_INFO("[BusinessManager] ObserveDeviceOsd failed");
        return -1;
    }

    return strong_topic_mgr->Observe("device_osd", [cb](int err, const std::shared_ptr<TopicMessageWrapper>& msg) {
        if (err == 0 && msg) {
            auto osd_msg = std::dynamic_pointer_cast<DeviceOsdTopic>(msg);
            if (osd_msg) {
                cb(osd_msg);
            }
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

NotificationCenterErrorCode BusinessManager::ListenAircraftLocation(OnSubscribeMessageCallback on_messages_callback,
    OnSubscribeResultCallback on_result_callback, std::string device_sn, NotifactionFrequency freq) {
    // todo:sdk
    return NotificationCenterErrorCode_NoError;
}
