package com.dji;

import com.dji.notificationcentersdk.generated.*;
import com.dji.notificationcentersdk.core.NotificationCenterSDK;

// 1. 创建连接监听器类
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

// 2. 创建飞机位置消息回调类
class MyAircraftLocationMessageCallback extends AircraftLocationCallback {
    @Override
    public void invoke(AircraftLocation message) {
        System.out.println("[Java] Received aircraft location message:");
        System.out.println("  - Height (椭球高度): " + message.getHeight());
        System.out.println("  - Elevation (相对起飞点高度): " + message.getElevation());
        System.out.println("  - Latitude (纬度): " + message.getLatitude());
        System.out.println("  - Longitude (经度): " + message.getLongitude());
    }
}

// 3. 创建订阅结果回调类
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

public class Main {

    public static void main(String[] args) throws InterruptedException {
        System.out.println("[Java] Starting NotificationCenterSDK Demo...");

        // 1. 获取SDK实例（自动加载JNI库）
        NotificationCenterSDK sdk = NotificationCenterSDK.getInstance();
        System.out.println("[Java] NotificationCenterSDK instance created.");

        // 2. 创建连接监听器（在SDK初始化之后）
        MyConnectionListener listener = new MyConnectionListener();

        // 3. 初始化SDK
        SdkInitializeInfo initInfo = new SdkInitializeInfo("", SdkLogLevel.INFO);
        sdk.init(initInfo);
        System.out.println("[Java] SDK initialized successfully.");

        // 4. 设置连接监听器
        sdk.setWebsocketEventListener(listener);
        System.out.println("[Java] Websocket event listener set.");

        String url = "ws://localhost:3001";
        System.out.println("[Java] Connecting to " + url + "...");
        sdk.connect(url);

        // 5. 等待连接建立
        System.out.println("[Java] Waiting for connection to establish...");
        Thread.sleep(2000); // 等待2秒让连接建立

        // 6. 获取BusinessManager并演示监听飞机位置功能
        BusinessManager businessManager = sdk.getBusinessManager();
        if (businessManager == null) {
            System.err.println("[Java] Failed to get BusinessManager - it is null!");
            return;
        }
        System.out.println("[Java] BusinessManager obtained successfully.");

        // 7. 创建回调对象
        MyAircraftLocationMessageCallback messageCallback = new MyAircraftLocationMessageCallback();
        MyAircraftLocationResultCallback resultCallback = new MyAircraftLocationResultCallback();

        // 8. 演示监听飞机位置消息
        System.out.println("[Java] Starting aircraft location monitoring...");
        
        // 9. 监听单个设备
        String deviceSN = "TEST001";
        NotificationFrequency frequency = NotificationFrequency.ANY;
        
        long listenId = businessManager.ListenAircraftLocation(
            messageCallback, 
            resultCallback, 
            deviceSN, 
            frequency
        );
        
        System.out.println("[Java] Started monitoring device " + deviceSN + 
                         " with frequency " + frequency + 
                         " (Listen ID: " + listenId + ")");
        
        // 10. 等待一段时间让自动poll处理消息
        System.out.println("[Java] Waiting for messages (auto-poll is running)...");
        Thread.sleep(10000); // 等待10秒
        
        // 取消订阅
        businessManager.cancelObserve((int)listenId);
        System.out.println("[Java] Cancelled monitoring for device " + deviceSN);

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