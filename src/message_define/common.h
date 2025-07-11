#pragma once

#include <AIGCJson.hpp>

class SDKConfig {
public:
    std::string sn;
    uint16_t ping_pong_interval;

    AIGC_JSON_HELPER(sn, ping_pong_interval);

    std::string ToJsonString() {
        std::string err;
        std::string json_string;
        aigc::JsonHelper::ObjectToJson(*this, json_string, &err);
        if (!err.empty()) {
            printf("CloudLinkConfigInfo ToJsonString error, err: %s", err.c_str());
        }
        return json_string;
    }

    void FromJsonString(const std::string &json_string) {
        std::string err;
        aigc::JsonHelper::JsonToObject(*this, json_string, {}, &err);
        if (!err.empty()) {
            printf("CloudLinkConfigInfo FromJsonString error, err: %s, json: %s", err.c_str(), json_string.c_str());
        }
    }
};
