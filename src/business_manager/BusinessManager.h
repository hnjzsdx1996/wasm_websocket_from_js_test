#pragma once
#include <functional>
#include <memory>
#include "BusinessManagerDefine.h"
#include "../topic_engine/TopicManager.h"
#include "../topic_engine/topic_message_define/DeviceOsdTopic.h"

// 业务逻辑：ping-pong 心跳，消息订阅/取消订阅，处理业务逻辑，提供接口
class BusinessManager {
public:
    explicit BusinessManager(std::weak_ptr<TopicManager> topic_mgr);

    void CancelObserve(int64_t listen_id);

    int64_t ObserveDeviceOsd(const std::function<void(const std::shared_ptr<DeviceOsdTopic>&)>& cb);

    // 接口范式
    /**
    * @brief 订阅飞机位置消息
    * 
    * @param on_messages_callback  消息回调函数，收到位置消息时会被调用，参数为消息内容
    * @param on_result_callback    订阅结果回调函数，订阅成功或失败时会被调用，参数为结果信息
    * @param device_sn             设备序列号（SN），用于唯一标识目标飞机
    * @param freq                  消息推送频率，详见 NotifactionFrequency 枚举定义
    * @return NotificationCenterErrorCode  返回错误码，成功为 ListenId，失败返回对应错误类型
    */
    NotificationCenterErrorCode ListenAircraftLocation(OnSubscribeMessageCallback on_messages_callback, OnSubscribeResultCallback on_result_callback, std::string device_sn, NotifactionFrequency freq);

private:
    std::weak_ptr<TopicManager> topic_mgr_;
};