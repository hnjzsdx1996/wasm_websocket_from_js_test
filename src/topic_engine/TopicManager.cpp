#include "TopicManager.h"
#include <memory>
#include <string>
#include <utility>

#include "logger.h"
#include "base/async/thread_pool_executor.h"
#include "base/utils/json_utils.h"
#include "../business_manager/BusinessManagerDefine.h"
#include "../business_manager/topic_message_define/PublishAircraftLocationTopic.h"


constexpr auto kWebsocketTimeout = 3000; // websocket 消息超时时间 ms

void TopicManager::setWebSocketHolder(const std::weak_ptr<WebSocketHolder>& holder) {
    ws_holder_ = holder;
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

int64_t TopicManager::Observe(const SubscribeTopicTuple& tuple, NotifactionFrequency freq, PublishTopicCallback cb, SubscribeResultCallback result_cb) {
    int64_t id;
    bool need_subscribe = false;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        id = next_listen_id_++;
        need_subscribe = (topic_observers_[tuple].empty()); // 检查是否已经订阅过这个消息（在添加新回调之前检查）
        topic_observers_[tuple][id] = std::move(cb);
    }
    
    // 如果需要订阅，发起订阅请求
    if (need_subscribe) {
        auto ret = SendSubscribe(tuple, freq, id, result_cb);
        if (ret != TopicManager_NoError) {
            return -1; // 返回错误ID
        }
    } else if (result_cb) {
        // 已经订阅过，直接返回成功
        result_cb(NotificationCenterErrorCode_NoError);
    }
    
    return id;
}

int64_t TopicManager::ObserveAll(PublishTopicCallback cb) {
    std::lock_guard<std::mutex> lock(mtx_);
    int64_t id = next_listen_id_++;
    all_topic_observers_[id] = std::move(cb);
    return id;
}

void TopicManager::CancelObserve(int64_t listen_id) {
    {
        std::lock_guard<std::mutex> lock(mtx_);
        for (auto& [tuple, observers] : topic_observers_) {
            observers.erase(listen_id);
        }
        all_topic_observers_.erase(listen_id);
    }
    // 如果没有人监听这个 sn+topic 了，取消监听
    UnsubscribeUnlistened();
}

int TopicManager::SendMessage(const std::shared_ptr<TopicMessageWrapper>& msg, SendTopicCallback cb) {
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
                }
            }, std::chrono::milliseconds(kWebsocketTimeout));
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
    // todo:sdk 判断订阅是否成功，现在是只要服务端返回数据就说明订阅成功
    // subscribe 是服务端回复的，没有人监听，只会先发起，然后等待回复
    // 1. 响应回调
    if (pending_requests_.count(subscribe_msg->message_id)) {
        SendTopicCallback cb;
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
        SendTopicCallback cb;
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

    NC_LOG_INFO("[TopicManager] OnPublish publish_msg: %s", json.c_str());

    if (publish_msg->isValid() == false) {
        // 收到非法 publish 消息
        NC_LOG_ERROR("[TopicManager] OnPublish unknown: %s", json.c_str());
        return;
    }

    // 构造订阅二元组key
    SubscribeTopicTuple tuple;
    tuple.sn = publish_msg->device_sn;
    tuple.topic = publish_msg->message_topic;

    // 2. 广播给所有监听者
    if (topic_observers_.count(tuple)) {
        std::unordered_map<int64_t, PublishTopicCallback> cbs;
        {
            std::lock_guard<std::mutex> lock(mtx_);
            cbs = topic_observers_[tuple];
        }
        for (auto& [_, cb] : cbs) {
            cb(publish_msg);
        }
    }

    // 全量消息的监听者
    std::unordered_map<int64_t, PublishTopicCallback> cbs;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        cbs = all_topic_observers_;
    }
    for (auto& [_, cb] : cbs) {
        cb(publish_msg);
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

void TopicManager::UnsubscribeUnlistened() {
    std::vector<SubscribeTopicTuple> need_unsubscribe;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        for (const auto& [topic_tuple, listeners]: topic_observers_) {
            if (listeners.empty()) {
                NC_LOG_INFO("[TopicManager] UnsubscribeUnlistened, nobody listen sn: %s, topic： %s", topic_tuple.sn.c_str(), topic_tuple.topic.c_str());
                need_unsubscribe.push_back(topic_tuple);
            }
        }
        for (const auto& topic_tuple : need_unsubscribe) {
            topic_observers_.erase(topic_tuple);
        }
    }
    // 发起取消订阅
    for (const auto& topic_tuple: need_unsubscribe) {
        SendUnsubscribe(topic_tuple);
    }
}

