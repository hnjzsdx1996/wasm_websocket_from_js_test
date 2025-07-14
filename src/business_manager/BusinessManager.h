#pragma once
#include <functional>
#include <memory>
#include "../topic_engine/TopicManager.h"
#include "../topic_engine/topic_message_define/DeviceOsdTopic.h"

class BusinessManager {
public:
    explicit BusinessManager(TopicManager* topic_mgr);
    int64_t ObserveDeviceOsd(const std::function<void(const DeviceOsdTopic&)>& cb);
    void CancelObserve(int64_t listen_id);
private:
    TopicManager* topic_mgr_;
};