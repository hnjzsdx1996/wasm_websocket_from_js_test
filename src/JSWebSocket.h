#pragma once
#include "WebSocketBase.h"
#include <string>
#include <emscripten/val.h>

class JSWebSocket : public WebSocketBase {
public:
    JSWebSocket();
    ~JSWebSocket() override;

    void connect(const std::string& url) override;
    void send(const std::string& message) override;
    void close() override;

    // 供JS回调C++
    void onOpenFromJS();
    void onMessageFromJS(const std::string& msg);
    void onCloseFromJS();
    void onErrorFromJS(const std::string& err);

    void setOnMessageCallback(std::function<void(std::string)> cb) override;

private:
    int wsId; // 支持多连接
    std::function<void(std::string)> cppOnMessageCallback;
}; 