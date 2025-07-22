#include "sdk_c_api.h"
#include "../SDKManager.h"
#include "../base/logger/logger.h"
#include "../message_define/common.h"
#include "../websocket/WebsocketEvent.h"
#include "../websocket/WebSocketBase.h"
#include <memory>

// 内部事件监听器类，用于桥接 C 回调函数到 C++ 接口
class CWebsocketEventListener : public WebsocketEvent {
public:
    CWebsocketEventListener(websocket_on_message_callback on_message,
                           websocket_on_open_callback on_open,
                           websocket_on_close_callback on_close,
                           websocket_on_error_callback on_error)
        : on_message_(on_message)
        , on_open_(on_open)
        , on_close_(on_close)
        , on_error_(on_error) {}

    void OnMessage(const std::string& message) override {
        if (on_message_) {
            on_message_(message.c_str());
        }
    }

    void OnOpen() override {
        if (on_open_) {
            on_open_();
        }
    }

    void OnClose() override {
        if (on_close_) {
            on_close_();
        }
    }

    void OnError(const std::string& error) override {
        if (on_error_) {
            on_error_(error.c_str());
        }
    }

private:
    websocket_on_message_callback on_message_;
    websocket_on_open_callback on_open_;
    websocket_on_close_callback on_close_;
    websocket_on_error_callback on_error_;
};

extern "C" {

sdk_handle sdk_create() {
    return new SDKManager();
}

void sdk_destroy(sdk_handle h) {
    if (!h) return;
    delete static_cast<SDKManager*>(h);
}

void sdk_init(sdk_handle h, const sdk_initialize_info_t* info) {
    if (!h || !info) return;
    
    SdkInitializeInfo init_info;
    if (info->log_path) {
        init_info.log_path = std::string(info->log_path);
    }
    init_info.log_level = static_cast<SdkLogLevel>(info->log_level);
    
    static_cast<SDKManager*>(h)->init(init_info);
}

int sdk_is_init(sdk_handle h) {
    if (!h) return 0;
    return static_cast<SDKManager*>(h)->isInit() ? 1 : 0;
}

void sdk_connect(sdk_handle h, const char* url) {
    if (!h || !url) return;
    static_cast<SDKManager*>(h)->connect(std::string(url));
}

void sdk_set_websocket(sdk_handle h, websocket_handle ws) {
    if (!h) return;
    static_cast<SDKManager*>(h)->setWebSocket(static_cast<WebSocketBase *>(ws));
}

void sdk_set_websocket_event_listener(sdk_handle h, 
                                     websocket_on_message_callback on_message,
                                     websocket_on_open_callback on_open,
                                     websocket_on_close_callback on_close,
                                     websocket_on_error_callback on_error) {
    if (!h) return;
    
    auto* mgr = static_cast<SDKManager*>(h);
    auto listener = std::make_shared<CWebsocketEventListener>(
        on_message, on_open, on_close, on_error);
    mgr->setWebsocketEventListener(listener);
}

size_t sdk_poll(sdk_handle h) {
    if (!h) return 0;
    return static_cast<SDKManager*>(h)->poll();
}

void sdk_cancel_observe(sdk_handle h, int64_t listen_id) {
    if (!h) return;
    auto* mgr = static_cast<SDKManager*>(h);
    auto business_mgr = mgr->getBusinessManager();
    if (!business_mgr) return;
    business_mgr->CancelObserve(listen_id);
}

}