int TopicManager::SendSubscribe(const SubscribeTopicTuple& tuple, NotifactionFrequency freq, int64_t listen_id, const SubscribeResultCallback& result_cb) {
    NC_LOG_INFO("[TopicManager] SendSubscribe, sn: %s, topic: %s, freq: %d", tuple.sn.c_str(), tuple.topic.c_str(), freq);

    auto subscribe_msg = std::make_shared<SubscribeTopicWrapper>(tuple.sn, tuple.topic, freq);
    
    WeakDummy(weak_ptr);
    auto ret = SendMessage(subscribe_msg, [this, weak_ptr, tuple, listen_id, result_cb](int err, const std::shared_ptr<TopicMessageWrapper>& message)->void {
        WeakDummyReturn(weak_ptr);
        if (err == TopicManager_NoError) {
            // 订阅成功
            if (result_cb) {
                result_cb(NotificationCenterErrorCode_NoError);
            }
            return;
        }
        
        // 订阅失败，清理监听回调
        {
            std::lock_guard<std::mutex> lock(mtx_);
            auto it = topic_observers_.find(tuple);
            if (it != topic_observers_.end()) {
                it->second.erase(listen_id);
                if (it->second.empty()) {
                    topic_observers_.erase(it);
                }
            }
        }
        
        if (result_cb) {
            result_cb(NotificationCenterErrorCode_SubscribeError);
        }
    });
    
    if (ret != TopicManager_NoError) {
        // 发送失败，清理监听回调
        {
            std::lock_guard<std::mutex> lock(mtx_);
            auto it = topic_observers_.find(tuple);
            if (it != topic_observers_.end()) {
                it->second.erase(listen_id);
                if (it->second.empty()) {
                    topic_observers_.erase(it);
                }
            }
        }
        
        if (result_cb) {
            result_cb(NotificationCenterErrorCode_SendError);
        }
    }
    
    return ret;
}

void TopicManager::SendUnsubscribe(const SubscribeTopicTuple &topic_tuple) {
    auto unsubscribe_msg = std::make_shared<UnSubscribeTopicWrapper>(topic_tuple.sn, topic_tuple.topic);
    WeakDummy(weak_ptr);
    auto ret = SendMessage(unsubscribe_msg, [this, weak_ptr, topic_tuple](int err, const std::shared_ptr<TopicMessageWrapper>& unsubscribe_reply)->void {
        WeakDummyReturn(weak_ptr);
        if (err != TopicManager_NoError) {
            // 取消订阅失败就不管了，就算收到了推送数据，由于 callback 已经删除，也没有什么影响
            NC_LOG_ERROR("[TopicManager] SendUnsubscribe [%s - %s] reply error: %d", topic_tuple.sn.c_str(), topic_tuple.topic.c_str(), err);
            return;
        }
        NC_LOG_INFO("[TopicManager] SendUnsubscribe [%s - %s] success", topic_tuple.sn.c_str(), topic_tuple.topic.c_str());
    });
    if (ret != TopicManager_NoError) {
        NC_LOG_ERROR("[TopicManager] SendUnsubscribe [%s - %s] error: %d", topic_tuple.sn.c_str(), topic_tuple.topic.c_str(), ret);
    }
}
