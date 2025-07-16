#include "SDKManager.h"
#include <iostream>
#include <thread>
#include <chrono>

#include "base/logger/logger.h"

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



    return 0;
} 