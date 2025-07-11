#include "SDKManager.h"
#include <iostream>

SDKManager::SDKManager() {}
SDKManager::~SDKManager() {}

void SDKManager::configure(const std::string& config) {
    std::cout << "SDKManager: 配置 " << config << std::endl;
    // TODO: 解析和应用配置
}

void SDKManager::connect(const std::string& url) {
    std::cout << "SDKManager: 连接 " << url << std::endl;
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->connect(url);
    } else {
        std::cout << "SDKManager: 未注入 WebSocket 实现" << std::endl;
    }
}

void SDKManager::send(const std::string& message) {
    std::cout << "SDKManager: 发送 " << message << std::endl;
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->send(message);
    } else {
        std::cout << "SDKManager: 未注入 WebSocket 实现" << std::endl;
    }
}

void SDKManager::close() {
    std::cout << "SDKManager: 关闭连接" << std::endl;
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