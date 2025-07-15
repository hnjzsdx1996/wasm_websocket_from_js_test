#pragma once
#include <stddef.h>  // for size_t
#ifdef __cplusplus
extern "C" {
#endif

typedef void* sdk_handle;
typedef void* websocket_handle;

// todo:sdk 不应该直接把 websocket 的原始 message_callback 之类的接口给到用户，需要封装
typedef void (*sdk_message_callback)(const char* msg, void* user_data);
typedef void (*sdk_open_callback)(void* user_data);
typedef void (*sdk_close_callback)(void* user_data);
typedef void (*sdk_error_callback)(const char* error, void* user_data);
void sdk_set_message_callback(sdk_handle h, sdk_message_callback cb, void* user_data);
void sdk_set_open_callback(sdk_handle h, sdk_open_callback cb, void* user_data);
void sdk_set_close_callback(sdk_handle h, sdk_close_callback cb, void* user_data);
void sdk_set_error_callback(sdk_handle h, sdk_error_callback cb, void* user_data);

sdk_handle sdk_create();
void sdk_destroy(sdk_handle h);
void sdk_configure(sdk_handle h, const char* config_str);
void sdk_connect(sdk_handle h, const char* url);
void sdk_send(sdk_handle h, const char* msg);
void sdk_close(sdk_handle h);
void sdk_set_websocket(sdk_handle h, websocket_handle ws);
size_t sdk_poll(sdk_handle h);

#ifdef __cplusplus
}
#endif 