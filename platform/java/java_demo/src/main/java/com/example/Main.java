package com.example;

import com.example.notificationcentersdk.*;

// 1. Create a class that extends the SWIG-generated listener.
// This is how we handle callbacks from C++.
class MyConnectionListener extends ConnectionListener {
    public void onOpen() {
        System.out.println("[Java] Connection opened.");
    }

    public void onClose(String reason) {
        System.out.println("[Java] Connection closed. Reason: " + reason);
    }

    public void onError(String error) {
        System.out.println("[Java] An error occurred: " + error);
    }

    public void onMessage(String message) {
        System.out.println("[Java] Received message: " + message);
    }
}

public class Main {

    static {
        // 2. Load the JNI library using an absolute path passed as a system property.
        try {
            String distPath = System.getProperty("notificationcenter.dist.path");
            if (distPath == null) {
                throw new UnsatisfiedLinkError("System property 'notificationcenter.dist.path' is not set.");
            }
            // On macOS, the library is named lib<name>.jnilib
            String libPath = distPath + "/libnotificationcenter.jnilib";
            System.load(libPath);
            System.out.println("[Java] JNI library loaded successfully from: " + libPath);
        } catch (UnsatisfiedLinkError e) {
            System.err.println("[Java] Native code library failed to load.\n" + e);
            System.exit(1);
        }
    }

    public static void main(String[] args) {
        System.out.println("[Java] Starting NotificationCenterSDK Demo...");

        // 3. Create an instance of our listener.
        MyConnectionListener listener = new MyConnectionListener();

        // 4. Create and configure the SDKManager.
        SDKManager sdkManager = new SDKManager();
        System.out.println("[Java] SDKManager created.");

        // We need to set the listener on the manager.
        sdkManager.setWebsocketEventListener(listener);
        System.out.println("[Java] Websocket event listener set.");

        String url = "ws://echo.websocket.events";
        System.out.println("[Java] Connecting to " + url + "...");
        sdkManager.connect(url);

        // Keep the application running to receive messages/events.
        try {
            System.out.println("[Java] Waiting for events for 10 seconds...");
            Thread.sleep(10000); // 10 seconds
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        System.out.println("[Java] Demo finished.");
        // 资源释放（如果 SWIG 生成了 delete 方法，可以调用）
        // sdkManager.delete();
    }
} 