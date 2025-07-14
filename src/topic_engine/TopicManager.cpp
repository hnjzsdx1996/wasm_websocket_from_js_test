#include "TopicManager.h"
#include "topic_message_define/DeviceOsdTopic.h"
#include <memory>
#include <string>

int64_t TopicManager::Observe(const std::string& topic, TopicCallback cb) {
    std::lock_guard<std::mutex> lock(mtx_);
    int64_t id = next_listen_id_++;
    topic_observers_[topic][id] = cb;
    return id;
}

void TopicManager::CancelObserve(int64_t listen_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& [topic, observers] : topic_observers_) {
        observers.erase(listen_id);
    }
}

int TopicManager::SendMessage(std::shared_ptr<TopicMessageWrapper> msg, TopicCallback cb) {
    if (ws_holder_ && ws_holder_->getWebSocket()) {
        std::lock_guard<std::mutex> lock(mtx_);
        pending_requests_[msg->uuid_] = cb;
        ws_holder_->getWebSocket()->send(msg->ToJsonString());
        return 0;
    }
    return -1;
}

void TopicManager::OnWebSocketMessage(const std::string& json) {
    // 这里只处理device_osd，其他topic可按需扩展
    std::shared_ptr<TopicMessageWrapper> msg;
    DeviceOsdTopic tmp;
    tmp.FromJsonString(json);
    if (tmp.topic_ == "device_osd") {
        msg = std::make_shared<DeviceOsdTopic>(tmp);
    }
    // ... 其他topic类型

    std::lock_guard<std::mutex> lock(mtx_);
    // 1. 响应回调
    if (msg && !msg->uuid_.empty() && pending_requests_.count(msg->uuid_)) {
        auto cb = pending_requests_[msg->uuid_];
        cb(0, msg);
        pending_requests_.erase(msg->uuid_);
        return;
    }
    // 2. 广播给所有监听者
    if (msg && topic_observers_.count(msg->topic_)) {
        for (auto& [id, cb] : topic_observers_[msg->topic_]) {
            cb(0, msg);
        }
    }
} 