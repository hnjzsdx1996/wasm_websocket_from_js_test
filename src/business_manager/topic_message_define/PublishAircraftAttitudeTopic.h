#pragma once

#include "parser/DeviceOsdAircraft.h"
#include "parser/DeviceOsdDock.h"
#include "parser/DeviceOsdType.h"
#include "topic_engine/TopicMessageWrapper.h"

class AircraftAttitudeMsg {
public:
    double attitude_head = 0.0; // 机头朝向角度
    double attitude_pitch = 0.0; // 俯仰轴角度
    double attitude_roll = 0.0; // 横滚轴角度

    AIGC_JSON_HELPER(attitude_head, attitude_pitch, attitude_roll);
};

class PublishAircraftAttitudeTopic : public PublishTopicWrapper {
public:
    bool isValid() override {
        return is_valid_;
    }

    explicit PublishAircraftAttitudeTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        MessageParserHook();
    }

    AircraftAttitudeMsg msg;

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
        // 解析机场 osd 消息
        DeviceOsdAircraft aircraft_parser(message_data);
        if (aircraft_parser.isParsedSuccessfully() == false) {
            is_valid_ = false;
            return;
        }
        // 获取需要的字段
        msg.attitude_head = aircraft_parser.getAttitudeHead();
        msg.attitude_pitch = aircraft_parser.getAttitudePitch();
        msg.attitude_roll = aircraft_parser.getAttitudeRoll();
    }
};