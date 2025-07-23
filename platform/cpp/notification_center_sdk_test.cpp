#include "../src/SDKManager.h"
#include "../src/business_manager/BusinessManager.h"
#include "../src/business_manager/BusinessManagerDefine.h"
#include "../src/business_manager/topic_message_define/PublishDeviceOsdTopic.h"
#include "base/logger/logger.h"
#include <thread>
#include <chrono>
#include <vector>

// 全局变量用于在回调中访问
std::shared_ptr<SDKManager> g_sdk_manager = nullptr;
std::shared_ptr<BusinessManager> g_business_manager = nullptr;

// WebSocket 事件回调类
class MyWebsocketEventListener : public WebsocketEvent {
public:
    void OnOpen() override {
        NC_LOG_INFO("[C++] Connection opened.");
    }

    void OnClose() override {
        NC_LOG_INFO("[C++] Connection closed.");
    }

    void OnError(const std::string& error) override {
        NC_LOG_INFO("[C++] An error occurred: %s", error.c_str());
    }

    void OnMessage(const std::string& message) override {
        NC_LOG_INFO("[C++] Received message: %s", message.c_str());
    }
};

// 设备OSD消息回调函数
void on_device_osd_message(const DeviceOsdMsg& message) {
    NC_LOG_INFO("[C++] Received device OSD message:");
    NC_LOG_INFO("[C++]   - Device SN (设备序列号): %s", message.sn.c_str());
    NC_LOG_INFO("[C++]   - Mode Code (模式代码): %d", message.host.mode_code);
    NC_LOG_INFO("[C++]   - Attitude Head (机头朝向角度): %f", message.host.attitude_head);
    NC_LOG_INFO("[C++]   - Attitude Pitch (俯仰轴角度): %f", message.host.attitude_pitch);
    NC_LOG_INFO("[C++]   - Attitude Roll (横滚轴角度): %f", message.host.attitude_roll);
}

void on_device_osd_drone_in_dock_message(const DroneInDockMsg& message) {
    NC_LOG_INFO("[C++] Received device OSD Drone In Dock message: %d", message.drone_in_dock);
}

// 订阅结果回调函数
void on_device_osd_result(const NotificationCenterErrorCode& error_code) {
    if (error_code == NotificationCenterErrorCode_NoError) {
        NC_LOG_INFO("[C++] Device OSD subscription successful.");
    } else {
        NC_LOG_INFO("[C++] Device OSD subscription failed. Error code: %ld", error_code);
        // 可以在这里处理不同的错误码
        if (error_code == NotificationCenterErrorCode_InvalidParameter) {
            NC_LOG_INFO("[C++] Error: Invalid parameter");
        } else if (error_code == NotificationCenterErrorCode_NotConnected) {
            NC_LOG_INFO("[C++] Error: Not connected");
        } else if (error_code == NotificationCenterErrorCode_SendError) {
            NC_LOG_INFO("[C++] Error: Send error");
        } else if (error_code == NotificationCenterErrorCode_SubscribeError) {
            NC_LOG_INFO("[C++] Error: Subscribe error");
        } else {
            NC_LOG_INFO("[C++] Error: Unknown error");
        }
    }
}

[[noreturn]] void runloop() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        SDKManager::poll();
    }
}

// constexpr auto address = "wss://dev-es310-api.dbeta.me/notification/ws/v1/notifications?x-auth-token=test";
constexpr auto address = "wss://test-es310-api.dbeta.me/notification/ws/v1/notifications?x-auth-token=test";

