#include "sdk_c_api.h"
#include "../SDKManager.h"
#include <iostream>

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
    static_cast<SDKManager*>(h)->connect(std::string(url));
}

void sdk_send(sdk_handle h, const char* msg) {
    if (!h) return;
    static_cast<SDKManager*>(h)->send(std::string(msg));
}

void sdk_close(sdk_handle h) {
    if (!h) return;
    static_cast<SDKManager*>(h)->close();
}

void sdk_set_websocket(sdk_handle h, websocket_handle ws) {
    if (!h) return;
    static_cast<SDKManager*>(h)->setWebSocket(static_cast<WebSocketBase*>(ws));
}

void sdk_set_message_callback(sdk_handle h, sdk_message_callback cb, void* user_data) {
    if (!h) return;
    static_cast<SDKManager*>(h)->setMessageCallback(
        [cb, user_data](const std::string& msg) {
            std::cout << "[C++] sdk_set_message_callback: msg='" << msg << "'\n";
            std::cout << "[C++] msg.size()=" << msg.size() << std::endl;
            if (cb) {
                char* buf = (char*)malloc(msg.size() + 1);
                std::memcpy(buf, msg.c_str(), msg.size() + 1);
                std::cout << "[C++] malloc buf ptr=" << (void*)buf << std::endl;
                std::cout << "[C++] buf content='" << buf << "'\n";
                cb(buf, user_data);
            }
        }
    );
}
void sdk_set_open_callback(sdk_handle h, sdk_open_callback cb, void* user_data) {
    if (!h) return;
    static_cast<SDKManager*>(h)->setOpenCallback(
        [cb, user_data]() {
            if (cb) cb(user_data);
        }
    );
}
void sdk_set_close_callback(sdk_handle h, sdk_close_callback cb, void* user_data) {
    if (!h) return;
    static_cast<SDKManager*>(h)->setCloseCallback(
        [cb, user_data]() {
            if (cb) cb(user_data);
        }
    );
}
void sdk_set_error_callback(sdk_handle h, sdk_error_callback cb, void* user_data) {
    if (!h) return;
    static_cast<SDKManager*>(h)->setErrorCallback(
        [cb, user_data](const std::string& err) {
            std::cout << "[C++] sdk_set_error_callback: err='" << err << "'\n";
            std::cout << "[C++] err.size()=" << err.size() << std::endl;
            if (cb) {
                char* buf = (char*)malloc(err.size() + 1);
                std::memcpy(buf, err.c_str(), err.size() + 1);
                std::cout << "[C++] malloc buf ptr=" << (void*)buf << std::endl;
                std::cout << "[C++] buf content='" << buf << "'\n";
                cb(buf, user_data);
            }
        }
    );
}

} 