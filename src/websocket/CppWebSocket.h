#pragma once
#include "WebSocketBase.h"
#include <memory>
#include <string>

// C++原生WebSocket实现（可以使用boost::beast或其他库）
// todo:sdk 使用 libwebsockets 实现
class CppWebSocket : public WebSocketBase {
public:
    CppWebSocket();
    ~CppWebSocket() override;

    void connect(const std::string& url) override;
    void send(const std::string& message) override;
    void close() override;
    bool isOpen() const override;
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
};