int main() {
    // 初始化日志
    nc_logger::init(plog::info, "notification_center_sdk_test.log");

    NC_LOG_INFO("[C++] Starting NotificationCenterSDK Demo...");

    // 创建 SDK 实例
    g_sdk_manager = std::make_shared<SDKManager>();
    if (!g_sdk_manager) {
        NC_LOG_ERROR("[C++] Failed to create SDK instance");
        return -1;
    }
    NC_LOG_INFO("[C++] NotificationCenterSDK instance created.");

    // 创建连接监听器
    auto listener = std::make_shared<MyWebsocketEventListener>();

    // 初始化 SDK
    SdkInitializeInfo init_info;
    init_info.log_path = "";
    init_info.log_level = SdkLogLevel::INFO;
    g_sdk_manager->init(init_info);
    NC_LOG_INFO("[C++] SDK initialized successfully.");

    // 设置连接监听器
    g_sdk_manager->setWebsocketEventListener(listener);
    NC_LOG_INFO("[C++] Websocket event listener set.");

    NC_LOG_INFO("[C++] Connecting to %s...", address);
    g_sdk_manager->connect(address);

    // 等待连接建立
    NC_LOG_INFO("[C++] Waiting for connection to establish...");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 获取BusinessManager并演示监听功能
    g_business_manager = g_sdk_manager->getBusinessManager();
    if (!g_business_manager) {
        NC_LOG_ERROR("[C++] Failed to get BusinessManager - it is null!");
        return -1;
    }
    NC_LOG_INFO("[C++] BusinessManager obtained successfully.");

    // 监听多个设备
    std::vector<std::string> deviceSNs = {
        "8UUXN2D00A00VL",
        "1581F6Q8D242100CPKTJ",
        // "8UUDMAQ00A0121",
        // "6QCDL820020093",
        // "8PHDM8L0010322",
        // "8UUDMAQ00A0138",
        // "8UUDMAQ00A0047",
    };
    NotifactionFrequency frequency = NotifactionFrequency_Push_1s;
    
    // 演示监听设备OSD消息
    NC_LOG_INFO("[C++] Starting device OSD monitoring for multiple devices...");
    
    // 存储所有监听ID，用于后续取消订阅
    std::vector<ListenId> deviceOsdListenIds;
    
    for (size_t i = 0; i < deviceSNs.size(); i++) {
        const std::string& deviceSN = deviceSNs[i];
        NC_LOG_INFO("[C++] Starting device OSD monitoring for device %zu: %s", i + 1, deviceSN.c_str());
        ListenId listenId;

        // listenId = g_business_manager->ListenAircraftAttitude(
        //     [](const AircraftAttitudeMsg &msg)-> void {
        //         NC_LOG_INFO("[C++] ListenAircraftAttitude: %.2f, %.2f, %.2f", msg.attitude_head, msg.attitude_pitch, msg.attitude_roll);
        //     },
        //     [](const NotificationCenterErrorCode& error_code)->void {
        //         NC_LOG_INFO("[C++] ListenAircraftAttitude subscribe error_code: %d", error_code);
        //     },
        //     deviceSN,
        //     frequency
        // );

        // listenId = g_business_manager->ListenAircraftLocation(
        //     [](const AircraftLocationMsg &msg)-> void {
        //         NC_LOG_INFO("[C++] ListenAircraftLocation: h: %.2f, e: %.2f, lon: %.2f, lat: %.2f", msg.height, msg.elevation, msg.longitude, msg.latitude);
        //     },
        //     [](const NotificationCenterErrorCode &error_code)-> void {
        //         NC_LOG_INFO("[C++] ListenAircraftLocation subscribe error_code: %d", error_code);
        //     },
        //     deviceSN,
        //     frequency
        // );

        listenId = g_business_manager->ListenAircraftSpeed(
            [](const AircraftSpeedMsg &msg)-> void {
                NC_LOG_INFO("[C++] ListenAircraftSpeed: h: %.2f, v: %.2f", msg.horizontal_speed, msg.vertical_speed);
            },
            [](const NotificationCenterErrorCode &error_code)-> void {
                NC_LOG_INFO("[C++] ListenAircraftSpeed subscribe error_code: %d", error_code);
            },
            deviceSN,
            frequency
        );
        deviceOsdListenIds.push_back(listenId);

        listenId = g_business_manager->ListenAircraftModeCode(
            [](const AircraftModeCodeMsg &msg)-> void {
                NC_LOG_INFO("[C++] ListenAircraftModeCode: %d", msg.mode_code);
            },
            [](const NotificationCenterErrorCode &error_code)-> void {
                NC_LOG_INFO("[C++] ListenAircraftModeCode subscribe error_code: %d", error_code);
            },
            deviceSN,
            frequency
        );
        deviceOsdListenIds.push_back(listenId);

        listenId = g_business_manager->ListenAircraftControlCode(
            [](const AircraftControlCodeMsg &msg)-> void {
                NC_LOG_INFO("[C++] ListenAircraftControlCode: %d", msg.control_mode);
            },
            [](const NotificationCenterErrorCode &error_code)-> void {
                NC_LOG_INFO("[C++] ListenAircraftControlCode subscribe error_code: %d", error_code);
            },
            deviceSN,
            frequency
        );
        deviceOsdListenIds.push_back(listenId);

        listenId = g_business_manager->ListenAircraftControlCode(
            [](const AircraftControlCodeMsg &msg)-> void {
                NC_LOG_INFO("[C++] ListenAircraftControlCode: %d", msg.control_mode);
            },
            [](const NotificationCenterErrorCode &error_code)-> void {
                NC_LOG_INFO("[C++] ListenAircraftControlCode subscribe error_code: %d", error_code);
            },
            deviceSN,
            frequency
        );
        deviceOsdListenIds.push_back(listenId);

        listenId = g_business_manager->ListenAircraftWindSpeed(
            [](const AircraftWindSpeedMsg &msg)-> void {
                NC_LOG_INFO("[C++] ListenAircraftWindSpeed: dir: %d, speed: %.2f", msg.wind_direction, msg.wind_speed);
            },
            [](const NotificationCenterErrorCode &error_code)-> void {
                NC_LOG_INFO("[C++] ListenAircraftWindSpeed subscribe error_code: %d", error_code);
            },
            deviceSN,
            frequency
        );
        deviceOsdListenIds.push_back(listenId);

        listenId = g_business_manager->ListenAircraftBatteryInfo(
            [](const AircraftBatteryInfoMsg &msg)-> void {
                NC_LOG_INFO("[C++] ListenAircraftBatteryInfo: capacity_percent: %d, ", msg.capacity_percent);
            },
            [](const NotificationCenterErrorCode &error_code)-> void {
                NC_LOG_INFO("[C++] ListenAircraftBatteryInfo subscribe error_code: %d", error_code);
            },
            deviceSN,
            frequency
        );
        deviceOsdListenIds.push_back(listenId);

        // listenId = g_business_manager->ListenDeviceOsd(
        //     on_device_osd_message,
        //     on_device_osd_result,
        //     deviceSN,
        //     frequency
        // );
        //
        // deviceOsdListenIds.push_back(listenId);
        //
        // listenId = g_business_manager->ListenDroneInDock(
        //     on_device_osd_drone_in_dock_message,
        //     on_device_osd_result,
        //     deviceSN,
        //     frequency
        // );
        // deviceOsdListenIds.push_back(listenId);
        
        NC_LOG_INFO("[C++] Started monitoring device OSD for device %s with frequency %d (Listen ID: %ld)", deviceSN.c_str(), frequency, listenId);
    }
    
    // 等待一段时间让自动poll处理消息
    NC_LOG_INFO("[C++] Waiting for messages (auto-poll is running)...");


    runloop();

    // 下面代码先不执行了
    
    // 取消所有设备的OSD监听
    for (size_t i = 0; i < deviceSNs.size(); i++) {
        g_business_manager->CancelObserve(deviceOsdListenIds[i]);
        NC_LOG_INFO("[C++] Cancelled device OSD monitoring for device %s", deviceSNs[i].c_str());
    }

    // 继续等待一段时间
    NC_LOG_INFO("[C++] Continuing to wait for cleanup...");
    std::this_thread::sleep_for(std::chrono::seconds(5)); // 再等待5秒

    NC_LOG_INFO("[C++] Demo finished.");
    
    // 清理资源并退出
    NC_LOG_INFO("[C++] Cleaning up resources...");
    
    // 清理全局变量
    g_business_manager.reset();
    g_sdk_manager.reset();
    
    NC_LOG_INFO("[C++] Exiting...");
    return 0;
}
