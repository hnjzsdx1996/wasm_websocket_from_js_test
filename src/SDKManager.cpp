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

void SDKManager::init(const SdkInitializeInfo& info) {
    nc_logger::init(static_cast<plog::Severity>(info.log_level), "notification_center_sdk.log");
    NC_LOG_INFO("[SDKManager] ctor: %p", this);

    // 初始化多线程
    initExecutors();

    // 初始化TopicManager和BusinessManager
#ifdef ENABLE_LIBWEBSOCKETS
    NC_LOG_INFO("[SDKManager] ctor ENABLE_LIBWEBSOCKETS = true");
    wsHolder_->setWebSocket(std::make_shared<CppWebSocket>());
#else
    NC_LOG_INFO("[SDKManager] ctor ENABLE_LIBWEBSOCKETS = false");
#endif

    topic_manager_->setWebSocketHolder(wsHolder_);

    business_manager_ = std::make_shared<BusinessManager>(topic_manager_);

    is_init_ = true;
}

bool SDKManager::isInit() {
    return is_init_;
}

void SDKManager::connect(const std::string& url) {
    if (isInit() == false) {
        NC_LOG_ERROR("[SDKManager] connect: %s, isInit == false", url.c_str());
        return;
    }
    NC_LOG_INFO("[SDKManager] connect: %s", url.c_str());
    wsHolder_->getWebSocket()->connect(url);
}

void SDKManager::setWebsocketEventListener(const std::shared_ptr<WebsocketEvent>& listener) {
    // Store the listener
    websocket_event_listener_ = listener;

    // Set up C++ lambda callbacks on wsHolder_
    wsHolder_->setOnOpen([this]() {
        if (websocket_event_listener_) {
            websocket_event_listener_->OnOpen();
        }
    });
    wsHolder_->setOnClose([this]() {
        if (websocket_event_listener_) {
            websocket_event_listener_->OnClose();
        }
    });
    wsHolder_->setOnError([this](const std::string& error) {
        if (websocket_event_listener_) {
            websocket_event_listener_->OnError(error);
        }
    });
    wsHolder_->setOnMessage([this](const std::string& message) {
        if (websocket_event_listener_) {
            websocket_event_listener_->OnMessage(message);
        }
    });
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
}
