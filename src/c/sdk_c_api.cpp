#include "sdk_c_api.h"
#include "../SDKManager.h"
#include "../base/logger/logger.h"

extern "C" {

sdk_handle sdk_create() {
    return new SDKManager();
}

void sdk_destroy(sdk_handle h) {
    delete static_cast<SDKManager*>(h);
}

void sdk_configure(sdk_handle h, const char* config_str) {
    if (!h) return;
    static_cast<SDKManager*>(h)->configure(std::string(config_str));
}

void sdk_connect(sdk_handle h, const char* url) {
    if (!h) return;
    auto strong_ws_holder = static_cast<SDKManager*>(h)->getWebSocketHolder().lock();
    if (strong_ws_holder == nullptr) {
        NC_LOG_INFO("[C API] sdk_connect: strong_ws_holder is nullptr");
        return;
    }
    const auto ws = strong_ws_holder->getWebSocket();
    if (ws == nullptr) {
        return;
    }
    ws->connect(std::string(url));
}

void sdk_send(sdk_handle h, const char* msg) {
    if (!h) return;
    static_cast<SDKManager*>(h)->getBusinessManager()->Send(std::string(msg));
}

void sdk_close(sdk_handle h) {
    if (!h) return;
    auto strong_ws_holder = static_cast<SDKManager*>(h)->getWebSocketHolder().lock();
    if (strong_ws_holder == nullptr) {
        NC_LOG_INFO("[C API] sdk_send: strong_ws_holder is nullptr");
        return;
    }
    const auto ws = strong_ws_holder->getWebSocket();
    if (ws == nullptr) {
        return;
    }
    ws->close();
}

void sdk_set_websocket(sdk_handle h, websocket_handle ws) {
    if (!h) return;
    auto strong_ws_holder = static_cast<SDKManager*>(h)->getWebSocketHolder().lock();
    if (strong_ws_holder == nullptr) {
        NC_LOG_INFO("[C API] sdk_set_websocket: strong_ws_holder is nullptr");
        return;
    }
    const std::shared_ptr<WebSocketBase> sp_ws(static_cast<WebSocketBase*>(ws));
    strong_ws_holder->setWebSocket(sp_ws);
}

size_t sdk_poll(sdk_handle h) {
    if (!h) return 0;
    return static_cast<SDKManager*>(h)->poll();
}

void sdk_set_message_callback(sdk_handle h, sdk_message_callback cb, void* user_data) {
    if (!h) return;
    static_cast<SDKManager *>(h)->getBusinessManager()->Observe([cb, user_data](const std::string &msg) {
        if (cb) {
            char *buf = (char *) malloc(msg.size() + 1);
            std::memcpy(buf, msg.c_str(), msg.size() + 1);
            cb(buf, user_data);
        }
    });
}

void sdk_set_open_callback(sdk_handle h, sdk_open_callback cb, void* user_data) {
    if (!h) return;
    auto strong_ws_holder = static_cast<SDKManager*>(h)->getWebSocketHolder().lock();
    if (strong_ws_holder == nullptr) {
        NC_LOG_INFO("[C API] sdk_set_open_callback: strong_ws_holder is nullptr");
        return;
    }
    strong_ws_holder->setOnOpen(
        [cb, user_data]() {
            if (cb) cb(user_data);
        }
    );
}

void sdk_set_close_callback(sdk_handle h, sdk_close_callback cb, void *user_data) {
    if (!h) return;
    auto strong_ws_holder = static_cast<SDKManager *>(h)->getWebSocketHolder().lock();
    if (strong_ws_holder == nullptr) {
        NC_LOG_INFO("[C API] sdk_set_open_callback: strong_ws_holder is nullptr");
        return;
    }
    strong_ws_holder->setOnClose(
        [cb, user_data]() {
            if (cb) cb(user_data);
        }
    );
}

void sdk_set_error_callback(sdk_handle h, sdk_error_callback cb, void *user_data) {
    if (!h) return;
    auto strong_ws_holder = static_cast<SDKManager *>(h)->getWebSocketHolder().lock();
    if (strong_ws_holder == nullptr) {
        NC_LOG_INFO("[C API] sdk_set_open_callback: strong_ws_holder is nullptr");
        return;
    }
    strong_ws_holder->setOnError(
        [cb, user_data](const std::string &err) {
            if (cb) {
                char *buf = (char *) malloc(err.size() + 1);
                std::memcpy(buf, err.c_str(), err.size() + 1);
                cb(buf, user_data);
            }
        }
    );
}

int sdk_listen_aircraft_location(sdk_handle h, sdk_listen_message_callback on_messages_callback, void* msg_user_data, sdk_listen_result_callback on_result_callback, void* result_user_data, const char* device_sn, int freq) {
    if (!h) return -1;
    SDKManager* mgr = reinterpret_cast<SDKManager*>(h);
    auto business_mgr = mgr->getBusinessManager();
    if (!business_mgr) return -2;
    // 适配回调，分配新内存，确保生命周期
    auto msg_cb = [on_messages_callback, msg_user_data](const std::string& msg) {
        if (on_messages_callback) {
            char* buf = (char*)malloc(msg.size() + 1);
            memcpy(buf, msg.c_str(), msg.size() + 1);
            on_messages_callback(buf, msg_user_data);
            free(buf);
        }
    };
    auto result_cb = [on_result_callback, result_user_data](int result) {
        if (on_result_callback) on_result_callback(result, result_user_data);
    };
    return business_mgr->ListenAircraftLocation(msg_cb, result_cb, device_sn, static_cast<NotifactionFrequency>(freq));
}

}