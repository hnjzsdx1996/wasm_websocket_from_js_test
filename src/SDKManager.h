#pragma once
#include <memory>
#include "base/async/timer.h"
#include <string>

#include "business_manager/BusinessManager.h"
#include "websocket/WebSocketHolder.h"
#include "topic_engine/TopicManager.h"

// 整个 SDK 的管理器，一条 websocket 链路创建一个 SDKManager 实例
// WebSocketHolder: 管理 websocket 连接
// TopicManager: 在 WebSocketHolder 基础上实现消息收发的匹配，解析消息内容
// BusinessManager: 业务逻辑处理
class SDKManager {
public:
    SDKManager();
    ~SDKManager();

    // sdk 配置
    void configure(const std::string& config);

    // 注入 websocket 能力
    void setWebSocket(WebSocketBase* ws);
    std::weak_ptr<WebSocketHolder> getWebSocketHolder();

    // todo:sdk 改成 weak_ptr
    std::shared_ptr<TopicManager> getTopicManager() {
        return topic_manager_;
    }

    // todo:sdk 改成 weak_ptr
    std::shared_ptr<BusinessManager> getBusinessManager() {
        return business_manager_;
    }

    /**
     * 轮询主线程任务
     * 用户需要定期调用此方法来处理主线程任务
     * @return 执行的任务数量
     */
    size_t poll();

private:
    void initExecutors();

    std::shared_ptr<WebSocketHolder> wsHolder_;
    std::shared_ptr<TopicManager> topic_manager_;
    std::shared_ptr<BusinessManager> business_manager_;
};