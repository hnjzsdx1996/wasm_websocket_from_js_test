#pragma once
#include <string>

/* Example:
{
    "message_topic": "device_osd",   // 比如device_osd数据
    "message_id": "xxxxxxx",
    "message_data": "xxxxxx",    // 内容为 json 字符串
    "timestamp": 1752197415
    "need_replay": false,
    "version": "xxx"
}
*/

class TopicMessageWrapper {
public:

    std::string ToJsonString() override {
        return {};
    }

    std::string topic_;
    uint8_t version_;
    uint64_t timestamp_;
    std::string raw_string_data_;
};
