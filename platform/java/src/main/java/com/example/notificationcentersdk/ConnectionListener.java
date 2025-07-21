package com.example.notificationcentersdk;

public interface ConnectionListener {
    /**
     * Called when the WebSocket connection is successfully opened.
     */
    void onOpen();

    /**
     * Called when the WebSocket connection is closed.
     */
    void onClose();

    /**
     * Called when an error occurs with the WebSocket connection.
     * @param error The error message.
     */
    void onError(String error);

    /**
     * Called when a message is received from the WebSocket.
     * @param message The message content.
     */
    void onMessage(String message);
} 