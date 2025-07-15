#include "TopicManager.h"
#include "topic_message_define/DeviceOsdTopic.h"
#include <memory>
#include <string>
#include <utility>

#include "logger.h"


void TopicManager::setWebSocketHolder(const std::weak_ptr<WebSocketHolder> &holder) {
    ws_holder_ = holder;
    // 注册消息回调监听
    auto strong_ws_holder = ws_holder_.lock();
    if (strong_ws_holder == nullptr) {
        NC_LOG_INFO("[TopicManager] setWebSocketHolder failed");
        return;
    }
    WeakDummy(weak_ptr);
    strong_ws_holder->setOnMessage([weak_ptr, this](const std::string& message)->void {
        WeakDummyReturn(weak_ptr);
        OnWebSocketMessage(message);
    });
}

int64_t TopicManager::Observe(const std::string& topic, TopicCallback cb) {
    std::lock_guard<std::mutex> lock(mtx_);
    int64_t id = next_listen_id_++;
    topic_observers_[topic][id] = std::move(cb);
    return id;
}

void TopicManager::CancelObserve(int64_t listen_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& [topic, observers] : topic_observers_) {
        observers.erase(listen_id);
    }
}

int TopicManager::SendMessage(const std::shared_ptr<TopicMessageWrapper>& msg, TopicCallback cb) {
    auto strong_ws_holder = ws_holder_.lock();
    if (strong_ws_holder == nullptr) {
        return TopicManager_ErrorNoWebsocket;
    }
    if (strong_ws_holder->getWebSocket()) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (cb != nullptr) {
            // 消息发出后需要回调
            // todo:sdk 消息超时发送需要回调
        }
        pending_requests_[msg->message_id] = std::move(cb);
        strong_ws_holder->getWebSocket()->send(msg->ToJsonString());
        return 0;
    }
    return -1;
}

void TopicManager::OnWebSocketMessage(const std::string& json) {
    // 这里只处理device_osd，其他topic可按需扩展
    std::shared_ptr<TopicMessageWrapper> msg;
    DeviceOsdTopic tmp;
    tmp.FromJsonString(json);
    // todo:sdk 更优雅的处理 topic 和 Message 的映射
    if (tmp.message_topic == "device_osd") {
        msg = std::make_shared<DeviceOsdTopic>(tmp);
    }
    // ... 其他topic类型

    std::lock_guard<std::mutex> lock(mtx_);
    // 1. 响应回调
    if (msg && !msg->message_id.empty() && pending_requests_.count(msg->message_id)) {
        auto cb = pending_requests_[msg->message_id];
        cb(0, msg);
        pending_requests_.erase(msg->message_id);
        return;
    }
    // 2. 广播给所有监听者
    if (msg && topic_observers_.count(msg->message_topic)) {
        for (auto& [id, cb] : topic_observers_[msg->message_topic]) {
            cb(TopicManager_NoError, msg);
        }
    }
} 