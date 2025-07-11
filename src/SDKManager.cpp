#include "SDKManager.h"
#include "CppWebSocket.h"
#include "JSWebSocket.h"
#include <iostream>

SDKManager::SDKManager(WebSocketType wsType) 
    : currentWsType(wsType) {
    createWebSocketInstance();
}

SDKManager::~SDKManager() {
    if (ws) {
        ws->close();
    }
}

void SDKManager::setWebSocketType(WebSocketType wsType) {
    if (currentWsType != wsType) {
        currentWsType = wsType;
        if (ws) {
            ws->close();
        }
        createWebSocketInstance();
    }
}

void SDKManager::setJSWebSocketPtr(uintptr_t jsWebSocketPtr) {
    this->jsWebSocketPtr = reinterpret_cast<void*>(jsWebSocketPtr);
    if (ws && currentWsType == WebSocketType::JS) {
        auto jsWs = dynamic_cast<JSWebSocket*>(ws.get());
        if (jsWs) {
            jsWs->setJSWebSocketPtr(reinterpret_cast<void*>(jsWebSocketPtr));
        }
    }
}

void SDKManager::createWebSocketInstance() {
    ws.reset();
    
    switch (currentWsType) {
        case WebSocketType::CPP:
            ws = std::make_unique<CppWebSocket>();
            std::cout << "SDKManager: 使用C++原生WebSocket" << std::endl;
            break;
        case WebSocketType::JS:
            ws = std::make_unique<JSWebSocket>();
            if (jsWebSocketPtr) {
                auto jsWs = dynamic_cast<JSWebSocket*>(ws.get());
                if (jsWs) {
                    jsWs->setJSWebSocketPtr(jsWebSocketPtr);
                }
            }
            std::cout << "SDKManager: 使用JavaScript WebSocket" << std::endl;
            break;
    }
    
    if (ws) {
        ws->setOnOpen([this]() { onWSOpen(); });
        ws->setOnMessage([this](const std::string& msg) { onWSMessage(msg); });
        ws->setOnClose([this]() { onWSClose(); });
        ws->setOnError([this](const std::string& err) { onWSError(err); });
    }
}

void SDKManager::createWebSocket(const std::string& url) {
    if (!ws) {
        std::cout << "SDKManager: WebSocket未初始化" << std::endl;
        return;
    }
    
    std::cout << "SDKManager: 创建WebSocket连接 " << url << std::endl;
    ws->connect(url);
}

void SDKManager::sendWebSocketMessage(const std::string& msg) {
    if (!ws) {
        std::cout << "SDKManager: WebSocket未初始化" << std::endl;
        return;
    }
    
    if (!ws->isOpen()) {
        std::cout << "SDKManager: WebSocket未连接" << std::endl;
        return;
    }
    
    std::cout << "SDKManager: 发送消息 " << msg << std::endl;
    ws->send(msg);
}

void SDKManager::closeWebSocket() {
    if (ws) {
        ws->close();
    }
}

bool SDKManager::isWebSocketOpen() const {
    return ws && ws->isOpen();
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

void SDKManager::onWSOpen() {
    std::cout << "SDKManager: WebSocket连接已打开" << std::endl;
}

#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
void SDKManager::setOnMessageCallback(emscripten::val cb) {
    onMessageCallback = [cb](const std::string& msg) {
        cb(msg);
    };
    if (ws) ws->setOnMessage([this](const std::string& msg) { onWSMessage(msg); });
}
#endif

void SDKManager::onWSMessage(const std::string& msg) {
    std::cout << "SDKManager: 收到消息 " << msg << std::endl;
    lastMessage = msg;
#ifdef __EMSCRIPTEN__
    if (onMessageCallback) onMessageCallback(msg);
#endif
}

void SDKManager::onWSClose() {
    std::cout << "SDKManager: WebSocket连接已关闭" << std::endl;
}

void SDKManager::onWSError(const std::string& err) {
    std::cout << "SDKManager: WebSocket错误 " << err << std::endl;
} 

#ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>
using namespace emscripten;

EMSCRIPTEN_BINDINGS(sdk_manager_bindings) {
    class_<SDKManager>("SDKManager")
        .constructor<WebSocketType>()
        .function("setWebSocketType", &SDKManager::setWebSocketType)
        .function("setJSWebSocketPtr", &SDKManager::setJSWebSocketPtr)
        .function("createWebSocket", &SDKManager::createWebSocket)
        .function("sendWebSocketMessage", &SDKManager::sendWebSocketMessage)
        .function("closeWebSocket", &SDKManager::closeWebSocket)
        .function("isWebSocketOpen", &SDKManager::isWebSocketOpen)
        .function("getLastMessage", &SDKManager::getLastMessage)
        .function("setInfo", &SDKManager::setInfo)
        .function("getInfo", &SDKManager::getInfo)
        .function("setOnMessageCallback", &SDKManager::setOnMessageCallback)
        ;
    enum_<WebSocketType>("WebSocketType")
        .value("CPP", WebSocketType::CPP)
        .value("JS", WebSocketType::JS);
}
#endif 