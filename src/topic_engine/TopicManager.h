#pragma once
#include <cstdint>
#include <functional>
#include <map>
#include <mutex>
#include <atomic>
#include <string>
#include <memory>
#include "TopicMessageWrapper.h"
#include "websocket/WebSocketHolder.h"
#include "../base/utils/async_capture_protect.h"

using PublishTopicCallback = std::function<void(std::shared_ptr<PublishTopicWrapper>)>;
using SendTopicCallback = std::function<void(int err, std::shared_ptr<TopicMessageWrapper>)>;
using SubscribeResultCallback = std::function<void(int err)>;

enum TopicManagerErrorCode {
    TopicManager_NoError = 0,
    TopicManager_ErrorNoWebsocket = -1,
    TopicManager_ErrorTimeout = -2,
    TopicManager_ErrorServerError = -3,
};

// 订阅消息二元组
// 用 sn 和 topic 可以唯一标识一个订阅
class SubscribeTopicTuple {
public:
    std::string sn;
    std::string topic;

    AIGC_JSON_HELPER(sn, topic);

    bool operator==(const SubscribeTopicTuple& other) const {
        return sn == other.sn && topic == other.topic;
    }
};

template<>
struct std::hash<SubscribeTopicTuple> {
    std::size_t operator()(const SubscribeTopicTuple& k) const noexcept {
        return std::hash<std::string>()(k.sn) ^ (std::hash<std::string>()(k.topic) << 1);
    }
};

// 消息匹配
class TopicManager : public AsyncCaptureProtect {
public:
    TopicManager() = default;
    ~TopicManager() override = default;

    void setWebSocketHolder(const std::weak_ptr<WebSocketHolder>& holder);

    int64_t Observe(const SubscribeTopicTuple& tuple, NotifactionFrequency freq, PublishTopicCallback cb, const SubscribeResultCallback& result_cb = nullptr);
    int64_t ObserveAll(PublishTopicCallback cb);
    void CancelObserve(int64_t listen_id);

private:
    int SendSubscribe(const SubscribeTopicTuple& tuple, NotifactionFrequency freq, int64_t listen_id, const SubscribeResultCallback& result_cb); // 发起订阅请求
    int SendMessage(const std::shared_ptr<TopicMessageWrapper>& msg, SendTopicCallback cb = nullptr);

    void OnWebSocketMessage(const std::string& json);

    void OnSubscribe(const std::string& json);
    void OnUnSubscribe(const std::string& json);
    void OnPublish(const std::string& json);
    void OnPing(const std::string& json);
    void OnPong(const std::string& json);

    // 引用计数，自动取消订阅
    void UnsubscribeUnlistened(); // 取消无人监听的订阅
    void SendUnsubscribe(const SubscribeTopicTuple& topic_tuple); // 取消订阅

    std::atomic<int64_t> next_listen_id_{1};
    std::mutex mtx_;
    std::unordered_map<SubscribeTopicTuple, std::unordered_map<int64_t, PublishTopicCallback>> topic_observers_;
    std::unordered_map<int64_t, PublishTopicCallback> all_topic_observers_;
    std::unordered_map<std::string, SendTopicCallback> pending_requests_; // uuid -> 回调
    std::weak_ptr<WebSocketHolder> ws_holder_;
};