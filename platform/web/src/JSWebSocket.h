#pragma once
#include "../../../src/websocket/WebSocketBase.h"
#include <string>

// JS WebSocket接口，用于接收来自JavaScript的WebSocket对象
class JSWebSocket : public WebSocketBase {
public:
    JSWebSocket();
    ~JSWebSocket() override;

    void connect(const std::string& url) override;
    void send(const std::string& message) override;
    void close() override;
    bool isOpen() const override;
    
    // 回调函数，供JavaScript调用
    void onJSMessage(const std::string& message);
    void onJSOpen();
    void onJSClose();
    void onJSError(const std::string& error);

private:
    void* jsWebSocketPtr = nullptr;
};