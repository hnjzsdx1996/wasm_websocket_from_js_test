#pragma once
#include <string>
#include <functional>

// WebSocket抽象基类，便于未来C++原生实现
class WebSocketBase {
public:
    using OnOpenCallback = std::function<void()>;
    using OnMessageCallback = std::function<void(std::string)>;
    using OnCloseCallback = std::function<void()>;
    using OnErrorCallback = std::function<void(const std::string&)>;

    virtual ~WebSocketBase() = default;

    virtual void connect(const std::string& url) = 0;
    virtual void send(const std::string& message) = 0;
    virtual void close() = 0;
    virtual bool isOpen() const = 0;

    void setOnOpen(OnOpenCallback cb) { onOpen = std::move(cb); }
    void setOnMessage(OnMessageCallback cb) { onMessage = std::move(cb); }
    void setOnClose(OnCloseCallback cb) { onClose = std::move(cb); }
    void setOnError(OnErrorCallback cb) { onError = std::move(cb); }

protected:
    OnOpenCallback onOpen;
    OnMessageCallback onMessage;
    OnCloseCallback onClose;
    OnErrorCallback onError;
}; 