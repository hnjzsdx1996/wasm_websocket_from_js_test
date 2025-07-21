#pragma once
#include <string>
#include <functional>
#include <list>

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

    void setOnMessage(MessageCallback cb);
    void setOnOpen(OpenCallback cb);
    void setOnClose(CloseCallback cb);
    void setOnError(ErrorCallback cb);

protected:
    void callOnMessage(const std::string& msg);
    void callOnOpen();
    void callOnClose();
    void callOnError(const std::string& err);

    bool connected{false};

private:

    std::list<MessageCallback> message_callbacks_;
    OpenCallback open_callback_;
    CloseCallback close_callback_;
    ErrorCallback error_callback_;

    std::mutex mtx_;
};