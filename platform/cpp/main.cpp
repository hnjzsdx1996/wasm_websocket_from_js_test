#include "SDKManager.h"
#include <iostream>
#include <thread>
#include <chrono>

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
    std::cout << "=== C++ WebSocket SDK Example ===" << std::endl;

    // 创建 SDKManager 实例
    SDKManager sdk;

    // 注册回调
    sdk.setMessageCallback(on_message);
    sdk.setOpenCallback(on_open);
    sdk.setCloseCallback(on_close);
    sdk.setErrorCallback(on_error);

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