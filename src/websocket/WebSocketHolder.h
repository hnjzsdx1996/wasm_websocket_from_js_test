#pragma once
#include "WebSocketBase.h"

class WebSocketHolder {
public:
    void setWebSocket(WebSocketBase* ws) { ws_ = ws; }
    WebSocketBase* getWebSocket() const { return ws_; }
private:
    WebSocketBase* ws_ = nullptr;
}; 