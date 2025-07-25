#pragma once

#include "topic_engine/TopicMessageWrapper.h"

class DeviceOnlineStatusMsg {
public:
    bool device_status = false;
    std::string device_callsign;
    std::string device_model;
    std::string device_type;

    AIGC_JSON_HELPER(device_status, device_callsign, device_model, device_type);
};

class PublishDeviceOnlineStatusTopic : public PublishTopicWrapper {
public:
    explicit PublishDeviceOnlineStatusTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
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
            NC_LOG_ERROR("PublishDeviceOnlineStatusTopic ctor error, err: %s, json: %s", err.c_str(), publish_msg->ToJsonString().c_str());
        }

    }

    DeviceOnlineStatusMsg msg;

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