#include "SDKManager.h"
#include <chrono>
#include <iostream>
#include <utility>

#include "CppWebSocket.h"
#include "base/async/main_thread_executor.h"
#include "message_define/common.h"
#include "base/logger/logger.h"
#include "base/async/thread_pool_executor.h"
#include "topic_engine/TopicManager.h"
#include "business_manager/BusinessManager.h"

SDKManager::SDKManager() {
    nc_logger::init(plog::info, "notification_center_log.log");
    NC_LOG_INFO("[SDKManager] ctor: %p", this);

    // 初始化多线程
    initExecutors();

    // 初始化TopicManager和BusinessManager
    wsHolder_ = std::make_shared<WebSocketHolder>();
#ifdef ENABLE_LIBWEBSOCKETS
    wsHolder_->setWebSocket(std::make_shared<CppWebSocket>());
#endif
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

void SDKManager::setWebSocket(WebSocketBase* ws) {
#ifdef ENABLE_LIBWEBSOCKETS
    NC_LOG_INFO("[SDKManager] setWebSocket, but ENABLE_LIBWEBSOCKETS");
#else
    NC_LOG_INFO("[SDKManager] setWebSocket: %p", ws);
    std::shared_ptr<WebSocketBase> sp_ws(ws);
    wsHolder_->setWebSocket(sp_ws);
#endif
}

std::weak_ptr<WebSocketHolder> SDKManager::getWebSocketHolder() {
    return wsHolder_;
}

size_t SDKManager::poll() {
    // 轮询主线程任务
    return ThreadPoolExecutor::Main().poll();
}

void SDKManager::initExecutors() {
    NC_LOG_INFO("[SDKManager] initExecutors");
    ThreadPoolExecutor::Worker();
    ThreadPoolExecutor::IO();
    ThreadPoolExecutor::Compute();

    // // 用于测试多线程 Timer
    // worker_timer_ = std::make_shared<Timer>();
    // worker_timer_->SetDefaultExecutor(ThreadPoolExecutor::Worker());
    // worker_timer_->PostRepeating(std::chrono::milliseconds(1000), std::chrono::milliseconds(60000), []()->void {
    //     NC_LOG_INFO("[SDKManager] Worker timer");
    // });
    // io_timer_ = std::make_shared<Timer>();
    // io_timer_->SetDefaultExecutor(ThreadPoolExecutor::IO());
    // io_timer_->PostRepeating(std::chrono::milliseconds(1000), std::chrono::milliseconds(60000), []()->void {
    //     NC_LOG_INFO("[SDKManager] IO timer");
    // });
    // compute_timer_ = std::make_shared<Timer>();
    // compute_timer_->SetDefaultExecutor(ThreadPoolExecutor::Compute());
    // compute_timer_->PostRepeating(std::chrono::milliseconds(1000), std::chrono::milliseconds(60000), []()->void {
    //     NC_LOG_INFO("[SDKManager] Compute timer");
    // });
}
