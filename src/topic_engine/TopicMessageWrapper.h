#pragma once
#include <cstdint>
#include <string>
#include <AIGCJson.hpp>

class TopicMessageWrapper {
public:
    std::string message_topic;
    std::string message_id;
    std::string device_sn;
    bool need_replay{false};
    std::string version;
    uint64_t timestamp{0};

    AIGC_JSON_HELPER(message_topic, message_id, device_sn, need_replay, version, timestamp);

    virtual std::string ToJsonString() {
        std::string err, json_string;
        aigc::JsonHelper::ObjectToJson(*this, json_string, &err);
        return json_string;
    }
    virtual void FromJsonString(const std::string& json) {
        std::string err;
        aigc::JsonHelper::JsonToObject(*this, json, {}, &err);
    }
    virtual ~TopicMessageWrapper() = default;
};
