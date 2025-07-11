#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "WebSocketBase.h"
#include <cstdint>
#ifdef __EMSCRIPTEN__
#include <emscripten/val.h>
#endif

// WebSocket类型枚举
enum class WebSocketType {
    CPP,    // C++原生实现
    JS      // JavaScript实现
};

class SDKManager {
public:
    // 构造函数，可以指定WebSocket类型
    explicit SDKManager(WebSocketType wsType = WebSocketType::CPP);
    ~SDKManager();

    // 设置WebSocket类型（在创建连接前调用）
    void setWebSocketType(WebSocketType wsType);
    
    // 设置JavaScript WebSocket对象（仅在使用JS类型时）
    void setJSWebSocketPtr(uintptr_t jsWebSocketPtr);

    // WebSocket相关
    void createWebSocket(const std::string& url);
    void sendWebSocketMessage(const std::string& msg);
    void closeWebSocket();
    bool isWebSocketOpen() const;
    std::string getLastMessage() const;

    // 其他接口
    void setInfo(const std::string& key, const std::string& value);
    std::string getInfo(const std::string& key) const;
#ifdef __EMSCRIPTEN__
    void setOnMessageCallback(emscripten::val cb);
#endif

private:
    std::unique_ptr<WebSocketBase> ws;
    WebSocketType currentWsType;
    void* jsWebSocketPtr = nullptr;
    std::string lastMessage;
    std::unordered_map<std::string, std::string> infoMap;
#ifdef __EMSCRIPTEN__
    std::function<void(std::string)> onMessageCallback;
#endif

    // 创建WebSocket实例
    void createWebSocketInstance();
    
    // 回调
    void onWSOpen();
    void onWSMessage(const std::string& msg);
    void onWSClose();
    void onWSError(const std::string& err);
};  