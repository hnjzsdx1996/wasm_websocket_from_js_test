#pragma once
#include "WebSocketBase.h"
#include <memory>
#include <string>
#include <thread>

// C++原生WebSocket实现, 使用 libwebsockets 实现
class CppWebSocket : public WebSocketBase {
public:
    CppWebSocket();
    ~CppWebSocket() override;

    void connect(const std::string& url) override;
    void send(const std::string& message) override;
    void close() override;
    bool isOpen() const override;
private:
#ifdef ENABLE_LIBWEBSOCKETS
    class Impl;
    std::unique_ptr<Impl> pImpl;
#else
    void* pImpl;
#endif
};