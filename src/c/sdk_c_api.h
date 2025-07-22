#pragma once
#include <stddef.h>  // for size_t
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef void* sdk_handle;
typedef void* websocket_handle;

// 日志级别枚举
typedef enum {
    SDK_LOG_NONE = 0,
    SDK_LOG_FATAL = 1,
    SDK_LOG_ERROR = 2,
    SDK_LOG_WARNING = 3,
    SDK_LOG_INFO = 4,
    SDK_LOG_DEBUG = 5,
    SDK_LOG_VERBOSE = 6
} sdk_log_level_t;

// 初始化信息结构体
typedef struct {
    const char* log_path;
    sdk_log_level_t log_level;
} sdk_initialize_info_t;

// WebSocket 事件回调函数类型
typedef void (*websocket_on_message_callback)(const char* message);
typedef void (*websocket_on_open_callback)();
typedef void (*websocket_on_close_callback)();
typedef void (*websocket_on_error_callback)(const char* error);

sdk_handle sdk_create();
void sdk_destroy(sdk_handle h);

// 初始化 SDK
void sdk_init(sdk_handle h, const sdk_initialize_info_t* info);
int sdk_is_init(sdk_handle h);

// WebSocket 相关接口
void sdk_connect(sdk_handle h, const char* url);
void sdk_set_websocket(sdk_handle h, websocket_handle ws);
void sdk_set_websocket_event_listener(sdk_handle h, 
                                     websocket_on_message_callback on_message,
                                     websocket_on_open_callback on_open,
                                     websocket_on_close_callback on_close,
                                     websocket_on_error_callback on_error);

// 轮询接口
size_t sdk_poll(sdk_handle h);

// 业务管理接口
void sdk_cancel_observe(sdk_handle h, int64_t listen_id);

#ifdef __cplusplus
}
#endif 