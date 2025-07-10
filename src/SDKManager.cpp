#include "SDKManager.h"
#include <unordered_map>
#include <emscripten/bind.h>
using namespace emscripten;
#include "JSWebSocket.h"
#include <iostream>
#include <emscripten/val.h>

SDKManager::SDKManager() {}
SDKManager::~SDKManager() { closeWebSocket(); }

void SDKManager::createWebSocket(const std::string& url) {
    ws = std::make_unique<JSWebSocket>(); // todo: 移动到platform相关代码中
    ws->setOnOpen([this]() { onWSOpen(); });
    ws->setOnMessage([this](const std::string& msg) { onWSMessage(msg); });
    ws->setOnClose([this]() { onWSClose(); });
    ws->setOnError([this](const std::string& err) { onWSError(err); });
    ws->connect(url);
}

void SDKManager::sendWebSocketMessage(const std::string& msg) {
    if (ws && wsOpen) ws->send(msg);
}

void SDKManager::closeWebSocket() {
    if (ws) ws->close();
    wsOpen = false;
}

bool SDKManager::isWebSocketOpen() const {
    return wsOpen;
}

std::string SDKManager::getLastMessage() const {
    return lastMessage;
}

void SDKManager::setInfo(const std::string& key, const std::string& value) {
    infoMap[key] = value;
}

std::string SDKManager::getInfo(const std::string& key) const {
    auto it = infoMap.find(key);
    return it != infoMap.end() ? it->second : "";
}

// void SDKManager::setOnMessageCallback(std::function<void(std::string)> cb) {
//     onMessageCallback = std::move(cb);
//     if (ws) ws->setOnMessageCallback(onMessageCallback);
// }

void SDKManager::setOnMessageCallbackJS(emscripten::val cb) {
    onMessageCallback = [cb](std::string msg) {
        cb(msg);
    };
    if (ws) ws->setOnMessageCallback(onMessageCallback);
}

// WebSocket回调
void SDKManager::onWSOpen() {
    wsOpen = true;
    std::cout << "WebSocket opened" << std::endl;
}
void SDKManager::onWSMessage(const std::string& msg) {
    lastMessage = msg;
    std::cout << "WebSocket message: " << msg << std::endl;
    if (onMessageCallback) onMessageCallback(msg);
}
void SDKManager::onWSClose() {
    wsOpen = false;
    std::cout << "WebSocket closed" << std::endl;
}
void SDKManager::onWSError(const std::string& err) {
    std::cout << "WebSocket error: " << err << std::endl;
}

// embind导出
EMSCRIPTEN_BINDINGS(sdk_manager_bindings) {
    emscripten::function("dummyCallbackType", emscripten::optional_override([](const std::function<void(std::string)>&){}));
    emscripten::class_<SDKManager>("SDKManager")
        .constructor<>()
        .function("createWebSocket", &SDKManager::createWebSocket)
        .function("sendWebSocketMessage", &SDKManager::sendWebSocketMessage)
        .function("closeWebSocket", &SDKManager::closeWebSocket)
        .function("isWebSocketOpen", &SDKManager::isWebSocketOpen)
        .function("getLastMessage", &SDKManager::getLastMessage)
        .function("setInfo", &SDKManager::setInfo)
        .function("getInfo", &SDKManager::getInfo)
        .function("setOnMessageCallback", &SDKManager::setOnMessageCallbackJS);
} 