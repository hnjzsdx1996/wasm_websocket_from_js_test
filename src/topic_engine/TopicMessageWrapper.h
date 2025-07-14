#pragma once
#include <cstdint>
#include <string>
#include <AIGCJson.hpp>

class TopicMessageWrapper {
public:
    std::string topic_;
    std::string uuid_;
    bool need_replay_{false};
    uint8_t version_{1};
    uint64_t timestamp_{0};
    std::string raw_string_data_;

    AIGC_JSON_HELPER(topic_, uuid_, need_replay_, version_, timestamp_, raw_string_data_);

    virtual std::string ToJsonString() const {
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
