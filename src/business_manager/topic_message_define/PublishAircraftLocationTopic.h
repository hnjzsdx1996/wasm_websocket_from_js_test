#pragma once

#include "topic_engine/TopicMessageWrapper.h"

class AircraftLocationMsg {
public:
    double height = 0.0; // 椭球高度
    double elevation = 0.0; // 相对起飞点高度
    double longitude = 0.0;
    double latitude = 0.0;

    AIGC_JSON_HELPER(height, elevation, longitude, latitude);
};

class PublishAircraftLocationTopic : public PublishTopicWrapper {
public:
    bool isValid() override {
        return is_valid_;
    }

    explicit PublishAircraftLocationTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        MessageParserHook();
    }

    AircraftLocationMsg msg;

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
        msg.height = aircraft_parser.getHeight();
        msg.elevation = aircraft_parser.getAttitudePitch();
        msg.longitude = aircraft_parser.getLongitude();
        msg.latitude = aircraft_parser.getLatitude();
    }
};