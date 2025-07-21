#include "websocket/CppWebSocket.h"
#include "base/logger/logger.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <csignal>
#include <atomic>

std::atomic<bool> interrupted(false);

void signal_handler(int signal) {
    interrupted = true;
}

int main() {
    signal(SIGINT, signal_handler);

#ifdef ENABLE_LIBWEBSOCKETS
    nc_logger::init(plog::debug, "libwebsockets_test.log");
    nc_logger::set_level(plog::debug);    

    CppWebSocket ws;

    // 设置回调函数
    ws.setOnOpen([]() {
        NC_LOG_INFO("WebSocket 连接已打开");
    });
    ws.setOnMessage([](const std::string& msg) {
        NC_LOG_INFO("收到消息: %s", msg.c_str());
    });
    ws.setOnError([](const std::string& errMsg) {
        NC_LOG_ERROR("发生错误: %s", errMsg.c_str());
        interrupted = true;
    });
    ws.setOnClose([]() {
        NC_LOG_INFO("WebSocket 连接已关闭");
        interrupted = true;
    });

    // 使用一个可靠的 WSS echo 服务器
    std::string url = "ws://echo.websocket.events";
    ws.connect(url);

    // 每秒发送一条消息，直到被中断
    int count = 0;
    while (!interrupted) {
        if (ws.isOpen()) {
            std::string msg = "Hello from NotificationCenterSDK! Count: " + std::to_string(count++);
            ws.send(msg);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    // 关闭连接
    if (ws.isOpen()) {
        ws.close();
    }
    
    NC_LOG_INFO("测试程序结束");
#else
    std::cout << "ENABLE_LIBWEBSOCKETS 未启用，无法测试 WebSocket 功能。" << std::endl;
#endif
    return 0;
}
