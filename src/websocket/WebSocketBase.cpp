#include "WebSocketBase.h"

#include "logger.h"

void WebSocketBase::setOnMessage(MessageCallback cb) {
    message_callbacks_.emplace_back(std::move(cb));
}

void WebSocketBase::clearMessageCallbacks() {
    std::unique_lock<std::mutex> lock(mtx_);
    message_callbacks_.clear();
}

void WebSocketBase::setOnOpen(OpenCallback cb) {
    open_callback_ = std::move(cb);
}

void WebSocketBase::setOnClose(CloseCallback cb) {
    close_callback_ = std::move(cb);
}

void WebSocketBase::setOnError(ErrorCallback cb) {
    error_callback_ = std::move(cb);
}

void WebSocketBase::callOnMessage(const std::string &message) {
    decltype(message_callbacks_) callback_tmp;
    {
        std::unique_lock<std::mutex> lock(mtx_);
        callback_tmp = message_callbacks_;
    }
    for (const auto& cb : callback_tmp) {
        if (!cb) {
            continue;
        }
        cb(message);
    }
}

void WebSocketBase::callOnOpen() {
    if (open_callback_ == nullptr) {
        NC_LOG_ERROR("[WebSocketBase] callOnOpen but no callback");
        return;
    }
    open_callback_();
}

void WebSocketBase::callOnClose() {
    if (close_callback_ == nullptr) {
        NC_LOG_ERROR("[WebSocketBase] callOnClose but no callback");
        return;
    }
    close_callback_();
}

void WebSocketBase::callOnError(const std::string &error) {
    if (error_callback_) {
        NC_LOG_ERROR("[WebSocketBase] callOnError but no callback");
        return;
    }
    error_callback_(error);
}
