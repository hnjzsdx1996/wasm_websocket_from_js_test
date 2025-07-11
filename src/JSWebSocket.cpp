#include "JSWebSocket.h"
#include <iostream>
#include <unordered_map>
#include <mutex>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#define EMS_KEEP EMSCRIPTEN_KEEPALIVE
#else
#define EMS_KEEP
#endif

// 声明外部C函数，这些函数将在JavaScript中实现
extern "C" {
    EMS_KEEP void js_websocket_connect(void* wsPtr, const char* url);
    EMS_KEEP void js_websocket_send(void* wsPtr, const char* message);
    EMS_KEEP void js_websocket_close(void* wsPtr);
    EMS_KEEP bool js_websocket_is_open(void* wsPtr);
}

// 静态映射 wsPtr -> JSWebSocket*
static std::unordered_map<void*, JSWebSocket*> g_wsPtr2Instance;
static std::mutex g_wsMapMutex;

JSWebSocket::JSWebSocket() {
    std::cout << "JSWebSocket: ctor" << std::endl;
    // 注册到全局映射
    std::lock_guard<std::mutex> lock(g_wsMapMutex);
    g_wsPtr2Instance[jsWebSocketPtr] = this;
}

JSWebSocket::~JSWebSocket() {
    std::cout << "JSWebSocket: dtor" << std::endl;
    if (connected) {
        close();
    }
    // 从全局映射移除
    std::lock_guard<std::mutex> lock(g_wsMapMutex);
    g_wsPtr2Instance.erase(jsWebSocketPtr);
}

void JSWebSocket::setJSWebSocketPtr(void* jsWebSocketPtr) {
    this->jsWebSocketPtr = jsWebSocketPtr;
    // 更新全局映射
    std::lock_guard<std::mutex> lock(g_wsMapMutex);
    g_wsPtr2Instance[jsWebSocketPtr] = this;
}

void JSWebSocket::connect(const std::string& url) {
    if (!jsWebSocketPtr) {
        std::cout << "JSWebSocket: 未设置JavaScript WebSocket对象" << std::endl;
        return;
    }
    
    std::cout << "JSWebSocket: 连接到 " << url << std::endl;
    js_websocket_connect(jsWebSocketPtr, url.c_str());
}

void JSWebSocket::send(const std::string& message) {
    if (!jsWebSocketPtr) {
        std::cout << "JSWebSocket: 未设置JavaScript WebSocket对象" << std::endl;
        return;
    }
    
    if (!connected) {
        std::cout << "JSWebSocket: 未连接，无法发送消息" << std::endl;
        return;
    }
    
    std::cout << "JSWebSocket: 发送消息 " << message << std::endl;
    js_websocket_send(jsWebSocketPtr, message.c_str());
}

void JSWebSocket::close() {
    if (!jsWebSocketPtr) {
        return;
    }
    
    if (!connected) {
        return;
    }
    
    std::cout << "JSWebSocket: 关闭连接" << std::endl;
    js_websocket_close(jsWebSocketPtr);
    connected = false;
}

bool JSWebSocket::isOpen() const {
    if (!jsWebSocketPtr) {
        return false;
    }
    return js_websocket_is_open(jsWebSocketPtr);
}

// 导出给JavaScript的回调函数
extern "C" {
    EMS_KEEP void js_websocket_on_open(void* wsPtr) {
        std::cout << "JSWebSocket: 连接已打开 (wsPtr: " << wsPtr << ")" << std::endl;
        std::lock_guard<std::mutex> lock(g_wsMapMutex);
        auto it = g_wsPtr2Instance.find(wsPtr);
        if (it != g_wsPtr2Instance.end() && it->second) {
            it->second->onJSOpen();
        }
    }
    
    EMS_KEEP void js_websocket_on_message(void* wsPtr, const char* message) {
        std::cout << "JSWebSocket: 收到消息 " << message << " (wsPtr: " << wsPtr << ")" << std::endl;
        std::lock_guard<std::mutex> lock(g_wsMapMutex);
        auto it = g_wsPtr2Instance.find(wsPtr);
        if (it != g_wsPtr2Instance.end() && it->second) {
            it->second->onJSMessage(message);
        }
    }
    
    EMS_KEEP void js_websocket_on_close(void* wsPtr) {
        std::cout << "JSWebSocket: 连接已关闭 (wsPtr: " << wsPtr << ")" << std::endl;
        std::lock_guard<std::mutex> lock(g_wsMapMutex);
        auto it = g_wsPtr2Instance.find(wsPtr);
        if (it != g_wsPtr2Instance.end() && it->second) {
            it->second->onJSClose();
        }
    }
    
    EMS_KEEP void js_websocket_on_error(void* wsPtr, const char* error) {
        std::cout << "JSWebSocket: 发生错误 " << error << " (wsPtr: " << wsPtr << ")" << std::endl;
        std::lock_guard<std::mutex> lock(g_wsMapMutex);
        auto it = g_wsPtr2Instance.find(wsPtr);
        if (it != g_wsPtr2Instance.end() && it->second) {
            it->second->onJSError(error);
        }
    }
}

// JavaScript回调函数实现
void JSWebSocket::onJSMessage(const std::string& message) {
    std::cout << "JSWebSocket: 收到消息 " << message << std::endl;
    if (onMessage) {
        onMessage(message);
    }
}

void JSWebSocket::onJSOpen() {
    std::cout << "JSWebSocket: 连接已打开" << std::endl;
    connected = true;
    if (onOpen) {
        onOpen();
    }
}

void JSWebSocket::onJSClose() {
    std::cout << "JSWebSocket: 连接已关闭" << std::endl;
    connected = false;
    if (onClose) {
        onClose();
    }
}

void JSWebSocket::onJSError(const std::string& error) {
    std::cout << "JSWebSocket: 发生错误 " << error << std::endl;
    if (onError) {
        onError(error);
    }
} 