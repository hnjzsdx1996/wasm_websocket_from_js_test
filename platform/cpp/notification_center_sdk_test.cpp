#include "../src/SDKManager.h"
#include "../src/business_manager/BusinessManager.h"
#include "../src/business_manager/BusinessManagerDefine.h"
#include "../src/business_manager/topic_message_define/PublishDeviceOsdTopic.h"
#include "base/logger/logger.h"
#include <thread>
#include <chrono>
#include <vector>
#include <atomic>
#include <mutex> // Added for mutex

#include "../src/base/async/thread_pool_executor.h"

constexpr auto address = "wss://dev-es310-api.dbeta.me/notification/ws/v1/notifications?x-auth-token=test";
// constexpr auto address = "wss://test-es310-api.dbeta.me/notification/ws/v1/notifications?x-auth-token=test";
// constexpr auto address = "ws://localhost:3001";

// 全局变量用于在回调中访问
std::shared_ptr<SDKManager> g_sdk_manager = nullptr;
std::shared_ptr<BusinessManager> g_business_manager = nullptr;
std::atomic<bool> g_connection_established(false);

// 存储所有监听ID，用于后续取消订阅
std::vector<ListenId> g_device_osd_listen_ids;
std::mutex g_listen_ids_mutex;

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

// 取消所有订阅的函数
void cancel_all_subscriptions() {
    if (!g_business_manager) {
        NC_LOG_WARN("[C++] BusinessManager is null, cannot cancel subscriptions");
        return;
    }

    std::lock_guard<std::mutex> lock(g_listen_ids_mutex);
    if (g_device_osd_listen_ids.empty()) {
        NC_LOG_INFO("[C++] No active subscriptions to cancel");
        return;
    }

    NC_LOG_INFO("[C++] Cancelling %zu active subscriptions...", g_device_osd_listen_ids.size());
    for (const auto& listenId : g_device_osd_listen_ids) {
        g_business_manager->CancelObserve(listenId);
    }
    g_device_osd_listen_ids.clear();
    NC_LOG_INFO("[C++] All subscriptions cancelled successfully");
}

// 开始监听设备消息的函数
void start_device_monitoring() {
    if (!g_business_manager) {
        NC_LOG_ERROR("[C++] BusinessManager is null, cannot start monitoring");
        return;
    }

    // 先取消之前的订阅
    cancel_all_subscriptions();

    // 监听多个设备
    std::vector<std::string> deviceSNs = {
        "8UUXN2D00A00VL",
        "1581F6Q8D242100CPKTJ",
        "1581F8HHD24B10010084",
        "8UUDMAQ00A0121",
        "6QCDL820020093",
        "8PHDM8L0010322",
        "8UUDMAQ00A0138",
        "8UUDMAQ00A0047",
        "8UUDMAQ00A0152",
        "8UUDMAQ00A0076",
    };
    NotifactionFrequency frequency = NotifactionFrequency_Push_1s;
    
    // 演示监听设备OSD消息
    NC_LOG_INFO("[C++] Starting device OSD monitoring for multiple devices...");
    
    std::lock_guard<std::mutex> lock(g_listen_ids_mutex);
    
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
        g_device_osd_listen_ids.push_back(listenId);

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
        g_device_osd_listen_ids.push_back(listenId);

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
        g_device_osd_listen_ids.push_back(listenId);

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
        g_device_osd_listen_ids.push_back(listenId);

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
        g_device_osd_listen_ids.push_back(listenId);

        listenId = g_business_manager->ListenDeviceOsd(
            on_device_osd_message,
            on_device_osd_result,
            deviceSN,
            frequency
        );

        g_device_osd_listen_ids.push_back(listenId);

        listenId = g_business_manager->ListenDockLocation(
            [](const DockLocationMsg &msg)-> void {
                NC_LOG_INFO("[C++] ListenDockLocation: heading: %.2f, height: %.2f, lon: %.2f, lat: %.2f", msg.heading, msg.height, msg.longitude, msg.latitude);
            },
            [](const NotificationCenterErrorCode &error_code)-> void {
                NC_LOG_INFO("[C++] ListenDockLocation subscribe error_code: %d", error_code);
            },
            deviceSN,
            frequency
        );
        g_device_osd_listen_ids.push_back(listenId);

        listenId = g_business_manager->ListenDroneInDock(
            on_device_osd_drone_in_dock_message,
            on_device_osd_result,
            deviceSN,
            frequency
        );
        g_device_osd_listen_ids.push_back(listenId);
        
        NC_LOG_INFO("[C++] Started monitoring device OSD for device %s with frequency %d (Listen ID: %ld)", deviceSN.c_str(), frequency, listenId);
    }
    
    NC_LOG_INFO("[C++] Device monitoring started successfully. Total subscriptions: %zu", g_device_osd_listen_ids.size());
}

