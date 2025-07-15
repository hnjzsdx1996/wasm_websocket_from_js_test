#include "SDKManager.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "logger.h"

// 简单的消息回调
void on_message(const std::string& msg) {
    std::cout << "[收到消息] " << msg << std::endl;
}

// 连接打开回调
void on_open() {
    std::cout << "[连接已打开]" << std::endl;
}

// 连接关闭回调
void on_close() {
    std::cout << "[连接已关闭]" << std::endl;
}

// 错误回调
void on_error(const std::string& err) {
    std::cout << "[错误] " << err << std::endl;
}

int main() {
    nc_logger::init(plog::debug, "example_cpp.log");
    nc_logger::set_level(plog::debug);

    std::cout << "=== C++ WebSocket SDK Example ===" << std::endl;

    // 创建 SDKManager 实例
    SDKManager sdk;
    sdk.configure(R"({"sn":"1234567890","ping_pong_interval":100})");

    // 注册回调
    auto strong_websocket_holder = sdk.getWebSocketHolder().lock();
    if (strong_websocket_holder == nullptr) {
        return -1;
    }
    strong_websocket_holder->setOnMessage(on_message);
    strong_websocket_holder->setOnOpen(on_open);
    strong_websocket_holder->setOnClose(on_close);
    strong_websocket_holder->setOnError(on_error);

    // 连接 WebSocket 服务器
    std::string url = "wss://echo.websocket.org";
    std::cout << "连接到: " << url << std::endl;
    sdk.connect(url);

    // 等待连接建立
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // 发送一条消息
    std::string msg = "Hello from C++ SDK!";
    std::cout << "发送消息: " << msg << std::endl;
    sdk.send(msg);

    // 等待消息回显
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 关闭连接
    std::cout << "关闭连接..." << std::endl;
    sdk.close();

    // 等待关闭回调
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::cout << "=== 示例结束 ===" << std::endl;
    return 0;
} 