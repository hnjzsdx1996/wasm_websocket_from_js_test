#pragma once
#include <string>
#include "websocket/WebSocketHolder.h"

class SDKManager {
public:
    using MessageCallback = std::function<void(const std::string&)>;
    using OpenCallback = std::function<void()>;
    using CloseCallback = std::function<void()>;
    using ErrorCallback = std::function<void(const std::string&)>;
    SDKManager();
    ~SDKManager();

    // sdk 配置
    void configure(const std::string& config);

    // websocket 管理
    void connect(const std::string& url);
    void send(const std::string& message);
    void close();

    // 注入 websocket 能力
    void setWebSocket(WebSocketBase* ws);

    // 设置回调
    void setMessageCallback(MessageCallback cb);
    void setOpenCallback(OpenCallback cb);
    void setCloseCallback(CloseCallback cb);
    void setErrorCallback(ErrorCallback cb);

    /**
     * 轮询主线程任务
     * 用户需要定期调用此方法来处理主线程任务
     * @return 执行的任务数量
     */
    size_t poll();

    WebSocketHolder& getWebSocketHolder();

private:
    WebSocketHolder wsHolder_;
    MessageCallback messageCallback_;
    OpenCallback openCallback_;
    CloseCallback closeCallback_;
    ErrorCallback errorCallback_;
};  