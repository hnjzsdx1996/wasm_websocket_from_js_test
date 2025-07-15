#pragma once

#include "../TopicMessageWrapper.h"
#include <AIGCJson.hpp>

class DeviceOsdMessage {
public:
    int velocity = 0;
    AIGC_JSON_HELPER(velocity);
};

class DeviceOsdTopic : public TopicMessageWrapper {
public:
    DeviceOsdTopic() {
        message_topic = "device_osd";
    }

    DeviceOsdMessage message_data;
    AIGC_JSON_HELPER(message_data);
    AIGC_JSON_HELPER_BASE((TopicMessageWrapper *)this)

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