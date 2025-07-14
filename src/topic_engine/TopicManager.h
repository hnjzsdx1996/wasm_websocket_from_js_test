#pragma once
#include <cstdint>
#include <functional>

// 对外提供平铺的和后端交互的消息接口
class TopicManager {
public:

    void CancelObserve(int64_t listen_id);

    int64_t ObserveDroneOsd(const std::function<void(const std::string& msg)>& callback);

};