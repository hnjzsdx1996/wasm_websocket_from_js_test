#include "../src/c/sdk_c_api.h"
#include "base/logger/logger.h"
#include <thread>
#include <chrono>

[[noreturn]] void runloop(sdk_handle sdk) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        sdk_poll(sdk);
    }
}

constexpr auto address = "wss://echo.websocket.org";

// 全局变量用于在回调中访问
sdk_handle g_sdk_handle = nullptr;

// WebSocket 事件回调函数
void on_websocket_open() {
    NC_LOG_INFO("[main] OnOpen");
    // 注意：在新的 C API 中，发送消息需要通过 BusinessManager 接口
    // 这里暂时只记录连接成功
}

void on_websocket_close() {
    NC_LOG_INFO("[main] OnClose");
}

void on_websocket_error(const char* error_msg) {
    NC_LOG_INFO("[main] OnError: %s", error_msg);
}

void on_websocket_message(const char* message) {
    NC_LOG_INFO("[main] OnMessage: %s", message);
}

int main() {
    // 初始化日志
    nc_logger::init(plog::info, "notification_center_sdk_test.log");

    // 创建 SDK 实例
    g_sdk_handle = sdk_create();
    if (!g_sdk_handle) {
        NC_LOG_ERROR("[main] Failed to create SDK instance");
        return -1;
    }

    // 初始化 SDK
    sdk_initialize_info_t init_info = {
        .log_path = "notification_center_sdk_test.log",
        .log_level = SDK_LOG_INFO
    };
    sdk_init(g_sdk_handle, &init_info);

    // 检查初始化状态
    if (!sdk_is_init(g_sdk_handle)) {
        NC_LOG_ERROR("[main] SDK initialization failed");
        sdk_destroy(g_sdk_handle);
        return -1;
    }

    // 设置 WebSocket 事件监听器
    sdk_set_websocket_event_listener(g_sdk_handle,
                                    on_websocket_message,
                                    on_websocket_open,
                                    on_websocket_close,
                                    on_websocket_error);

    // 连接到 WebSocket 服务器
    sdk_connect(g_sdk_handle, address);

    // 运行主循环
    runloop(g_sdk_handle);

    // 清理资源（实际上不会执行到这里，因为 runloop 是无限循环）
    sdk_destroy(g_sdk_handle);
    return 0;
}
