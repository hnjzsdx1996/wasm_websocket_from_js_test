#include "./JSWebSocket.h"
#include "../../../src/base/logger/logger.h"
#include "../../../src/base/async/thread_pool_executor.h"
#include <chrono>
#include <unordered_map>
#include <mutex>
#include <emscripten/emscripten.h>

// 声明外部C函数，这些函数将在JavaScript中实现
extern "C" {
    EMSCRIPTEN_KEEPALIVE void js_websocket_connect(void* wsPtr, const char* url);
    EMSCRIPTEN_KEEPALIVE void js_websocket_send(void* wsPtr, const char* message);
    EMSCRIPTEN_KEEPALIVE void js_websocket_close(void* wsPtr);
    EMSCRIPTEN_KEEPALIVE bool js_websocket_is_open(void* wsPtr);
}

// 静态映射 wsPtr -> JSWebSocket*
static std::unordered_map<void*, JSWebSocket*> g_wsPtr2Instance;
static std::mutex g_wsMapMutex;

// 导出给JavaScript的回调函数
extern "C" {
    EMSCRIPTEN_KEEPALIVE void js_websocket_on_open(void* wsPtr) {
        // NC_LOG_INFO("JSWebSocket: 连接已打开 (wsPtr: %p)", wsPtr);
        std::lock_guard<std::mutex> lock(g_wsMapMutex);
        auto it = g_wsPtr2Instance.find(wsPtr);
        if (it != g_wsPtr2Instance.end() && it->second) {
            it->second->onJSOpen();
        }
    }

    EMSCRIPTEN_KEEPALIVE void js_websocket_on_message(void* wsPtr, const char* message) {
        // NC_LOG_INFO("JSWebSocket: 收到消息 %s (wsPtr: %p)", message, wsPtr);
        std::lock_guard<std::mutex> lock(g_wsMapMutex);
        auto it = g_wsPtr2Instance.find(wsPtr);
        if (it != g_wsPtr2Instance.end() && it->second) {
            it->second->onJSMessage(message);
        }
    }

    EMSCRIPTEN_KEEPALIVE void js_websocket_on_close(void* wsPtr) {
        // NC_LOG_INFO("JSWebSocket: 连接已关闭 (wsPtr: %p)", wsPtr);
        std::lock_guard<std::mutex> lock(g_wsMapMutex);
        auto it = g_wsPtr2Instance.find(wsPtr);
        if (it != g_wsPtr2Instance.end() && it->second) {
            it->second->onJSClose();
        }
    }

    EMSCRIPTEN_KEEPALIVE void js_websocket_on_error(void* wsPtr, const char* error) {
        // NC_LOG_INFO("JSWebSocket: 发生错误 %s (wsPtr: %p)", error, wsPtr);
        std::lock_guard<std::mutex> lock(g_wsMapMutex);
        auto it = g_wsPtr2Instance.find(wsPtr);
        if (it != g_wsPtr2Instance.end() && it->second) {
            it->second->onJSError(error);
        }
    }
}

JSWebSocket::JSWebSocket() {
    NC_LOG_INFO("JSWebSocket: ctor");
    this->jsWebSocketPtr = this; // 关键：用 this 指针作为 wsPtr
    // 注册到全局映射
    std::lock_guard<std::mutex> lock(g_wsMapMutex);
    g_wsPtr2Instance[jsWebSocketPtr] = this;
}

JSWebSocket::~JSWebSocket() {
    NC_LOG_INFO("JSWebSocket: dtor");
    if (connected) {
        close();
    }
    // 从全局映射移除
    std::lock_guard<std::mutex> lock(g_wsMapMutex);
    g_wsPtr2Instance.erase(jsWebSocketPtr);
}

void JSWebSocket::connect(const std::string& url) {
    if (!jsWebSocketPtr) {
        NC_LOG_INFO("JSWebSocket: 未设置JavaScript WebSocket对象");
        return;
    }

    NC_LOG_INFO("JSWebSocket: 连接到 %s", url.c_str());
    js_websocket_connect(jsWebSocketPtr, url.c_str());
}

void JSWebSocket::send(const std::string& message) {
    if (!jsWebSocketPtr) {
        NC_LOG_INFO("JSWebSocket: 未设置JavaScript WebSocket对象");
        return;
    }

    if (!connected) {
        NC_LOG_INFO("JSWebSocket: 未连接，无法发送消息");
        return;
    }
    
    NC_LOG_INFO("JSWebSocket: 发送消息 %s", message.c_str());
    js_websocket_send(jsWebSocketPtr, message.c_str());
}

void JSWebSocket::close() {
    if (!jsWebSocketPtr) {
        return;
    }

    if (!connected) {
        return;
    }
    
    NC_LOG_INFO("JSWebSocket: 关闭连接");
    js_websocket_close(jsWebSocketPtr);
}

bool JSWebSocket::isOpen() const {
    if (!jsWebSocketPtr) {
        return false;
    }
    return js_websocket_is_open(jsWebSocketPtr);
}

void JSWebSocket::onJSMessage(const std::string& message) {
    NC_LOG_INFO("JSWebSocket: 收到消息 %s", message.c_str());
    callOnMessage(message);
}

void JSWebSocket::onJSOpen() {
    NC_LOG_INFO("JSWebSocket: 连接已打开");
    connected = true;
    callOnOpen();
}

void JSWebSocket::onJSClose() {
    NC_LOG_INFO("JSWebSocket: 连接已关闭");
    connected = false;
    callOnClose();
}

void JSWebSocket::onJSError(const std::string& error) {
    NC_LOG_INFO("JSWebSocket: 发生错误 %s", error.c_str());
    connected = false;
    callOnError(error);
} 