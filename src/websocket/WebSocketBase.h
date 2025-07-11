#pragma once
#include <string>
#include <functional>

// WebSocket抽象基类，便于未来C++原生实现
class WebSocketBase {
public:
    using MessageCallback = std::function<void(const std::string&)>;
    using OpenCallback = std::function<void()>;
    using CloseCallback = std::function<void()>;
    using ErrorCallback = std::function<void(const std::string&)>;
    virtual ~WebSocketBase() = default;
    virtual void connect(const std::string& url) = 0;
    virtual void send(const std::string& message) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;
    virtual void setOnMessage(MessageCallback cb) = 0;
    virtual void setOnOpen(OpenCallback cb) = 0;
    virtual void setOnClose(CloseCallback cb) = 0;
    virtual void setOnError(ErrorCallback cb) = 0;
}; 