#include "WebSocketHolder.h"

#include <utility>

int64_t WebSocketHolder::setOnMessage(MessageCallback cb) {
    std::lock_guard<std::mutex> lock(mtx_);
    int64_t id = next_listen_id_++;
    message_callbacks_[id] = std::move(cb);
    if (ws_) ws_->setOnMessage(cb); // 设置到当前 ws
    return id;
}

void WebSocketHolder::setOnOpen(OpenCallback cb) {
    std::lock_guard<std::mutex> lock(mtx_);
    open_callback_ = std::move(cb);
    if (ws_) ws_->setOnOpen(open_callback_);
}

void WebSocketHolder::setOnClose(CloseCallback cb) {
    std::lock_guard<std::mutex> lock(mtx_);
    close_callback_ = std::move(cb);
    if (ws_) ws_->setOnClose(close_callback_);
}

void WebSocketHolder::setOnError(ErrorCallback cb) {
    std::lock_guard<std::mutex> lock(mtx_);
    error_callback_ = std::move(cb);
    if (ws_) ws_->setOnError(error_callback_);
}

void WebSocketHolder::cancelOnMessage(int64_t listen_id) {
    std::lock_guard<std::mutex> lock(mtx_);
    message_callbacks_.erase(listen_id);
    // 这里可以选择同步到 ws_，如有需要
}

void WebSocketHolder::setWebSocket(std::shared_ptr<WebSocketBase> ws) {
    std::lock_guard<std::mutex> lock(mtx_);
    ws_ = std::move(ws);
    if (!ws_) return;
    // 重新设置所有 callback 到新的 ws
    for (const auto& kv : message_callbacks_) {
        ws_->setOnMessage(kv.second);
    }
    ws_->setOnOpen(open_callback_);
    ws_->setOnClose(close_callback_);
    ws_->setOnError(error_callback_);
} 