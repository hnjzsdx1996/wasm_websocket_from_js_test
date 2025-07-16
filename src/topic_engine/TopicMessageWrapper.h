#pragma once
#include <cstdint>
#include <string>
#include <AIGCJson.hpp>

#include "base/logger/logger.h"
#include "base/utils/cpp_uuid.h"

class TopicMessageWrapper {
public:
    virtual ~TopicMessageWrapper() = default;

    std::string message_type; // subscribe/unsubscribe/publish/ping/pong
    std::string message_id; // uuid, 用于消息匹配
    uint64_t timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    bool need_replay{false};
    std::string version;

    AIGC_JSON_HELPER(message_type, message_id, timestamp, need_replay, version);

    virtual std::string ToJsonString() {
        std::string err, json_string;
        aigc::JsonHelper::ObjectToJson(*this, json_string, &err);
        return json_string;
    }
    virtual void FromJsonString(const std::string& json) {
        std::string err;
        aigc::JsonHelper::JsonToObject(*this, json, {}, &err);
    }
};

class PublishTopicWrapper : public TopicMessageWrapper {
public:

    std::string message_topic;

    AIGC_JSON_HELPER(message_topic);

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

class PingTopicWrapper : public TopicMessageWrapper {
public:
    explicit PingTopicWrapper() {
        message_type = "ping";
        message_id = generate_uuid_v4();
        need_replay = false; // pong 消息不需要回复
        version = "1"; // 当前版本先写死为 1
    }
};

class PongTopicWrapper : public TopicMessageWrapper {
public:
    // 基于 ping 消息构造 pong 消息
    explicit PongTopicWrapper(const std::shared_ptr<PingTopicWrapper>& ping) {
        message_type = "pong";
        message_id = ping->message_id;
        timestamp = ping->timestamp; // 填 ping 包的时间戳，ping 包发送方可以计算 RTT
        need_replay = false; // pong 消息不需要回复
        version = ping->version;
    }
};