#include "BusinessManager.h"

BusinessManager::BusinessManager(TopicManager* topic_mgr) : topic_mgr_(topic_mgr) {}

int64_t BusinessManager::ObserveDeviceOsd(const std::function<void(const DeviceOsdTopic&)>& cb) {
    return topic_mgr_->Observe("device_osd", [cb](int err, std::shared_ptr<TopicMessageWrapper> msg) {
        if (err == 0 && msg) {
            auto osd_msg = std::dynamic_pointer_cast<DeviceOsdTopic>(msg);
            if (osd_msg) {
                cb(*osd_msg);
            }
        }
    });
}

void BusinessManager::CancelObserve(int64_t listen_id) {
    topic_mgr_->CancelObserve(listen_id);
} 