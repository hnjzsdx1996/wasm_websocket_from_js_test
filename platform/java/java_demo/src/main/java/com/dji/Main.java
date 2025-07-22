package com.dji;

import com.dji.notificationcentersdk.*;

// 1. Create a class that extends the SWIG-generated listener.
// This is how we handle callbacks from C++.
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

// 2. Create callback classes for aircraft location monitoring using new interface
class MyAircraftLocationMessageCallback extends JavaMessageCallbackAircraftLocation {
    @Override
    public void invoke(AircraftLocation message) {
        System.out.println("[Java] Received aircraft location message:");
        System.out.println("  - Height (椭球高度): " + message.getHeight());
        System.out.println("  - Elevation (相对起飞点高度): " + message.getElevation());
        System.out.println("  - Latitude (纬度): " + message.getLatitude());
        System.out.println("  - Longitude (经度): " + message.getLongitude());
    }
}

class MyAircraftLocationResultCallback extends JavaResultCallback {
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

    static {
        // 3. Load the JNI library using an absolute path passed as a system property.
        String distPath = System.getProperty("notificationcenter.dist.path");
        if (distPath == null) {
            throw new UnsatisfiedLinkError("System property 'notificationcenter.dist.path' is not set.");
        }
        // On macOS, the library is named lib<name>.jnilib
        String libPath = distPath + "/libnotificationcenter.jnilib";
        System.load(libPath);
        System.out.println("[Java] JNI library loaded successfully from: " + libPath);
    }

    public static void main(String[] args) throws InterruptedException {
        System.out.println("[Java] Starting NotificationCenterSDK Demo...");

        // 4. Create an instance of our listener.
        MyConnectionListener listener = new MyConnectionListener();

        // 5. Create and configure the SDKManager.
        SDKManager sdkManager = new SDKManager();
        System.out.println("[Java] SDKManager created.");

        // 6. 初始化SDK
        SdkInitializeInfo initInfo = new SdkInitializeInfo("", SdkLogLevel.INFO);
        sdkManager.init(initInfo);
        System.out.println("[Java] SDK initialized successfully.");

        // We need to set the listener on the manager.
        sdkManager.setWebsocketEventListener(listener);
        System.out.println("[Java] Websocket event listener set.");

        String url = "ws://localhost:3001";
        System.out.println("[Java] Connecting to " + url + "...");
        sdkManager.connect(url);

        // 7. 等待连接建立
        System.out.println("[Java] Waiting for connection to establish...");
        Thread.sleep(2000); // 等待2秒让连接建立

        // 8. 获取BusinessManager并演示新的监听飞机位置功能
        BusinessManager businessManager = sdkManager.getBusinessManager();
        if (businessManager == null) {
            System.err.println("[Java] Failed to get BusinessManager - it is null!");
            return;
        }
        System.out.println("[Java] BusinessManager obtained successfully.");

        // 创建回调对象
        MyAircraftLocationMessageCallback messageCallback = new MyAircraftLocationMessageCallback();
        MyAircraftLocationResultCallback resultCallback = new MyAircraftLocationResultCallback();

        // 演示监听飞机位置消息
        System.out.println("[Java] Starting aircraft location monitoring...");
        
        // 监听单个设备
        String deviceSN = "TEST001";
        NotificationFrequency frequency = NotificationFrequency.ANY; // 使用新的枚举类型
        
        long listenId = businessManager.ListenAircraftLocation(
            messageCallback, 
            resultCallback, 
            deviceSN, 
            frequency
        );
        
        System.out.println("[Java] Started monitoring device " + deviceSN + 
                         " with frequency " + frequency + 
                         " (Listen ID: " + listenId + ")");
        
        // 9. 定时调用poll方法消费任务
        System.out.println("[Java] Starting poll loop...");
        long startTime = System.currentTimeMillis();
        long pollInterval = 100; // 每100ms调用一次poll
        
        while (System.currentTimeMillis() - startTime < 10000) { // 运行10秒
            long taskCount = SDKManager.poll();
            if (taskCount > 0) {
                System.out.println("[Java] Poll executed " + taskCount + " tasks");
            }
            Thread.sleep(pollInterval);
        }
        
        // 取消订阅
        businessManager.cancelObserve((int)listenId);
        System.out.println("[Java] Cancelled monitoring for device " + deviceSN);

        // 继续poll一段时间
        System.out.println("[Java] Continuing poll for cleanup...");
        startTime = System.currentTimeMillis();
        while (System.currentTimeMillis() - startTime < 5000) { // 再运行5秒
            long taskCount = SDKManager.poll();
            if (taskCount > 0) {
                System.out.println("[Java] Poll executed " + taskCount + " tasks");
            }
            Thread.sleep(pollInterval);
        }

        System.out.println("[Java] Demo finished.");
    }
} 