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

using TopicCallback = std::function<void(int err, std::shared_ptr<TopicMessageWrapper>)>;

class TopicManager {
public:
    TopicManager() = default;
    ~TopicManager() = default;

    void setWebSocketHolder(WebSocketHolder* holder) { ws_holder_ = holder; }

    int64_t Observe(const std::string& topic, TopicCallback cb);
    void CancelObserve(int64_t listen_id);
    int SendMessage(std::shared_ptr<TopicMessageWrapper> msg, TopicCallback cb);
    void OnWebSocketMessage(const std::string& json);

private:
    std::atomic<int64_t> next_listen_id_{1};
    std::mutex mtx_;
    std::map<std::string, std::map<int64_t, TopicCallback>> topic_observers_;
    std::map<std::string, TopicCallback> pending_requests_; // uuid -> 回调
    WebSocketHolder* ws_holder_{nullptr};
};