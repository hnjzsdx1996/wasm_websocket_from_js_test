#pragma once
#include "WebSocketBase.h"
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <functional>

// 管理一条 websocket 连接
// WebSocketBase 指针可以随意改变，但是 callback 只保存一份就行
class WebSocketHolder {
public:
    using MessageCallback = std::function<void(const std::string&)>;
    using OpenCallback = std::function<void()>;
    using CloseCallback = std::function<void()>;
    using ErrorCallback = std::function<void(const std::string&)>;

    int64_t setOnMessage(MessageCallback cb);
    void setOnOpen(OpenCallback cb);
    void setOnClose(CloseCallback cb);
    void setOnError(ErrorCallback cb);
    void cancelOnMessage(int64_t listen_id);

    // 当 setWebSocket 时，将所有 callback 设置到新的 WebSocketBase
    void setWebSocket(std::shared_ptr<WebSocketBase> ws);

    // todo:sdk 干掉这个函数
    WebSocketBase* getWebSocket() const {
        if (ws_ == nullptr) {
            return nullptr;
        }
        return ws_.get();
    }


private:
    std::atomic<int64_t> next_listen_id_{0};
    std::unordered_map<int64_t, MessageCallback> message_callbacks_;
    OpenCallback open_callback_;
    CloseCallback close_callback_;
    ErrorCallback error_callback_;
    std::shared_ptr<WebSocketBase> ws_{nullptr};
    std::mutex mtx_;
}; 