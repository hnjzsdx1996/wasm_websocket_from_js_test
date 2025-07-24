package com.dji;

import com.dji.notificationcentersdk.generated.*;
import com.dji.notificationcentersdk.core.NotificationCenterSDK;

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

class MyDeviceOsdMessageCallback extends DeviceOsdCallback {
    @Override
    public void invoke(DeviceOsd message) {
        System.out.println("[Java] Received device OSD message:");
        System.out.println("  - Device SN (设备序列号): " + message.getSn());
        System.out.println("  - Mode Code (模式代码): " + message.getHost().getMode_code());
        System.out.println("  - Attitude Head (机头朝向角度): " + message.getHost().getAttitude_head());
        System.out.println("  - Attitude Pitch (俯仰轴角度): " + message.getHost().getAttitude_pitch());
        System.out.println("  - Attitude Roll (横滚轴角度): " + message.getHost().getAttitude_roll());
    }
}

// 新增：飞机电池信息回调类
class MyAircraftBatteryInfoCallback extends AircraftBatteryInfoCallback {
    @Override
    public void invoke(AircraftBatteryInfo message) {
        System.out.println("[Java] Received aircraft battery info message:");
        System.out.println("  - Capacity Percent (电池组容量百分比): " + message.getCapacity_percent() + "%");
        System.out.println("  - Landing Power (降落电量): " + message.getLanding_power() + "%");
        System.out.println("  - Remain Flight Time (剩余飞行时间): " + message.getRemain_flight_time() + "s");
        System.out.println("  - Return Home Power (返航电量): " + message.getReturn_home_power() + "%");
        // 注意：由于SWIG的限制，std::vector可能无法直接使用size()和get()方法
        // 这里暂时跳过电池详细信息的打印
        System.out.println("  - Battery Count (电池数量): 需要手动处理vector访问");
        
        // 打印每个电池的详细信息（暂时注释掉）
        /*
        for (int i = 0; i < message.getBatteries().size(); i++) {
            BatteryInfo battery = message.getBatteries().get(i);
            System.out.println("    Battery " + (i + 1) + ":");
            System.out.println("      - SN: " + battery.getSn());
            System.out.println("      - Capacity: " + battery.getCapacity_percent() + "%");
            System.out.println("      - Voltage: " + battery.getVoltage() + "mV");
            System.out.println("      - Temperature: " + battery.getTemperature() + "°C");
            System.out.println("      - Loop Times: " + battery.getLoop_times());
            System.out.println("      - Type: " + battery.getType());
        }
        */
    }
}

// 新增：飞机控制代码回调类
class MyAircraftControlCodeCallback extends AircraftControlCodeCallback {
    @Override
    public void invoke(AircraftControlCode message) {
        System.out.println("[Java] Received aircraft control code message:");
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
}

// 新增：飞机风速回调类
class MyAircraftWindSpeedCallback extends AircraftWindSpeedCallback {
    @Override
    public void invoke(AircraftWindSpeed message) {
        System.out.println("[Java] Received aircraft wind speed message:");
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
}

// 新增：飞机模式代码回调类
class MyAircraftModeCodeCallback extends AircraftModeCodeCallback {
    @Override
    public void invoke(AircraftModeCode message) {
        System.out.println("[Java] Received aircraft mode code message:");
        System.out.println("  - Mode Code (模式代码): " + message.getMode_code());
    }
}

// 新增：无人机在机场状态回调类
class MyDroneInDockCallback extends DroneInDockCallback {
    @Override
    public void invoke(DroneInDock message) {
        System.out.println("[Java] Received drone in dock message:");
        System.out.println("  - Drone In Dock (无人机在机场): " + message.getDrone_in_dock());
        String statusDesc = message.getDrone_in_dock() == 1 ? "在机场" : "不在机场";
        System.out.println("  - Status Description: " + statusDesc);
    }
}

class MyAircraftLocationResultCallback extends SDKSubscribeResultCallback {
    @Override
    public void invoke(NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft location subscription successful.");
        } else {
            System.out.println("[Java] Aircraft location subscription failed. Error code: " + errorCode);
            // 可以在这里处理不同的错误码
            if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_InvalidParameter) {
                System.out.println("[Java] Error: Invalid parameter");
            } else if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NotConnected) {
                System.out.println("[Java] Error: Not connected");
            } else if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_SendError) {
                System.out.println("[Java] Error: Send error");
            } else if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_SubscribeError) {
                System.out.println("[Java] Error: Subscribe error");
            } else {
                System.out.println("[Java] Error: Unknown error");
            }
        }
    }
}

