#pragma once

#include "../TopicMessageWrapper.h"
#include <AIGCJson.hpp>

class DeviceOsdMessage {
public:
    int velocity;
    AIGC_JSON_HELPER(velocity);
};

class DeviceOsdTopic : public TopicMessageWrapper {
public:
    DeviceOsdMessage msg;
    AIGC_JSON_HELPER_INHERIT(TopicMessageWrapper, msg);

    std::string ToJsonString() const override {
        std::string err, json_string;
        aigc::JsonHelper::ObjectToJson(*this, json_string, &err);
        return json_string;
    }
    void FromJsonString(const std::string& json) override {
        std::string err;
        aigc::JsonHelper::JsonToObject(*this, json, {}, &err);
    }
};