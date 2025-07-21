package com.example;

import com.example.notificationcentersdk.SDKManager;
import com.example.notificationcentersdk.BusinessManager;
import com.example.notificationcentersdk.ConnectionListener;

public class Main {
    public static void main(String[] args) {
        System.out.println("Hello from Java Demo!");

        // 1. Initialize SDKManager
        SDKManager sdkManager = new SDKManager();
        System.out.println("SDKManager created.");

        // 2. Set up connection listener
        sdkManager.setConnectionListener(new ConnectionListener() {
            @Override
            public void onOpen() {
                System.out.println(">>> WebSocket connected!");
            }

            @Override
            public void onClose() {
                System.out.println(">>> WebSocket disconnected!");
            }

            @Override
            public void onError(String error) {
                System.err.println(">>> WebSocket error: " + error);
            }

            @Override
            public void onMessage(String message) {
                System.out.println(">>> Received message: " + message);
            }
        });

        // 3. Connect WebSocket
        String wsUrl = "ws://echo.websocket.events";
        System.out.println("Connecting to WebSocket: " + wsUrl);
        sdkManager.connect(wsUrl);

        // 4. 配置 SDK（如有需要）
        // sdkManager.configure("{...}");

        // 5. Get BusinessManager
        BusinessManager businessManager = sdkManager.getBusinessManager();
        System.out.println("Got BusinessManager.");

        // 6. Subscribe to a topic
        String deviceSn = "123456789";
        int frequency = 1; // Example frequency
        System.out.println("Subscribing to aircraft location for SN: " + deviceSn);
        long listenId = businessManager.listenAircraftLocation(deviceSn, frequency);

        if (listenId > 0) {
            System.out.println("Successfully subscribed with listenId: " + listenId);
        } else {
            System.err.println("Failed to subscribe.");
            sdkManager.destroy();
            return;
        }

        // 7. Poll for messages in a loop
        System.out.println("Starting to poll for events...");
        for (int i = 0; i < 20; i++) {
            sdkManager.poll();
            try {
                // Poll every 500ms
                Thread.sleep(500);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        // 8. Clean up
        System.out.println("Cleaning up resources...");
        businessManager.cancelObserve(listenId);
        System.out.println("Cancelled observation for listenId: " + listenId);
        
        sdkManager.destroy();
        System.out.println("SDKManager destroyed.");

        System.out.println("Java Demo finished.");
    }
} 