// 7. 创建飞机姿态订阅结果回调类
class MyAircraftAttitudeResultCallback extends SDKSubscribeResultCallback {
    @Override
    public void invoke(NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft attitude subscription successful.");
        } else {
            System.out.println("[Java] Aircraft attitude subscription failed. Error code: " + errorCode);
        }
    }
}

// 8. 创建飞机速度订阅结果回调类
class MyAircraftSpeedResultCallback extends SDKSubscribeResultCallback {
    @Override
    public void invoke(NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Aircraft speed subscription successful.");
        } else {
            System.out.println("[Java] Aircraft speed subscription failed. Error code: " + errorCode);
        }
    }
}

// 9. 创建设备OSD订阅结果回调类
class MyDeviceOsdResultCallback extends SDKSubscribeResultCallback {
    @Override
    public void invoke(NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] Device OSD subscription successful.");
        } else {
            System.out.println("[Java] Device OSD subscription failed. Error code: " + errorCode);
        }
    }
}

// 新增：通用订阅结果回调类
class MyGenericResultCallback extends SDKSubscribeResultCallback {
    private String subscriptionName;
    
    public MyGenericResultCallback(String name) {
        this.subscriptionName = name;
    }
    
    @Override
    public void invoke(NotificationCenterErrorCode errorCode) {
        if (errorCode == NotificationCenterErrorCode.NotificationCenterErrorCode_NoError) {
            System.out.println("[Java] " + subscriptionName + " subscription successful.");
        } else {
            System.out.println("[Java] " + subscriptionName + " subscription failed. Error code: " + errorCode);
        }
    }
}

public class Main {

