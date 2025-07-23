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
        System.out.println("  - Mode Code (模式代码): " + message.getMode_code());
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

        // 演示监听飞机位置消息
        System.out.println("[Java] Starting aircraft location monitoring...");
        
        // 监听多个设备
        String[] deviceSNs = {
            "8UUXN2D00A00VL",
            "1581F8HGD24BN0010286",
        };
        NotificationFrequency frequency = NotificationFrequency.PUSH_1S;
        
        // 演示监听设备OSD消息
        System.out.println("[Java] Starting device OSD monitoring for multiple devices...");
        
        // 存储所有监听ID，用于后续取消订阅
        long[] deviceOsdListenIds = new long[deviceSNs.length];
        
        for (int i = 0; i < deviceSNs.length; i++) {
            String deviceSN = deviceSNs[i];
            System.out.println("[Java] Starting device OSD monitoring for device " + (i + 1) + ": " + deviceSN);
            
            deviceOsdListenIds[i] = businessManager.ListenDeviceOsd(
                deviceOsdMessageCallback, 
                deviceOsdResultCallback, 
                deviceSN, 
                frequency
            );
            
            System.out.println("[Java] Started monitoring device OSD for device " + deviceSN + 
                             " with frequency " + frequency + 
                             " (Listen ID: " + deviceOsdListenIds[i] + ")");
        }
        
        // 等待一段时间让自动poll处理消息
        System.out.println("[Java] Waiting for messages (auto-poll is running)...");
        Thread.sleep(1000000); // 等待1000秒
        
        // 取消所有设备的OSD监听
        for (int i = 0; i < deviceSNs.length; i++) {
            businessManager.cancelObserve((int)deviceOsdListenIds[i]);
            System.out.println("[Java] Cancelled device OSD monitoring for device " + deviceSNs[i]);
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