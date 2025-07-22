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
void js_sdk_init(sdk_handle h, const char* log_path, int log_level) {
    sdk_initialize_info_t info;
    info.log_path = log_path;
    info.log_level = static_cast<sdk_log_level_t>(log_level);
    ::sdk_init(h, &info);
}

EMSCRIPTEN_KEEPALIVE
int js_sdk_is_init(sdk_handle h) {
    return ::sdk_is_init(h);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_connect(sdk_handle h, const char* url) {
    ::sdk_connect(h, url);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_set_websocket(sdk_handle h, websocket_handle ws) {
    ::sdk_set_websocket(h, ws);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_set_websocket_event_listener(sdk_handle h, 
                                        websocket_on_message_callback on_message,
                                        websocket_on_open_callback on_open,
                                        websocket_on_close_callback on_close,
                                        websocket_on_error_callback on_error) {
    ::sdk_set_websocket_event_listener(h, on_message, on_open, on_close, on_error);
}

EMSCRIPTEN_KEEPALIVE
size_t js_sdk_poll(sdk_handle h) {
    return ::sdk_poll(h);
}

EMSCRIPTEN_KEEPALIVE
void js_sdk_cancel_observe(sdk_handle h, int64_t listen_id) {
    ::sdk_cancel_observe(h, listen_id);
}

} 