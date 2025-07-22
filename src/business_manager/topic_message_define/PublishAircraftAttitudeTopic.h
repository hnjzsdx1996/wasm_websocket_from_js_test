#pragma once

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

    explicit PublishAircraftAttitudeTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        std::string err;
        aigc::JsonHelper::JsonToObject(msg, publish_msg->message_data, {}, &err);
        if (!err.empty()) {
            NC_LOG_ERROR("PublishAircraftAttitudeTopic ctor error, err: %s, json: %s", err.c_str(), publish_msg->ToJsonString().c_str());
        }
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
};