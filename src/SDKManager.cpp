#include "SDKManager.h"
#include <chrono>
#include <iostream>
#include "base/async/main_thread_executor.h"
#include "message_define/common.h"
#include "base/logger/logger.h"
#include "base/async/thread_pool_executor.h"
#include "topic_engine/TopicManager.h"
#include "business_manager/BusinessManager.h"

SDKManager::SDKManager() {
    // 初始化TopicManager和BusinessManager
    topic_manager_ = std::make_shared<TopicManager>();
    business_manager_ = std::make_shared<BusinessManager>(topic_manager_.get());
    topic_manager_->setWebSocketHolder(&wsHolder_);
}

SDKManager::~SDKManager() {}

void SDKManager::configure(const std::string& config) {
    NC_LOG_INFO("SDKManager: 配置 %s", config.c_str());
    auto config_ = SDKConfig();
    config_.FromJsonString(config);
    NC_LOG_INFO("sn: %s, ping_pong_interval: %d", config_.sn.c_str(), config_.ping_pong_interval);

    // 测试Timer
    // timer_->SetDefaultExecutor(ThreadPoolExecutor::Worker());
    // timer_->PostRepeating(std::chrono::milliseconds(1000), std::chrono::milliseconds(1000), []()->void{
    //     NC_LOG_INFO("SDKManager: heart_beat");
    // });
}

void SDKManager::connect(const std::string& url) {
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->connect(url);
        wsHolder_.getWebSocket()->setOnMessage([this](const std::string& msg) {
            if (topic_manager_) {
                topic_manager_->OnWebSocketMessage(msg);
            }
            if (messageCallback_) messageCallback_(msg);
        });
        wsHolder_.getWebSocket()->setOnOpen([this]() {
            if (openCallback_) openCallback_();
        });
        wsHolder_.getWebSocket()->setOnClose([this]() {
            if (closeCallback_) closeCallback_();
        });
        wsHolder_.getWebSocket()->setOnError([this](const std::string& err) {
            if (errorCallback_) errorCallback_(err);
        });
    }
}

void SDKManager::send(const std::string& message) {
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->send(message);
    }
}

void SDKManager::close() {
    if (wsHolder_.getWebSocket()) {
        wsHolder_.getWebSocket()->close();
    }
}

void SDKManager::setWebSocket(WebSocketBase* ws) {
    wsHolder_.setWebSocket(ws);
    if (topic_manager_) {
        topic_manager_->setWebSocketHolder(&wsHolder_);
    }
}

WebSocketHolder& SDKManager::getWebSocketHolder() {
    return wsHolder_;
}

void SDKManager::setMessageCallback(MessageCallback cb) {
    messageCallback_ = cb;
}

void SDKManager::setOpenCallback(OpenCallback cb) {
    openCallback_ = cb;
}

void SDKManager::setCloseCallback(CloseCallback cb) {
    closeCallback_ = cb;
}

void SDKManager::setErrorCallback(ErrorCallback cb) {
    errorCallback_ = cb;
}

size_t SDKManager::poll() {
    // 轮询主线程任务
    return ThreadPoolExecutor::Main().poll();
}