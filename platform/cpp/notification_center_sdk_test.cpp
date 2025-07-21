#include "SDKManager.h"
#include "async/thread_pool_executor.h"
#include "base/logger/logger.h"

[[noreturn]] void runloop(const std::shared_ptr<SDKManager> &sdk_manager) {
    while (true) {
        sleep(1);
        sdk_manager->poll();
    }
}

constexpr auto address = "wss://echo.websocket.org";

std::shared_ptr<SDKManager> sdk_manager;

int main() {
    nc_logger::init(plog::info, "notification_center_sdk_test.log");

    sdk_manager = std::make_shared<SDKManager>();

    auto strong_websocket_holder = sdk_manager->getWebSocketHolder().lock();
    strong_websocket_holder->setOnOpen([]()-> void {
        NC_LOG_INFO("[main] OnOpen");
        // 启动发送数据
        ThreadPoolExecutor::Worker()->Post([]()-> void {
            auto strong_websocket_holder = sdk_manager->getWebSocketHolder().lock();
            strong_websocket_holder->getWebSocket()->send("Hello, World!");
        });
    });
    strong_websocket_holder->setOnClose([]()-> void {
        NC_LOG_INFO("[main] OnClose");
    });
    strong_websocket_holder->setOnError([](const std::string &error_msg)-> void {
        NC_LOG_INFO("[main] OnError: %s", error_msg.c_str());
    });
    strong_websocket_holder->setOnMessage([](const std::string &message)-> void {
        NC_LOG_INFO("[main] OnMessage: %s", message.c_str());
    });
    strong_websocket_holder->getWebSocket()->connect(address);


    runloop(sdk_manager);

    return 0;
}
