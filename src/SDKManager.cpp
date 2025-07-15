#include "SDKManager.h"
#include <chrono>
#include <iostream>
#include <utility>
#include "base/async/main_thread_executor.h"
#include "message_define/common.h"
#include "base/logger/logger.h"
#include "base/async/thread_pool_executor.h"
#include "topic_engine/TopicManager.h"
#include "business_manager/BusinessManager.h"

SDKManager::SDKManager() {
    nc_logger::init(plog::info, "notification_center_log.log");

    NC_LOG_INFO("[SDKManager] ctor: %p", this);
    // 初始化TopicManager和BusinessManager
    // todo:sdk wsHolder_ 的构造，内部持有的 websocket 对象的使用重构
    wsHolder_ = std::make_shared<WebSocketHolder>();
    topic_manager_ = std::make_shared<TopicManager>();
    business_manager_ = std::make_shared<BusinessManager>(topic_manager_);
    topic_manager_->setWebSocketHolder(wsHolder_);
}

SDKManager::~SDKManager() {
    NC_LOG_INFO("[SDKManager] dtor: %p", this);
}

void SDKManager::configure(const std::string& config) {
    NC_LOG_INFO("[SDKManager] configure %s", config.c_str());
    auto config_ = SDKConfig();
    config_.FromJsonString(config);
    NC_LOG_INFO("[SDKManager] configure sn: %s, ping_pong_interval: %d", config_.sn.c_str(), config_.ping_pong_interval);
}

void SDKManager::connect(const std::string& url) {
    if (wsHolder_->getWebSocket()) {
        wsHolder_->getWebSocket()->connect(url);
    }
}

void SDKManager::send(const std::string& message) {
    if (wsHolder_->getWebSocket()) {
        wsHolder_->getWebSocket()->send(message);
    }
}

void SDKManager::close() {
    if (wsHolder_->getWebSocket()) {
        wsHolder_->getWebSocket()->close();
    }
}

void SDKManager::setWebSocket(WebSocketBase* ws) {
    NC_LOG_INFO("[SDKManager] setWebSocket: %p", ws);
    std::shared_ptr<WebSocketBase> sp_ws(ws);
    wsHolder_->setWebSocket(sp_ws);
}

std::weak_ptr<WebSocketHolder> SDKManager::getWebSocketHolder() {
    return wsHolder_;
}

size_t SDKManager::poll() {
    // 轮询主线程任务
    return ThreadPoolExecutor::Main().poll();
}