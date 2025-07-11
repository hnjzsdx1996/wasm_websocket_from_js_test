#pragma once
#include "WebSocketBase.h"
#include <memory>
#include <string>

// C++原生WebSocket实现（可以使用boost::beast或其他库）
class CppWebSocket : public WebSocketBase {
public:
    CppWebSocket();
    ~CppWebSocket() override;

    void connect(const std::string& url) override;
    void send(const std::string& message) override;
    void close() override;
    bool isOpen() const override;
    void setOnMessage(MessageCallback cb) override;
    void setOnOpen(OpenCallback cb) override;
    void setOnClose(CloseCallback cb) override;
    void setOnError(ErrorCallback cb) override;
private:
    class Impl;
    std::unique_ptr<Impl> pImpl;
    bool connected = false;
    MessageCallback onMessage_;
    OpenCallback onOpen_;
    CloseCallback onClose_;
    ErrorCallback onError_;
}; 