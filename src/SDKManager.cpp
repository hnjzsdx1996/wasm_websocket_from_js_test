#include "SDKManager.h"
#include <iostream>
#include "message_define/common.h"
#include "base/logger/logger.h"

SDKManager::SDKManager() {
    nc_logger::init(plog::info, "notification_center_log.log");
    NC_LOG_INFO("SDKManager ctor");
}

SDKManager::~SDKManager() {}

void SDKManager::configure(const std::string& config) {
    NC_LOG_INFO("SDKManager configure");
    NC_LOG_INFO("SDKManager: 配置 %s", config.c_str());
    auto config_ = SDKConfig();
    config_.FromJsonString(config);
    NC_LOG_INFO("sn: %s, ping_pong_interval: %d", config_.sn.c_str(), config_.ping_pong_interval);
}

void SDKManager::connect(const std::string& url) {
    NC_LOG_INFO("SDKManager: 连接 %s", url.c_str());
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->connect(url);
    } else {
        NC_LOG_INFO("SDKManager: 未注入 WebSocket 实现");
    }
}

void SDKManager::send(const std::string& message) {
    NC_LOG_INFO("SDKManager: 发送 %s", message.c_str());
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->send(message);
    } else {
        NC_LOG_INFO("SDKManager: 未注入 WebSocket 实现");
    }
}

void SDKManager::close() {
    NC_LOG_INFO("SDKManager: 关闭连接");
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->close();
    }
}

void SDKManager::setWebSocket(WebSocketBase* ws) {
    wsHolder_.setWebSocket(ws);
}

WebSocketHolder& SDKManager::getWebSocketHolder() {
    return wsHolder_;
}

void SDKManager::setMessageCallback(MessageCallback cb) {
    messageCallback_ = std::move(cb);
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->setOnMessage(messageCallback_);
    }
}

void SDKManager::setOpenCallback(OpenCallback cb) {
    openCallback_ = std::move(cb);
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->setOnOpen(openCallback_);
    }
}
void SDKManager::setCloseCallback(CloseCallback cb) {
    closeCallback_ = std::move(cb);
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->setOnClose(closeCallback_);
    }
}
void SDKManager::setErrorCallback(ErrorCallback cb) {
    errorCallback_ = std::move(cb);
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->setOnError(errorCallback_);
    }
} 