// WebSocket 事件回调类
class MyWebsocketEventListener : public WebsocketEvent {
private:
    std::atomic<bool> m_has_started_monitoring{false};
    std::atomic<bool> m_is_reconnecting{false};
    std::thread m_reconnect_thread;

public:
    ~MyWebsocketEventListener() {
        if (m_reconnect_thread.joinable()) {
            m_reconnect_thread.join();
        }
    }

    void OnOpen() override {
        NC_LOG_INFO("[C++] Connection opened.");
        
        // 防止重复启动监听
        bool expected = false;
        if (m_has_started_monitoring.compare_exchange_strong(expected, true)) {
            NC_LOG_INFO("[C++] Starting device monitoring after connection established...");
            
            // 获取BusinessManager
            g_business_manager = g_sdk_manager->getBusinessManager();
            if (!g_business_manager) {
                NC_LOG_ERROR("[C++] Failed to get BusinessManager - it is null!");
                return;
            }
            NC_LOG_INFO("[C++] BusinessManager obtained successfully.");
            
            // 开始监听设备消息
            start_device_monitoring();
            
            g_connection_established = true;
            NC_LOG_INFO("[C++] Connection established and monitoring started successfully.");
        } else {
            NC_LOG_INFO("[C++] Monitoring already started, skipping...");
        }
    }

    void OnClose() override {
        NC_LOG_INFO("[C++] Connection closed. Current reconnecting state: %s", m_is_reconnecting.load() ? "true" : "false");
        g_connection_established = false;
        
        // 启动重连逻辑
        schedule_reconnect();
    }

    void OnError(const std::string& error) override {
        NC_LOG_INFO("[C++] An error occurred: %s. Current reconnecting state: %s", error.c_str(), m_is_reconnecting.load() ? "true" : "false");
        g_connection_established = false;
        
        // 启动重连逻辑
        schedule_reconnect();
    }

    void OnMessage(const std::string& message) override {
        // NC_LOG_INFO("[C++] Received message: %s", message.c_str());
    }

private:
        void schedule_reconnect() {
        // 防止重复重连
        bool expected = false;
        if (!m_is_reconnecting.compare_exchange_strong(expected, true)) {
            NC_LOG_INFO("[C++] Reconnection already in progress, skipping...");
            return;
        }

        NC_LOG_INFO("[C++] Starting reconnection process...");

        // 取消之前的订阅
        cancel_all_subscriptions();

        // 在独立线程中执行重连
        if (m_reconnect_thread.joinable()) {
            m_reconnect_thread.join();
        }

        m_reconnect_thread = std::thread([this]() {
            NC_LOG_INFO("[C++] Waiting 10 seconds before attempting to reconnect...");
            std::this_thread::sleep_for(std::chrono::seconds(10));
            
            // 在主线程中执行重连
            ThreadPoolExecutor::Main().post([this]()->void {
                NC_LOG_INFO("[C++] Executing reconnection in main thread...");
                
                if (g_sdk_manager) {
                    NC_LOG_INFO("[C++] Attempting to reconnect...");
                    
                    // 重置监听标志，允许重新建立监听
                    m_has_started_monitoring = false;
                    
                    // 确保连接状态已重置
                    g_connection_established = false;
                    
                    // 等待一小段时间确保之前的连接完全关闭
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    
                    // 执行重连
                    g_sdk_manager->connect(address);
                    
                    NC_LOG_INFO("[C++] Reconnection request sent, waiting for connection events...");
                    
                    // 设置一个超时检查
                    ThreadPoolExecutor::Main().post([this]()->void {
                        std::this_thread::sleep_for(std::chrono::seconds(5));
                        if (!g_connection_established) {
                            NC_LOG_WARN("[C++] Reconnection timeout - no connection established within 5 seconds");
                        }
                    });
                } else {
                    NC_LOG_ERROR("[C++] SDKManager is null during reconnection!");
                }
                
                m_is_reconnecting = false;
            });
        });
    }
};

[[noreturn]] void runloop() {
    NC_LOG_INFO("[C++] Starting main event loop...");
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        size_t tasks_processed = SDKManager::poll();
        if (tasks_processed > 0) {
            NC_LOG_DEBUG("[C++] Processed %zu tasks in main thread", tasks_processed);
        }
    }
}


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

    // 等待一段时间让自动poll处理消息
    NC_LOG_INFO("[C++] Waiting for messages (auto-poll is running)...");

    runloop();

    // 下面代码先不执行了
    
    // 取消所有设备的OSD监听
    // for (size_t i = 0; i < deviceSNs.size(); i++) {
    //     g_business_manager->CancelObserve(deviceOsdListenIds[i]);
    //     NC_LOG_INFO("[C++] Cancelled device OSD monitoring for device %s", deviceSNs[i].c_str());
    // }

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
