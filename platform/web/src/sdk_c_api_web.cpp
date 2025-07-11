#include "../../../src/c/sdk_c_api.h"
#include "./JSWebSocket.h"
#include <emscripten/emscripten.h>

extern "C" {

EMSCRIPTEN_KEEPALIVE
sdk_handle js_sdk_create() {
    return ::sdk_create();
}

EMSCRIPTEN_KEEPALIVE
websocket_handle js_sdk_create_js_websocket() {
    return new JSWebSocket();
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_destroy(sdk_handle h) {
    ::sdk_destroy(h);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_configure(sdk_handle h, const char* config_str) {
    ::sdk_configure(h, config_str);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_connect(sdk_handle h, const char* url) {
    ::sdk_connect(h, url);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_send(sdk_handle h, const char* msg) {
    ::sdk_send(h, msg);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_close(sdk_handle h) {
    ::sdk_close(h);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_set_websocket(sdk_handle h, websocket_handle ws) {
    ::sdk_set_websocket(h, ws);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_set_message_callback(sdk_handle h, sdk_message_callback cb, void* user_data) {
    ::sdk_set_message_callback(h, cb, user_data);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_set_open_callback(sdk_handle h, sdk_open_callback cb, void* user_data) {
    ::sdk_set_open_callback(h, cb, user_data);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_set_close_callback(sdk_handle h, sdk_close_callback cb, void* user_data) {
    ::sdk_set_close_callback(h, cb, user_data);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_set_error_callback(sdk_handle h, sdk_error_callback cb, void* user_data) {
    ::sdk_set_error_callback(h, cb, user_data);
}

} 