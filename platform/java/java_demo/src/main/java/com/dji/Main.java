package com.dji;

import com.dji.notificationcentersdk.generated.*;
import com.dji.notificationcentersdk.core.NotificationCenterSDK;
import com.dji.notificationcentersdk.core.listeners.*;

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

// 用户友好的监听器实现 - 设备OSD
class MyDeviceOsdListener implements DeviceOsdListener {
    @Override
    public void onDeviceOsdReceived(String deviceSn, DeviceOsd message) {
        System.out.println("[Java] Received device OSD message for device " + deviceSn + ":");
        System.out.println("  - Device SN (设备序列号): " + message.getSn());
        System.out.println("  - Mode Code (模式代码): " + message.getHost().getMode_code());
        System.out.println("  - Attitude Head (机头朝向角度): " + message.getHost().getAttitude_head());
        System.out.println("  - Attitude Pitch (俯仰轴角度): " + message.getHost().getAttitude_pitch());
        System.out.println("  - Attitude Roll (横滚轴角度): " + message.getHost().getAttitude_roll());
    }
    
    @Override
    public void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Device OSD subscription successful for device " + deviceSn);
        } else {
            System.out.println("[Java] Device OSD subscription failed for device " + deviceSn + ". Error code: " + errorCode);
        }
    }
}

// 用户友好的监听器实现 - 飞机电池信息
class MyAircraftBatteryInfoListener implements AircraftBatteryInfoListener {
    @Override
    public void onAircraftBatteryInfoReceived(String deviceSn, AircraftBatteryInfo message) {
        System.out.println("[Java] Received aircraft battery info message for device " + deviceSn + ":");
        System.out.println("  - Capacity Percent (电池组容量百分比): " + message.getCapacity_percent() + "%");
        System.out.println("  - Landing Power (降落电量): " + message.getLanding_power() + "%");
        System.out.println("  - Remain Flight Time (剩余飞行时间): " + message.getRemain_flight_time() + "s");
        System.out.println("  - Return Home Power (返航电量): " + message.getReturn_home_power() + "%");
        System.out.println("  - Battery Count (电池数量): " + "需要手动处理vector访问");
    }
    
    @Override
    public void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft battery info subscription successful for device " + deviceSn);
        } else {
            System.out.println("[Java] Aircraft battery info subscription failed for device " + deviceSn + ". Error code: " + errorCode);
        }
    }
}

// 用户友好的监听器实现 - 飞机控制代码
class MyAircraftControlCodeListener implements AircraftControlCodeListener {
    @Override
    public void onAircraftControlCodeReceived(String deviceSn, AircraftControlCode message) {
        System.out.println("[Java] Received aircraft control code message for device " + deviceSn + ":");
        System.out.println("  - Control Mode (控制模式): " + message.getControl_mode());
        String modeDesc = "";
        switch (message.getControl_mode()) {
            case 0: modeDesc = "航线控制-执行态"; break;
            case 1: modeDesc = "指令控制-手动飞行"; break;
            case 2: modeDesc = "指令控制-自动飞行"; break;
            case 3: modeDesc = "航线控制-中断态"; break;
            default: modeDesc = "未知模式"; break;
        }
        System.out.println("  - Mode Description: " + modeDesc);
    }
    
    @Override
    public void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft control code subscription successful for device " + deviceSn);
        } else {
            System.out.println("[Java] Aircraft control code subscription failed for device " + deviceSn + ". Error code: " + errorCode);
        }
    }
}

// 用户友好的监听器实现 - 飞机风速
class MyAircraftWindSpeedListener implements AircraftWindSpeedListener {
    @Override
    public void onAircraftWindSpeedReceived(String deviceSn, AircraftWindSpeed message) {
        System.out.println("[Java] Received aircraft wind speed message for device " + deviceSn + ":");
        System.out.println("  - Wind Direction (风向): " + message.getWind_direction());
        System.out.println("  - Wind Speed (风速): " + message.getWind_speed() + " m/s");
        
        String directionDesc = "";
        switch (message.getWind_direction()) {
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
    }
    
    @Override
    public void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft wind speed subscription successful for device " + deviceSn);
        } else {
            System.out.println("[Java] Aircraft wind speed subscription failed for device " + deviceSn + ". Error code: " + errorCode);
        }
    }
}

// 用户友好的监听器实现 - 飞机模式代码
class MyAircraftModeCodeListener implements AircraftModeCodeListener {
    @Override
    public void onAircraftModeCodeReceived(String deviceSn, AircraftModeCode message) {
        System.out.println("[Java] Received aircraft mode code message for device " + deviceSn + ":");
        System.out.println("  - Mode Code (模式代码): " + message.getMode_code());
    }
    
    @Override
    public void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft mode code subscription successful for device " + deviceSn);
        } else {
            System.out.println("[Java] Aircraft mode code subscription failed for device " + deviceSn + ". Error code: " + errorCode);
        }
    }
}

