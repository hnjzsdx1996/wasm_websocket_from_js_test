#pragma once

#include "topic_engine/TopicMessageWrapper.h"

class DockLocationMsg {
public:
    double heading = 0.0;
    double height = 0.0;
    double latitude = 0.0;
    double longitude = 0.0;

    AIGC_JSON_HELPER(heading, height, longitude, latitude);
};

class PublishDockLocationTopic : public PublishTopicWrapper {
public:
    bool isValid() override {
        return is_valid_;
    }

    explicit PublishDockLocationTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        MessageParserHook();
    }

    DockLocationMsg msg;

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
        if (device_type.getDeviceOsdTypeEnum() != DeviceOsdTypeEnum::DeviceOsdDock) {
            is_valid_ = false;
            return;
        }
        // 解析机场 osd 消息
        DeviceOsdDock dock_parser(message_data);
        if (dock_parser.isParsedSuccessfully() == false) {
            is_valid_ = false;
            return;
        }
        // 获取需要的字段
        msg.heading = dock_parser.getHeading();
        msg.height = dock_parser.getHeight();
        msg.longitude = dock_parser.getLongitude();
        msg.latitude = dock_parser.getLatitude();
    }
};