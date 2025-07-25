#pragma once

#include "topic_engine/TopicMessageWrapper.h"

class AircraftControlCodeMsg {
public:
    int control_mode = 0.0; // "0": "航线控制-执行态", "1": "指令控制-手动飞行", "2": "指令控制-自动飞行", "3": "航线控制-中断态"

    AIGC_JSON_HELPER(control_mode);
};

class PublishAircraftControlCodeTopic : public PublishTopicWrapper {
public:
    bool isValid() override {
        return is_valid_;
    }

    explicit PublishAircraftControlCodeTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        MessageParserHook();
    }

    AircraftControlCodeMsg msg;

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
        msg.control_mode = aircraft_parser.getModeCode(); // todo:sdk OSD 里面没有这个字段
    }
};