    public static void main(String[] args) throws InterruptedException {
        System.out.println("[Java] Starting NotificationCenterSDK Demo...");

        // 获取SDK实例（自动加载JNI库）
        NotificationCenterSDK sdk = NotificationCenterSDK.getInstance();
        System.out.println("[Java] NotificationCenterSDK instance created.");

        // 创建连接监听器（在SDK初始化之后）
        MyConnectionListener listener = new MyConnectionListener();

        // 初始化SDK
        SdkInitializeInfo initInfo = new SdkInitializeInfo("", SdkLogLevel.INFO);
        sdk.init(initInfo);
        System.out.println("[Java] SDK initialized successfully.");

        // 设置连接监听器
        sdk.setWebsocketEventListener(listener);
        System.out.println("[Java] Websocket event listener set.");

        String url = "wss://dev-es310-api.dbeta.me/notification/ws/v1/notifications?x-auth-token=test";
        System.out.println("[Java] Connecting to " + url + "...");
        sdk.connect(url);

        // 等待连接建立
        System.out.println("[Java] Waiting for connection to establish...");
        Thread.sleep(2000); // 等待2秒让连接建立

        // 获取BusinessManager并演示监听功能
        BusinessManager businessManager = sdk.getBusinessManager();
        if (businessManager == null) {
            System.err.println("[Java] Failed to get BusinessManager - it is null!");
            return;
        }
        System.out.println("[Java] BusinessManager obtained successfully.");

        // 创建回调对象
        MyDeviceOsdMessageCallback deviceOsdMessageCallback = new MyDeviceOsdMessageCallback();
        MyDeviceOsdResultCallback deviceOsdResultCallback = new MyDeviceOsdResultCallback();
        
        // 新增：创建新接口的回调对象
        MyAircraftBatteryInfoCallback batteryInfoCallback = new MyAircraftBatteryInfoCallback();
        MyAircraftControlCodeCallback controlCodeCallback = new MyAircraftControlCodeCallback();
        MyAircraftWindSpeedCallback windSpeedCallback = new MyAircraftWindSpeedCallback();
        MyAircraftModeCodeCallback modeCodeCallback = new MyAircraftModeCodeCallback();
        MyDroneInDockCallback droneInDockCallback = new MyDroneInDockCallback();
        
        MyGenericResultCallback batteryInfoResultCallback = new MyGenericResultCallback("Aircraft Battery Info");
        MyGenericResultCallback controlCodeResultCallback = new MyGenericResultCallback("Aircraft Control Code");
        MyGenericResultCallback windSpeedResultCallback = new MyGenericResultCallback("Aircraft Wind Speed");
        MyGenericResultCallback modeCodeResultCallback = new MyGenericResultCallback("Aircraft Mode Code");
        MyGenericResultCallback droneInDockResultCallback = new MyGenericResultCallback("Drone In Dock");

        // 演示监听飞机位置消息
        System.out.println("[Java] Starting aircraft location monitoring...");
        
        // 监听多个设备
        String[] deviceSNs = {
            "8UUXN2D00A00VL",
            "1581F8HGD24BN0010286",
            "8UUDMAQ00A0121",
            "6QCDL820020093",
            "8PHDM8L0010322",
            "8UUDMAQ00A0138",
            "8UUDMAQ00A0047",
        };
        NotificationFrequency frequency = NotificationFrequency.PUSH_1S;
        
        // 演示监听设备OSD消息
        System.out.println("[Java] Starting device OSD monitoring for multiple devices...");
        
        // 存储所有监听ID，用于后续取消订阅
        long[] deviceOsdListenIds = new long[deviceSNs.length];
        long[] batteryInfoListenIds = new long[deviceSNs.length];
        long[] controlCodeListenIds = new long[deviceSNs.length];
        long[] windSpeedListenIds = new long[deviceSNs.length];
        long[] modeCodeListenIds = new long[deviceSNs.length];
        long[] droneInDockListenIds = new long[deviceSNs.length];
        
        for (int i = 0; i < deviceSNs.length; i++) {
            String deviceSN = deviceSNs[i];
            System.out.println("[Java] Starting monitoring for device " + (i + 1) + ": " + deviceSN);
            
            // 监听设备OSD
            deviceOsdListenIds[i] = businessManager.ListenDeviceOsd(
                deviceOsdMessageCallback, 
                deviceOsdResultCallback, 
                deviceSN, 
                frequency
            );
            
            // 新增：监听飞机电池信息
            batteryInfoListenIds[i] = businessManager.ListenAircraftBatteryInfo(
                batteryInfoCallback,
                batteryInfoResultCallback,
                deviceSN,
                frequency
            );
            
            // 新增：监听飞机控制代码
            controlCodeListenIds[i] = businessManager.ListenAircraftControlCode(
                controlCodeCallback,
                controlCodeResultCallback,
                deviceSN,
                frequency
            );
            
            // 新增：监听飞机风速
            windSpeedListenIds[i] = businessManager.ListenAircraftWindSpeed(
                windSpeedCallback,
                windSpeedResultCallback,
                deviceSN,
                frequency
            );
            
            // 新增：监听飞机模式代码
            modeCodeListenIds[i] = businessManager.ListenAircraftModeCode(
                modeCodeCallback,
                modeCodeResultCallback,
                deviceSN,
                frequency
            );
            
            // 新增：监听无人机在机场状态
            droneInDockListenIds[i] = businessManager.ListenDroneInDock(
                droneInDockCallback,
                droneInDockResultCallback,
                deviceSN,
                frequency
            );
            
            System.out.println("[Java] Started monitoring for device " + deviceSN + 
                             " with frequency " + frequency + 
                             " (Listen IDs - OSD: " + deviceOsdListenIds[i] + 
                             ", Battery: " + batteryInfoListenIds[i] + 
                             ", Control: " + controlCodeListenIds[i] + 
                             ", Wind: " + windSpeedListenIds[i] + 
                             ", Mode: " + modeCodeListenIds[i] + 
                             ", Dock: " + droneInDockListenIds[i] + ")");
        }
        
        // 等待一段时间让自动poll处理消息
        System.out.println("[Java] Waiting for messages (auto-poll is running)...");
        System.out.println("[Java] Monitoring the following topics:");
        System.out.println("  - Device OSD (设备OSD信息)");
        System.out.println("  - Aircraft Battery Info (飞机电池信息)");
        System.out.println("  - Aircraft Control Code (飞机控制代码)");
        System.out.println("  - Aircraft Wind Speed (飞机风速)");
        System.out.println("  - Aircraft Mode Code (飞机模式代码)");
        System.out.println("  - Drone In Dock (无人机在机场状态)");
        Thread.sleep(1000000); // 等待1000秒
        
        // 取消所有设备的监听
        System.out.println("[Java] Cancelling all subscriptions...");
        for (int i = 0; i < deviceSNs.length; i++) {
            businessManager.cancelObserve((int)deviceOsdListenIds[i]);
            businessManager.cancelObserve((int)batteryInfoListenIds[i]);
            businessManager.cancelObserve((int)controlCodeListenIds[i]);
            businessManager.cancelObserve((int)windSpeedListenIds[i]);
            businessManager.cancelObserve((int)modeCodeListenIds[i]);
            businessManager.cancelObserve((int)droneInDockListenIds[i]);
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