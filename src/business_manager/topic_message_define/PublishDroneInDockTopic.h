#pragma once

#include "topic_engine/TopicMessageWrapper.h"
#include "parser/DeviceOsdType.h"
#include "parser/DeviceOsdDock.h"

class DroneInDockMsg {
public:
    int drone_in_dock = 0;

    AIGC_JSON_HELPER(drone_in_dock);
};

class PublishDroneInDockTopic : public PublishTopicWrapper {
public:
    bool isValid() override {
        return is_valid_;
    }

    explicit PublishDroneInDockTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        NC_LOG_INFO("PublishDroneInDockTopic message_data: %s", publish_msg->message_data.c_str());

        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        // 有些复杂结构体无法用 AIGCJson 解析，手动进行解析
        MessageParserHook();
    }

    DroneInDockMsg msg;

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
        if (device_type.isDrone()) {
            // 飞机的 osd，不处理
            is_valid_ = false;
            return;
        }
        // 解析机场 osd 消息
        DeviceOsdDock docker_parser(message_data);
        if (docker_parser.isParsedSuccessfully() == false) {
            is_valid_ = false;
            return;
        }
        // 获取需要的字段
        msg.drone_in_dock = docker_parser.getDroneInDock();
    }
};