// 用户友好的监听器实现 - 无人机在机场状态
class MyDroneInDockListener implements DroneInDockListener {
    @Override
    public void onDroneInDockReceived(String deviceSn, DroneInDock message) {
        System.out.println("[Java] Received drone in dock message for device " + deviceSn + ":");
        System.out.println("  - Drone In Dock (无人机在机场): " + message.getDrone_in_dock());
        String statusDesc = message.getDrone_in_dock() == 1 ? "在机场" : "不在机场";
        System.out.println("  - Status Description: " + statusDesc);
    }
    
    @Override
    public void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Drone in dock subscription successful for device " + deviceSn);
        } else {
            System.out.println("[Java] Drone in dock subscription failed for device " + deviceSn + ". Error code: " + errorCode);
        }
    }
}

// 用户友好的监听器实现 - 飞机位置
class MyAircraftLocationListener implements AircraftLocationListener {
    @Override
    public void onAircraftLocationReceived(String deviceSn, AircraftLocation message) {
        System.out.println("[Java] Received aircraft location message for device " + deviceSn + ":");
        System.out.println("  - Height (椭球高度): " + message.getHeight() + " m");
        System.out.println("  - Elevation (相对起飞点高度): " + message.getElevation() + " m");
        System.out.println("  - Latitude (纬度): " + message.getLatitude());
        System.out.println("  - Longitude (经度): " + message.getLongitude());
    }
    
    @Override
    public void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft location subscription successful for device " + deviceSn);
        } else {
            System.out.println("[Java] Aircraft location subscription failed for device " + deviceSn + ". Error code: " + errorCode);
        }
    }
}

// 用户友好的监听器实现 - 飞机姿态
class MyAircraftAttitudeListener implements AircraftAttitudeListener {
    @Override
    public void onAircraftAttitudeReceived(String deviceSn, AircraftAttitude message) {
        System.out.println("[Java] Received aircraft attitude message for device " + deviceSn + ":");
        System.out.println("  - Attitude Head (机头朝向角度): " + message.getAttitude_head() + "°");
        System.out.println("  - Attitude Pitch (俯仰轴角度): " + message.getAttitude_pitch() + "°");
        System.out.println("  - Attitude Roll (横滚轴角度): " + message.getAttitude_roll() + "°");
    }
    
    @Override
    public void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft attitude subscription successful for device " + deviceSn);
        } else {
            System.out.println("[Java] Aircraft attitude subscription failed for device " + deviceSn + ". Error code: " + errorCode);
        }
    }
}

// 用户友好的监听器实现 - 飞机速度
class MyAircraftSpeedListener implements AircraftSpeedListener {
    @Override
    public void onAircraftSpeedReceived(String deviceSn, AircraftSpeed message) {
        System.out.println("[Java] Received aircraft speed message for device " + deviceSn + ":");
        System.out.println("  - Horizontal Speed (水平速度): " + message.getHorizontal_speed() + " m/s");
        System.out.println("  - Vertical Speed (垂直速度): " + message.getVertical_speed() + " m/s");
    }
    
    @Override
    public void onSubscribeResult(String deviceSn, NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft speed subscription successful for device " + deviceSn);
        } else {
            System.out.println("[Java] Aircraft speed subscription failed for device " + deviceSn + ". Error code: " + errorCode);
        }
    }
}

public class Main {

