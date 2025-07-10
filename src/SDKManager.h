#pragma once
#include <memory>
#include <string>
#include "WebSocketBase.h"
#include <emscripten/val.h>

class SDKManager {
public:
    SDKManager();
    ~SDKManager();

    // WebSocket相关
    void createWebSocket(const std::string& url);
    void sendWebSocketMessage(const std::string& msg);
    void closeWebSocket();
    bool isWebSocketOpen() const;
    std::string getLastMessage() const;
    void setOnMessageCallbackJS(emscripten::val cb);

    // 其他接口
    void setInfo(const std::string& key, const std::string& value);
    std::string getInfo(const std::string& key) const;

private:
    std::unique_ptr<WebSocketBase> ws;
    bool wsOpen = false;
    std::string lastMessage;
    std::unordered_map<std::string, std::string> infoMap;
    std::function<void(std::string)> onMessageCallback;

    // 回调
    void onWSOpen();
    void onWSMessage(const std::string& msg);
    void onWSClose();
    void onWSError(const std::string& err);
}; 