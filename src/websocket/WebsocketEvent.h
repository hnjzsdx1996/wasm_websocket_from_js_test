#pragma once
#include <string>


class WebsocketEvent {
public:
    virtual ~WebsocketEvent() = 0;

    virtual void OnMessage(const std::string& message) = 0;

    virtual void OnOpen() = 0;

    virtual void OnClose() = 0;

    virtual void OnError(const std::string& error) = 0;
};