    public static void main(String[] args) throws InterruptedException {
        System.out.println("[Java] Starting NotificationCenterSDK Demo with User-Friendly API...");

        // 获取SDK实例（自动加载JNI库）
        NotificationCenterSDK sdk = NotificationCenterSDK.getInstance();
        System.out.println("[Java] NotificationCenterSDK instance created.");

        // 创建连接监听器
        MyConnectionListener listener = new MyConnectionListener();

        // 初始化SDK
        SdkInitializeInfo initInfo = new SdkInitializeInfo("", SdkLogLevel.INFO);
        sdk.init(initInfo);
        System.out.println("[Java] SDK initialized successfully.");

        // 设置连接监听器
        sdk.setWebsocketEventListener(listener);
        System.out.println("[Java] Websocket event listener set.");

        String url = "ws://localhost:3001";
        System.out.println("[Java] Connecting to " + url + "...");
        sdk.connect(url);

        // 等待连接建立
        System.out.println("[Java] Waiting for connection to establish...");
        Thread.sleep(2000); // 等待2秒让连接建立

        // 创建用户友好的监听器实例
        MyDeviceOsdListener deviceOsdListener = new MyDeviceOsdListener();
        MyAircraftBatteryInfoListener batteryInfoListener = new MyAircraftBatteryInfoListener();
        MyAircraftControlCodeListener controlCodeListener = new MyAircraftControlCodeListener();
        MyAircraftWindSpeedListener windSpeedListener = new MyAircraftWindSpeedListener();
        MyAircraftModeCodeListener modeCodeListener = new MyAircraftModeCodeListener();
        MyDroneInDockListener droneInDockListener = new MyDroneInDockListener();
        MyAircraftLocationListener locationListener = new MyAircraftLocationListener();
        MyAircraftAttitudeListener attitudeListener = new MyAircraftAttitudeListener();
        MyAircraftSpeedListener speedListener = new MyAircraftSpeedListener();

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
        long[] deviceOsdListenIds = new long[deviceSNs.length];
        long[] batteryInfoListenIds = new long[deviceSNs.length];
        long[] controlCodeListenIds = new long[deviceSNs.length];
        long[] windSpeedListenIds = new long[deviceSNs.length];
        long[] modeCodeListenIds = new long[deviceSNs.length];
        long[] droneInDockListenIds = new long[deviceSNs.length];
        long[] locationListenIds = new long[deviceSNs.length];
        long[] attitudeListenIds = new long[deviceSNs.length];
        long[] speedListenIds = new long[deviceSNs.length];
        
        System.out.println("[Java] Starting monitoring for multiple devices using User-Friendly API...");
        
        for (int i = 0; i < deviceSNs.length; i++) {
            String deviceSN = deviceSNs[i];
            System.out.println("[Java] Starting monitoring for device " + (i + 1) + ": " + deviceSN);
            
            // 使用用户友好的API监听各种消息
            deviceOsdListenIds[i] = sdk.listenDeviceOsd(deviceSN, frequency, deviceOsdListener);
            batteryInfoListenIds[i] = sdk.listenAircraftBatteryInfo(deviceSN, frequency, batteryInfoListener);
            controlCodeListenIds[i] = sdk.listenAircraftControlCode(deviceSN, frequency, controlCodeListener);
            windSpeedListenIds[i] = sdk.listenAircraftWindSpeed(deviceSN, frequency, windSpeedListener);
            modeCodeListenIds[i] = sdk.listenAircraftModeCode(deviceSN, frequency, modeCodeListener);
            droneInDockListenIds[i] = sdk.listenDroneInDock(deviceSN, frequency, droneInDockListener);
            locationListenIds[i] = sdk.listenAircraftLocation(deviceSN, frequency, locationListener);
            attitudeListenIds[i] = sdk.listenAircraftAttitude(deviceSN, frequency, attitudeListener);
            speedListenIds[i] = sdk.listenAircraftSpeed(deviceSN, frequency, speedListener);
            
            System.out.println("[Java] Started monitoring for device " + deviceSN + 
                             " with frequency " + frequency + 
                             " (Listen IDs - OSD: " + deviceOsdListenIds[i] + 
                             ", Battery: " + batteryInfoListenIds[i] + 
                             ", Control: " + controlCodeListenIds[i] + 
                             ", Wind: " + windSpeedListenIds[i] + 
                             ", Mode: " + modeCodeListenIds[i] + 
                             ", Dock: " + droneInDockListenIds[i] + 
                             ", Location: " + locationListenIds[i] + 
                             ", Attitude: " + attitudeListenIds[i] + 
                             ", Speed: " + speedListenIds[i] + ")");
        }
        
        // 等待一段时间让自动poll处理消息
        System.out.println("[Java] Waiting for messages (auto-poll is running)...");
        System.out.println("[Java] Monitoring the following topics with User-Friendly API:");
        System.out.println("  - Device OSD (设备OSD信息)");
        System.out.println("  - Aircraft Battery Info (飞机电池信息)");
        System.out.println("  - Aircraft Control Code (飞机控制代码)");
        System.out.println("  - Aircraft Wind Speed (飞机风速)");
        System.out.println("  - Aircraft Mode Code (飞机模式代码)");
        System.out.println("  - Drone In Dock (无人机在机场状态)");
        System.out.println("  - Aircraft Location (飞机位置)");
        System.out.println("  - Aircraft Attitude (飞机姿态)");
        System.out.println("  - Aircraft Speed (飞机速度)");
        Thread.sleep(1000000); // 等待1000秒
        
        // 取消所有设备的监听
        System.out.println("[Java] Cancelling all subscriptions...");
        for (int i = 0; i < deviceSNs.length; i++) {
            sdk.cancelListen(deviceOsdListenIds[i]);
            sdk.cancelListen(batteryInfoListenIds[i]);
            sdk.cancelListen(controlCodeListenIds[i]);
            sdk.cancelListen(windSpeedListenIds[i]);
            sdk.cancelListen(modeCodeListenIds[i]);
            sdk.cancelListen(droneInDockListenIds[i]);
            sdk.cancelListen(locationListenIds[i]);
            sdk.cancelListen(attitudeListenIds[i]);
            sdk.cancelListen(speedListenIds[i]);
            System.out.println("[Java] Cancelled all monitoring for device " + deviceSNs[i]);
        }

        // 继续等待一段时间
        System.out.println("[Java] Continuing to wait for cleanup...");
        Thread.sleep(5000); // 再等待5秒

        System.out.println("[Java] Demo finished.");
        
        // 清理资源并退出
        System.out.println("[Java] Cleaning up resources...");
        try {
            // 获取底层SDK管理器并清理
            SDKManager sdkManager = sdk.getSDKManager();
            if (sdkManager != null) {
                sdkManager.delete();
                System.out.println("[Java] SDKManager cleaned up.");
            }
        } catch (Exception e) {
            System.err.println("[Java] Error during cleanup: " + e.getMessage());
        }
        
        System.out.println("[Java] Exiting...");
        System.exit(0);
    }
} 