#include "BusinessManager.h"

#include "logger.h"

BusinessManager::BusinessManager(std::weak_ptr<TopicManager> topic_mgr) : topic_mgr_(std::move(topic_mgr)) {
    NC_LOG_INFO("[BusinessManager] ctor");
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
