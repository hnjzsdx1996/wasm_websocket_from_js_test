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

using TopicCallback = std::function<void(int err, std::shared_ptr<TopicMessageWrapper>)>;

enum TopicManagerErrorCode {
    TopicManager_NoError = 0,
    TopicManager_ErrorNoWebsocket = -1,
    TopicManager_ErrorTimeout = -2,
};

// 消息匹配
class TopicManager : public AsyncCaptureProtect {
public:
    TopicManager() = default;
    ~TopicManager() override = default;

    void setWebSocketHolder(const std::weak_ptr<WebSocketHolder>& holder);

    int64_t Observe(const std::string& topic, TopicCallback cb);
    int64_t ObserveAll(TopicCallback cb);
    void CancelObserve(int64_t listen_id);
    int SendMessage(const std::shared_ptr<TopicMessageWrapper>& msg, TopicCallback cb = nullptr);

private:
    void OnWebSocketMessage(const std::string& json);

    void OnSubscribe(const std::string& json);
    void OnUnSubscribe(const std::string& json);
    void OnPublish(const std::string& json);
    void OnPing(const std::string& json);
    void OnPong(const std::string& json);

    std::atomic<int64_t> next_listen_id_{1};
    std::mutex mtx_;
    std::unordered_map<std::string, std::unordered_map<int64_t, TopicCallback>> topic_observers_;
    std::unordered_map<int64_t, TopicCallback> all_topic_observers_;
    std::unordered_map<std::string, TopicCallback> pending_requests_; // uuid -> 回调
    std::weak_ptr<WebSocketHolder> ws_holder_;
};