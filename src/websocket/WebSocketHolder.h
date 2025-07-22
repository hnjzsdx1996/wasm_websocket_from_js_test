#pragma once
#include <functional>
#include <memory>
#include <mutex>
#include <unordered_map>
#include <cstdint>
#include "WebSocketBase.h"

using MessageCallback = std::function<void(const std::string&)>;
using OpenCallback = std::function<void()>;
using CloseCallback = std::function<void()>;
using ErrorCallback = std::function<void(const std::string&)>;

class WebSocketHolder {
public:
    WebSocketHolder() = default;
    ~WebSocketHolder() = default;

    // 支持多个消息回调
    int64_t setOnMessage(MessageCallback cb);
    void cancelOnMessage(int64_t listen_id);
    
    // 单个回调（覆盖模式）
    void setOnOpen(OpenCallback cb);
    void setOnClose(CloseCallback cb);
    void setOnError(ErrorCallback cb);

    void setWebSocket(std::shared_ptr<WebSocketBase> ws);
    void resetWebSocket();
    WebSocketBase* getWebSocket() const;

private:
    std::mutex mtx_;
    std::shared_ptr<WebSocketBase> ws_;
    
    // 支持多个消息回调
    std::unordered_map<int64_t, MessageCallback> message_callbacks_;
    std::atomic<int64_t> next_listen_id_{1};
    
    // 单个回调
    OpenCallback open_callback_;
    CloseCallback close_callback_;
    ErrorCallback error_callback_;
}; 