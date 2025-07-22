#pragma once
#include <string>
#include <functional>
#include <list>
#include <mutex>

// WebSocket抽象基类
// 支持多种 websocket 链路（JS 平台使用注入方式，其他平台使用内置 libwebsockets 库）
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
    void clearMessageCallbacks(); // 清除所有消息回调
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