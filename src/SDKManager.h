#pragma once
#include <memory>
#include <string>

#include "websocket/WebsocketEvent.h"
#include "base/async/timer.h"
#include "business_manager/BusinessManager.h"
#include "message_define/common.h"
#include "websocket/WebSocketHolder.h"
#include "topic_engine/TopicManager.h"

// 整个 SDK 的管理器，一条 websocket 链路创建一个 SDKManager 实例
// WebSocketHolder: 管理 websocket 连接
// TopicManager: 在 WebSocketHolder 基础上实现消息收发的匹配，解析消息内容
// BusinessManager: 业务逻辑处理
class SDKManager {
public:
    SDKManager() = default;
    ~SDKManager() = default;

    void init(const SdkInitializeInfo& info);
    bool isInit();

    void setWebSocket(WebSocketBase* ws); // 注入 websocket 能力 (ENABLE_LIBWEBSOCKETS 为 false 时使用)
    void setWebsocketEventListener(const std::shared_ptr<WebsocketEvent>& listener); // 设置链接事件回调
    void connect(const std::string& url); // websocket 连接

    // todo:sdk 改成 weak_ptr
    std::shared_ptr<BusinessManager> getBusinessManager() {
        return business_manager_;
    }

    /**
     * 轮询主线程任务
     * 用户需要定期调用此方法来处理主线程任务
     * @return 执行的任务数量
     */
    static size_t poll();

private:
    std::atomic<bool> is_init_ = false;

    void initExecutors();
    std::shared_ptr<Timer> main_poller_timer_ = std::make_shared<Timer>();

    std::shared_ptr<WebsocketEvent> websocket_event_listener_;
    std::shared_ptr<WebSocketHolder> wsHolder_ = std::make_shared<WebSocketHolder>();
    std::shared_ptr<TopicManager> topic_manager_ = std::make_shared<TopicManager>();;
    std::shared_ptr<BusinessManager> business_manager_;
};