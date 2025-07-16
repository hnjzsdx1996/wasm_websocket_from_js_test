#include "TopicManager.h"
#include "topic_message_define/DeviceOsdTopic.h"
#include <memory>
#include <string>
#include <utility>

#include "logger.h"
#include "base/utils/json_utils.h"


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
    int64_t id;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        id = next_listen_id_++;
        topic_observers_[topic][id] = std::move(cb);
    }
    // todo:sdk observe 时需要向服务器发起订阅消息
    return id;
}

int64_t TopicManager::ObserveAll(TopicCallback cb) {
    std::lock_guard<std::mutex> lock(mtx_);
    int64_t id = next_listen_id_++;
    all_topic_observers_[id] = std::move(cb);
    return id;
}

void TopicManager::CancelObserve(int64_t listen_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    for (auto& [topic, observers] : topic_observers_) {
        observers.erase(listen_id);
    }
    all_topic_observers_.erase(listen_id);
}

int TopicManager::SendMessage(const std::shared_ptr<TopicMessageWrapper>& msg, TopicCallback cb) {
    auto strong_ws_holder = ws_holder_.lock();
    if (strong_ws_holder == nullptr) {
        return TopicManager_ErrorNoWebsocket;
    }
    if (strong_ws_holder->getWebSocket()) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (cb != nullptr && msg->need_replay == true) {
            // 消息发出后需要回调
            // todo:sdk 消息超时发送需要回调
            pending_requests_[msg->message_id] = std::move(cb);
        }
        strong_ws_holder->getWebSocket()->send(msg->ToJsonString());
        return 0;
    }
    return -1;
}

void TopicManager::OnWebSocketMessage(const std::string& json) {
    if (isValidJson(json) == false) {
        NC_LOG_ERROR("[TopicManager] OnWebSocketMessage, recv invalid json string: %s", json.c_str());
        return;
    }

    // 这里只处理device_osd，其他topic可按需扩展
    auto msg = std::make_shared<TopicMessageWrapper>();
    msg->FromJsonString(json);
    NC_LOG_INFO("[TopicManager] OnWebSocketMessage Wrapper: %s", msg->ToJsonString().c_str());

    if (msg->message_type == "subscribe") {
        OnSubscribe(json);
        return;
    }
    if (msg->message_type == "unsubscribe") {
        OnUnSubscribe(json);
        return;
    }
    if (msg->message_type == "ping") {
        OnPing(json);
        return;
    }
    if (msg->message_type == "publish") {
        OnPublish(json);
        return;
    }
    NC_LOG_ERROR("[TopicManager] OnWebSocketMessage message_type unknown: %s", msg->message_type.c_str());
}

void TopicManager::OnSubscribe(const std::string &json) {
    // todo:sdk
    // 收到 subscribe 消息，是来自服务端对 SDK 发起的 subscribe 的回复
    // 匹配之前发出的消息

}

void TopicManager::OnUnSubscribe(const std::string &json) {
    // todo:sdk
    // 收到 unsubscribe 消息，是来自服务端对 SDK 发起的 unsubscribe 的回复
    // 匹配之前发出的消息
}

void TopicManager::OnPublish(const std::string &json) {
    // todo:sdk 完善收到推送消息的逻辑
    // std::lock_guard<std::mutex> lock(mtx_);
    // // 1. 响应回调
    // if (msg && !msg->message_id.empty() && pending_requests_.count(msg->message_id)) {
    //     auto cb = pending_requests_[msg->message_id];
    //     cb(0, msg);
    //     pending_requests_.erase(msg->message_id);
    //     return;
    // }
    // // 2. 广播给所有监听者
    // if (msg && topic_observers_.count(msg->message_topic)) {
    //     for (auto& [id, cb] : topic_observers_[msg->message_topic]) {
    //         cb(TopicManager_NoError, msg);
    //     }
    // }
}

void TopicManager::OnPing(const std::string &json) {
    // 收到服务端发送的 ping，原样回复一个 pong
    auto ping_msg = std::make_shared<PingTopicWrapper>();
    ping_msg->FromJsonString(json);
    NC_LOG_INFO("[TopicManager] OnPing, uuid: %s, ts: %llu", ping_msg->message_id.c_str(), ping_msg->timestamp);

    auto pong_msg = std::make_shared<PongTopicWrapper>(ping_msg);
    auto ret = SendMessage(pong_msg, nullptr);
    if (ret != TopicManager_NoError) {
        NC_LOG_ERROR("[TopicManager] OnPing, pong error: %d", ret);
    }
}
