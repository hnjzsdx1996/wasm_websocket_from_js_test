#include "SDKManager.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "=== WASM WebSocket SDK 测试程序 ===" << std::endl;
    
    // 测试C++原生WebSocket
    std::cout << "\n1. 测试C++原生WebSocket:" << std::endl;
    SDKManager sdkCpp(WebSocketType::CPP);
    sdkCpp.createWebSocket("ws://echo.websocket.org");
    
    // 等待连接
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    if (sdkCpp.isWebSocketOpen()) {
        sdkCpp.sendWebSocketMessage("Hello from C++ WebSocket!");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "最后收到的消息: " << sdkCpp.getLastMessage() << std::endl;
    }
    
    sdkCpp.closeWebSocket();
    
    // 测试切换到JS WebSocket（在Web环境下）
    std::cout << "\n2. 测试JavaScript WebSocket:" << std::endl;
    SDKManager sdkJs(WebSocketType::JS);
    
    // 注意：在纯C++环境下，JS WebSocket需要JavaScript环境支持
    // 这里只是演示API调用
    sdkJs.createWebSocket("ws://echo.websocket.org");
    
    std::cout << "\n=== 测试完成 ===" << std::endl;
    return 0;
} 