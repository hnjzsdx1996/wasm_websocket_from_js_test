package com.dji;

import com.dji.notificationcentersdk.generated.*;
import com.dji.notificationcentersdk.core.NotificationCenterClient;

class MyConnectionListener extends ConnectionListener {
    @Override
    public void OnOpen() {
        System.out.println("[Java] Connection opened.");
    }

    @Override
    public void OnClose() {
        System.out.println("[Java] Connection closed.");
    }

    @Override
    public void OnError(String error) {
        System.out.println("[Java] An error occurred: " + error);
    }

    @Override
    public void OnMessage(String message) {
        System.out.println("[Java] Received message: " + message);
    }
}

public class Main {

    public static void main(String[] args) throws InterruptedException {
        System.out.println("[Java] Starting NotificationCenterSDK Demo with Template API...");

        // 获取客户端实例（自动加载JNI库）
        NotificationCenterClient client = NotificationCenterClient.getInstance();
        System.out.println("[Java] NotificationCenterClient instance created.");

        // 创建连接监听器
        MyConnectionListener listener = new MyConnectionListener();

        // 初始化SDK
        SdkInitializeInfo initInfo = new SdkInitializeInfo("", SdkLogLevel.INFO);
        client.init(initInfo);
        System.out.println("[Java] SDK initialized successfully.");

        // 设置连接监听器
        client.setWebsocketEventListener(listener);
        System.out.println("[Java] Websocket event listener set.");

        String url = "ws://localhost:3001";
        System.out.println("[Java] Connecting to " + url + "...");
        client.connect(url);

        // 等待连接建立
        System.out.println("[Java] Waiting for connection to establish...");
        Thread.sleep(2000); // 等待2秒让连接建立

        // 监听多个设备
        String[] deviceSNs = {
            "8UUXN2D00A00VL",
            "1581F8HGD24BN0010286",
            "1581F6Q8D242100CPKTJ",
            "8UUDMAQ00A0121",
            "6QCDL820020093",
            "8PHDM8L0010322",
            "8UUDMAQ00A0138",
            "8UUDMAQ00A0047",
        };
        NotificationFrequency frequency = NotificationFrequency.PUSH_1S;
        
        // 存储所有监听ID，用于后续取消订阅
        long[] locationListenIds = new long[deviceSNs.length];
        long[] speedListenIds = new long[deviceSNs.length];
        long[] attitudeListenIds = new long[deviceSNs.length];
        long[] modeCodeListenIds = new long[deviceSNs.length];
        long[] controlCodeListenIds = new long[deviceSNs.length];
        long[] windSpeedListenIds = new long[deviceSNs.length];
        long[] batteryInfoListenIds = new long[deviceSNs.length];
        long[] deviceOsdListenIds = new long[deviceSNs.length];
        long[] droneInDockListenIds = new long[deviceSNs.length];
        long[] deviceOnlineStatusListenIds = new long[deviceSNs.length];
        long[] aircraftPayloadsListListenIds = new long[deviceSNs.length];
        long[] dockLocationListenIds = new long[deviceSNs.length];
        
        System.out.println("[Java] Starting monitoring for multiple devices using Template API...");
        
        for (int i = 0; i < deviceSNs.length; i++) {
            String deviceSN = deviceSNs[i];
            System.out.println("[Java] Starting monitoring for device " + (i + 1) + ": " + deviceSN);
            
            // 使用模板化API监听飞机位置
            locationListenIds[i] = client.ListenAircraftLocation(
                // 消息回调函数
                (AircraftLocation location) -> {
                    System.out.println("[Java] Received aircraft location message for device " + deviceSN + ":");
                    System.out.println("  - Height (椭球高度): " + location.getHeight() + " m");
                    System.out.println("  - Elevation (相对起飞点高度): " + location.getElevation() + " m");
                    System.out.println("  - Latitude (纬度): " + location.getLatitude());
                    System.out.println("  - Longitude (经度): " + location.getLongitude());
                },
                // 订阅结果回调函数
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Aircraft location subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Aircraft location subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听飞机速度
            speedListenIds[i] = client.ListenAircraftSpeed(
                (AircraftSpeed speed) -> {
                    System.out.println("[Java] Received aircraft speed message for device " + deviceSN + ":");
                    System.out.println("  - Horizontal Speed (水平速度): " + speed.getHorizontal_speed() + " m/s");
                    System.out.println("  - Vertical Speed (垂直速度): " + speed.getVertical_speed() + " m/s");
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Aircraft speed subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Aircraft speed subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听飞机姿态
            attitudeListenIds[i] = client.ListenAircraftAttitude(
                (AircraftAttitude attitude) -> {
                    System.out.println("[Java] Received aircraft attitude message for device " + deviceSN + ":");
                    System.out.println("  - Attitude Head (机头朝向角度): " + attitude.getAttitude_head() + "°");
                    System.out.println("  - Attitude Pitch (俯仰轴角度): " + attitude.getAttitude_pitch() + "°");
                    System.out.println("  - Attitude Roll (横滚轴角度): " + attitude.getAttitude_roll() + "°");
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Aircraft attitude subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Aircraft attitude subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听飞机模式代码
            modeCodeListenIds[i] = client.ListenAircraftModeCode(
                (AircraftModeCode modeCode) -> {
                    System.out.println("[Java] Received aircraft mode code message for device " + deviceSN + ":");
                    System.out.println("  - Mode Code (模式代码): " + modeCode.getMode_code());
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Aircraft mode code subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Aircraft mode code subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听飞机控制代码
            controlCodeListenIds[i] = client.ListenAircraftControlCode(
                (AircraftControlCode controlCode) -> {
                    System.out.println("[Java] Received aircraft control code message for device " + deviceSN + ":");
                    System.out.println("  - Control Mode (控制模式): " + controlCode.getControl_mode());
                    String modeDesc = "";
                    switch (controlCode.getControl_mode()) {
                        case 0: modeDesc = "航线控制-执行态"; break;
                        case 1: modeDesc = "指令控制-手动飞行"; break;
                        case 2: modeDesc = "指令控制-自动飞行"; break;
                        case 3: modeDesc = "航线控制-中断态"; break;
                        default: modeDesc = "未知模式"; break;
                    }
                    System.out.println("  - Mode Description: " + modeDesc);
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Aircraft control code subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Aircraft control code subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听飞机风速
            windSpeedListenIds[i] = client.ListenAircraftWindSpeed(
                (AircraftWindSpeed windSpeed) -> {
                    System.out.println("[Java] Received aircraft wind speed message for device " + deviceSN + ":");
                    System.out.println("  - Wind Direction (风向): " + windSpeed.getWind_direction());
                    System.out.println("  - Wind Speed (风速): " + windSpeed.getWind_speed() + " m/s");
                    
                    String directionDesc = "";
                    switch (windSpeed.getWind_direction()) {
                        case 1: directionDesc = "正北"; break;
                        case 2: directionDesc = "东北"; break;
                        case 3: directionDesc = "东"; break;
                        case 4: directionDesc = "东南"; break;
                        case 5: directionDesc = "南"; break;
                        case 6: directionDesc = "西南"; break;
                        case 7: directionDesc = "西"; break;
                        case 8: directionDesc = "西北"; break;
                        default: directionDesc = "未知方向"; break;
                    }
                    System.out.println("  - Direction Description: " + directionDesc);
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Aircraft wind speed subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Aircraft wind speed subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听飞机电池信息
            batteryInfoListenIds[i] = client.ListenAircraftBatteryInfo(
                (AircraftBatteryInfo batteryInfo) -> {
                    System.out.println("[Java] Received aircraft battery info message for device " + deviceSN + ":");
                    System.out.println("  - Capacity Percent (电池组容量百分比): " + batteryInfo.getCapacity_percent() + "%");
                    System.out.println("  - Landing Power (降落电量): " + batteryInfo.getLanding_power() + "%");
                    System.out.println("  - Remain Flight Time (剩余飞行时间): " + batteryInfo.getRemain_flight_time() + "s");
                    System.out.println("  - Return Home Power (返航电量): " + batteryInfo.getReturn_home_power() + "%");
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Aircraft battery info subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Aircraft battery info subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听设备OSD信息
            deviceOsdListenIds[i] = client.ListenDeviceOsd(
                (DeviceOsd deviceOsd) -> {
                    System.out.println("[Java] Received device OSD message for device " + deviceSN + ":");
                    System.out.println("  - Device SN (设备序列号): " + deviceOsd.getSn());
                    System.out.println("  - Mode Code (模式代码): " + deviceOsd.getHost().getMode_code());
                    System.out.println("  - Attitude Head (机头朝向角度): " + deviceOsd.getHost().getAttitude_head());
                    System.out.println("  - Attitude Pitch (俯仰轴角度): " + deviceOsd.getHost().getAttitude_pitch());
                    System.out.println("  - Attitude Roll (横滚轴角度): " + deviceOsd.getHost().getAttitude_roll());
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Device OSD subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Device OSD subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听无人机在机场状态
            droneInDockListenIds[i] = client.ListenDroneInDock(
                (DroneInDock droneInDock) -> {
                    System.out.println("[Java] Received drone in dock message for device " + deviceSN + ":");
                    System.out.println("  - Drone In Dock (无人机在机场): " + droneInDock.getDrone_in_dock());
                    String statusDesc = droneInDock.getDrone_in_dock() == 1 ? "在机场" : "不在机场";
                    System.out.println("  - Status Description: " + statusDesc);
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Drone in dock subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Drone in dock subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            
            // 使用模板化API监听飞机载荷列表信息
            aircraftPayloadsListListenIds[i] = client.ListenAircraftPayloadsList(
                (AircraftPayloadsList payloadsList) -> {
                    System.out.println("[Java] Received aircraft payloads list message for device " + deviceSN + ":");
                    StringVector payloadsListVector = payloadsList.getPayloads_list();
                    if (payloadsListVector != null && !payloadsListVector.isEmpty()) {
                        System.out.println("  - Payloads List (载荷列表):");
                        for (int j = 0; j < payloadsListVector.size(); j++) {
                            System.out.println("    [" + j + "] " + payloadsListVector.get(j));
                        }
                    } else {
                        System.out.println("  - Payloads List: 空列表或无数据");
                    }
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Aircraft payloads list subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Aircraft payloads list subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听机场位置信息
            dockLocationListenIds[i] = client.ListenDockLocation(
                (DockLocation dockLocation) -> {
                    System.out.println("[Java] Received dock location message for device " + deviceSN + ":");
                    System.out.println("  - Heading (朝向): " + dockLocation.getHeading() + "°");
                    System.out.println("  - Height (高度): " + dockLocation.getHeight() + " m");
                    System.out.println("  - Latitude (纬度): " + dockLocation.getLatitude());
                    System.out.println("  - Longitude (经度): " + dockLocation.getLongitude());
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Dock location subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Dock location subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            // 使用模板化API监听设备在线状态
            deviceOnlineStatusListenIds[i] = client.ListenDeviceOnlineStatus(
                (DeviceOnlineStatus deviceOnlineStatus) -> {
                    System.out.println("[Java] Received device online status message for device " + deviceSN + ":");
                    System.out.println("  - Device Status (设备状态): " + deviceOnlineStatus.getDevice_status());
                    String statusDesc = deviceOnlineStatus.getDevice_status() ? "在线" : "离线";
                    System.out.println("  - Status Description: " + statusDesc);
                    System.out.println("  - Device Callsign (设备呼号): " + deviceOnlineStatus.getDevice_callsign());
                    System.out.println("  - Device Model (设备型号): " + deviceOnlineStatus.getDevice_model());
                    System.out.println("  - Device Type (设备类型): " + deviceOnlineStatus.getDevice_type());
                },
                (NotificationCenterErrorCode errorCode) -> {
                    if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
                        System.out.println("[Java] Device online status subscription successful for device " + deviceSN);
                    } else {
                        System.out.println("[Java] Device online status subscription failed for device " + deviceSN + ". Error code: " + errorCode);
                    }
                },
                deviceSN,
                frequency
            );
            
            System.out.println("[Java] Started monitoring for device " + deviceSN + 
                             " with frequency " + frequency + 
                             " (Listen IDs - Location: " + locationListenIds[i] + 
                             ", Speed: " + speedListenIds[i] + 
                             ", Attitude: " + attitudeListenIds[i] + 
                             ", Mode: " + modeCodeListenIds[i] + 
                             ", Control: " + controlCodeListenIds[i] + 
                             ", Wind: " + windSpeedListenIds[i] + 
                             ", Battery: " + batteryInfoListenIds[i] + 
                             ", OSD: " + deviceOsdListenIds[i] + 
                             ", Dock: " + droneInDockListenIds[i] + 
                             ", Online: " + deviceOnlineStatusListenIds[i] + 
                             ", PayloadsList: " + aircraftPayloadsListListenIds[i] + 
                             ", DockLocation: " + dockLocationListenIds[i] + ")");
        }
        
        // 等待一段时间让自动poll处理消息
        System.out.println("[Java] Waiting for messages (auto-poll is running)...");
        System.out.println("[Java] Monitoring the following topics with Template API:");
        System.out.println("  - Aircraft Location (飞机位置)");
        System.out.println("  - Aircraft Speed (飞机速度)");
        System.out.println("  - Aircraft Attitude (飞机姿态)");
        System.out.println("  - Aircraft Mode Code (飞机模式代码)");
        System.out.println("  - Aircraft Control Code (飞机控制代码)");
        System.out.println("  - Aircraft Wind Speed (飞机风速)");
        System.out.println("  - Aircraft Battery Info (飞机电池信息)");
        System.out.println("  - Device OSD (设备OSD信息)");
        System.out.println("  - Drone In Dock (无人机在机场状态)");
        System.out.println("  - Device Online Status (设备在线状态)");
        System.out.println("  - Aircraft Payloads List (飞机载荷列表)");
        System.out.println("  - Dock Location (机场位置)");
        Thread.sleep(1000000); // 等待1000秒
        
        // 取消所有设备的监听
        System.out.println("[Java] Cancelling all subscriptions...");
        for (int i = 0; i < deviceSNs.length; i++) {
            client.cancelListen(locationListenIds[i]);
            client.cancelListen(speedListenIds[i]);
            client.cancelListen(attitudeListenIds[i]);
            client.cancelListen(modeCodeListenIds[i]);
            client.cancelListen(controlCodeListenIds[i]);
            client.cancelListen(windSpeedListenIds[i]);
            client.cancelListen(batteryInfoListenIds[i]);
            client.cancelListen(deviceOsdListenIds[i]);
            client.cancelListen(droneInDockListenIds[i]);
            client.cancelListen(deviceOnlineStatusListenIds[i]);
            client.cancelListen(aircraftPayloadsListListenIds[i]);
            client.cancelListen(dockLocationListenIds[i]);
            System.out.println("[Java] Cancelled all monitoring for device " + deviceSNs[i]);
        }

        // 继续等待一段时间
        System.out.println("[Java] Continuing to wait for cleanup...");
        Thread.sleep(5000); // 再等待5秒

        System.out.println("[Java] Demo finished.");
        System.out.println("[Java] Exiting...");
        System.exit(0);
    }
} 