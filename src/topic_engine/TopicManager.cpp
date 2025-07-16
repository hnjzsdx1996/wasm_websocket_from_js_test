#include "TopicManager.h"
#include "topic_message_define/DeviceOsdTopic.h"
#include <memory>
#include <string>
#include <utility>

#include "logger.h"
#include "base/async/thread_pool_executor.h"
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
    NC_LOG_INFO("[TopicManager] SendMessage: %s", msg->ToJsonString().c_str());
    auto strong_ws_holder = ws_holder_.lock();
    if (strong_ws_holder == nullptr) {
        return TopicManager_ErrorNoWebsocket;
    }
    if (strong_ws_holder->getWebSocket()) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (cb != nullptr && msg->need_replay == true) {
            // 消息发出后需要回调
            pending_requests_[msg->message_id] = std::move(cb);
            WeakDummy(weak_ptr);
            ThreadPoolExecutor::Worker()->PostDelayed([this, weak_ptr, msg_uuid = msg->message_id]()->void {
                WeakDummyReturn(weak_ptr);
                if (pending_requests_.count(msg_uuid)) {
                    // 消息还在队列，说明没有收到回复
                    auto callback = pending_requests_[msg_uuid];
                    pending_requests_.erase(msg_uuid);
                    ThreadPoolExecutor::Main().post([this, weak_ptr, cb = callback]()->void {
                        cb(TopicManager_ErrorTimeout, nullptr);
                    });
                } else {
                    NC_LOG_INFO("[TopicManager] SendMessage, 收到回调");
                }
            }, std::chrono::milliseconds(3000));
        }
        strong_ws_holder->getWebSocket()->send(msg->ToJsonString());
        return TopicManager_NoError;
    }
    return TopicManager_ErrorNoWebsocket;
}

void TopicManager::OnWebSocketMessage(const std::string& json) {
    if (isValidJson(json) == false) {
        NC_LOG_ERROR("[TopicManager] OnWebSocketMessage, recv invalid json string: %s", json.c_str());
        return;
    }

    // 这里只处理device_osd，其他topic可按需扩展
    auto msg = std::make_shared<TopicMessageWrapper>();
    msg->FromJsonString(json);
    // NC_LOG_INFO("[TopicManager] OnWebSocketMessage raw: %s", json.c_str());
    // NC_LOG_INFO("[TopicManager] OnWebSocketMessage Wrapper: %s", msg->ToJsonString().c_str());

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
    if (msg->message_type == "pong") {
        OnPong(json);
        return;
    }
    if (msg->message_type == "publish") {
        OnPublish(json);
        return;
    }
    NC_LOG_ERROR("[TopicManager] OnWebSocketMessage message_type unknown: %s", msg->message_type.c_str());
}

void TopicManager::OnSubscribe(const std::string &json) {
    // 收到 subscribe 消息，是来自服务端对 SDK 发起的 subscribe 的回复
    // 匹配之前发出的消息
    auto subscribe_msg = std::make_shared<TopicMessageWrapper>();
    subscribe_msg->FromJsonString(json);

    NC_LOG_ERROR("[TopicManager] OnSubscribe subscribe_msg: %s", subscribe_msg->ToJsonString().c_str());

    if (subscribe_msg->isValid() == false) {
        NC_LOG_ERROR("[TopicManager] OnSubscribe unknown: %s", json.c_str());
        return;
    }
    // subscribe 是服务端回复的，没有人监听，只会先发起，然后等待回复
    // 1. 响应回调
    if (pending_requests_.count(subscribe_msg->message_id)) {
        TopicCallback cb;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            cb = pending_requests_[subscribe_msg->message_id];
            pending_requests_.erase(subscribe_msg->message_id);
        }
        cb(TopicManager_NoError, subscribe_msg);
        return;
    }

}

void TopicManager::OnUnSubscribe(const std::string &json) {
    // 收到 unsubscribe 消息，是来自服务端对 SDK 发起的 unsubscribe 的回复
    // 匹配之前发出的消息
    auto unsubscribe_msg = std::make_shared<TopicMessageWrapper>();
    unsubscribe_msg->FromJsonString(json);

    NC_LOG_ERROR("[TopicManager] OnUnSubscribe unsubscribe_msg: %s", unsubscribe_msg->ToJsonString().c_str());

    if (unsubscribe_msg->isValid() == false) {
        NC_LOG_ERROR("[TopicManager] OnUnSubscribe unknown: %s", json.c_str());
        return;
    }
    // unsubscribe 是服务端回复的，没有人监听，只会先发起，然后等待回复
    // 1. 响应回调
    if (pending_requests_.count(unsubscribe_msg->message_id)) {
        TopicCallback cb;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            cb = pending_requests_[unsubscribe_msg->message_id];
            pending_requests_.erase(unsubscribe_msg->message_id);
        }
        cb(TopicManager_NoError, unsubscribe_msg);
        return;
    }
}

void TopicManager::OnPublish(const std::string &json) {
    auto publish_msg = std::make_shared<PublishTopicWrapper>();
    publish_msg->FromJsonString(json);

    NC_LOG_ERROR("[TopicManager] OnPublish publish_msg: %s", publish_msg->ToJsonString().c_str());

    if (publish_msg->isValid() == false) {
        NC_LOG_ERROR("[TopicManager] OnPublish unknown: %s", json.c_str());
        return;
    }

    // publish 是服务端主动推送的，没有人等待回复, 只需要回调给监听者
    // 2. 广播给所有监听者
    if (topic_observers_.count(publish_msg->message_topic)) {
        std::unordered_map<int64_t, TopicCallback> cbs;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            cbs = topic_observers_[publish_msg->message_topic];
        }
        for (auto& [_, cb] : cbs) {
            cb(TopicManager_NoError, publish_msg);
        }
    }

    // 全量消息的监听者
    std::unordered_map<int64_t, TopicCallback> cbs;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        cbs = all_topic_observers_;
    }
    for (auto& [_, cb] : cbs) {
        cb(TopicManager_NoError, publish_msg);
    }

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

void TopicManager::OnPong(const std::string &json) {
    auto pong_msg = std::make_shared<PongTopicWrapper>(json);
    pong_msg->FromJsonString(json);

    auto time_now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    auto time_diff = time_now - pong_msg->timestamp;

    NC_LOG_INFO("[TopicManager] OnPong, uuid: %s, rtt: %llu(ms)", pong_msg->message_id.c_str(), time_diff);
}
