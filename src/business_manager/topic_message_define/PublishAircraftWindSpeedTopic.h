#pragma once

#include "topic_engine/TopicMessageWrapper.h"

class AircraftWindSpeedMsg {
public:
    int wind_direction = 0; // 当前风向 "enum_desc": "1:正北  2：东北  3:东  4:东南  5:南  6:西南  7:西   8:西北"
    double wind_speed = 0.0; // 风速估计，该风速是通过飞机姿态推测出的，有一定的误差，仅供参考，不能作为气象数据使用

    AIGC_JSON_HELPER(wind_direction, wind_speed);
};

class PublishAircraftWindSpeedTopic : public PublishTopicWrapper {
public:
    bool isValid() override {
        return is_valid_;
    }

    explicit PublishAircraftWindSpeedTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        MessageParserHook();
    }

    AircraftWindSpeedMsg msg;

    AIGC_JSON_HELPER(msg);
    AIGC_JSON_HELPER_BASE((PublishTopicWrapper *)this)

    std::string ToJsonString() override {
        std::string err, json_string;
        aigc::JsonHelper::ObjectToJson(*this, json_string, &err);
        return json_string;
    }
    void FromJsonString(const std::string& json) override {
        std::string err;
        aigc::JsonHelper::JsonToObject(*this, json, {}, &err);
    }
private:
    bool is_valid_ = true;

    void MessageParserHook() {
        // hook 消息解析
        DeviceOsdType device_type(message_data);
        if (device_type.isDrone() == false) {
            // 机场的 osd，不处理
            is_valid_ = false;
            return;
        }
        // 解析飞机 osd 消息
        DeviceOsdAircraft aircraft_parser(message_data);
        if (aircraft_parser.isParsedSuccessfully() == false) {
            is_valid_ = false;
            return;
        }
        // 获取需要的字段
        msg.wind_direction = aircraft_parser.getWindDirection();
        msg.wind_speed = aircraft_parser.getWindSpeed();
    }
};