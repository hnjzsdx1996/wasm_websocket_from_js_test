#pragma once
#include <cstdint>
#include <string>
#include <AIGCJson.hpp>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "base/logger/logger.h"
#include "base/utils/cpp_uuid.h"

class TopicMessageWrapper {
public:
    virtual ~TopicMessageWrapper() = default;

    virtual bool isValid() {
        if (message_type.empty()) {
            NC_LOG_ERROR("TopicMessageWrapper isValid: message_type.empty()");
            return false;
        }
        if (message_type != "subscribe" &&
            message_type != "unsubscribe" &&
            message_type != "publish" &&
            message_type != "ping" &&
            message_type != "pong") {
            NC_LOG_ERROR("TopicMessageWrapper isValid: message_type unknown: %s", message_type.c_str());
            return false;
        }
        if (message_id.empty()) {
            NC_LOG_ERROR("TopicMessageWrapper isValid: message_id.empty()");
            return false;
        }
        return true;
    }

    std::string message_type; // subscribe/unsubscribe/publish/ping/pong
    std::string message_id = generate_uuid_v4(); // uuid, 用于消息匹配
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

enum NotifactionFrequency {
    NotifactionFrequency_Any = 0, // 服务端自定决定推送频率
    NotifactionFrequency_OnChanged = 1, // 按变推送
    NotifactionFrequency_Push_1s = 1000, // 定频 1s 推送 1 次
    NotifactionFrequency_Push_2s = 2000, // 定频 2s 推送 1 次
    NotifactionFrequency_Push_3s = 3000, // 定频 3s 推送 1 次
    NotifactionFrequency_Push_4s = 4000, // 定频 4s 推送 1 次
    NotifactionFrequency_Push_5s = 5000, // 定频 5s 推送 1 次
    NotifactionFrequency_Push_10s = 10000, // 定频 10s 推送 1 次
    NotifactionFrequency_Push_20s = 20000, // 定频 20s 推送 1 次
    NotifactionFrequency_Push_30s = 30000, // 定频 30s 推送 1 次
};

class SubscribeItems {
public:
    std::string device_sn;
    std::vector<std::string> topics;

    AIGC_JSON_HELPER(device_sn, topics);
};

class SubscribeTopicMessageData {
public:
    NotifactionFrequency notify_strategy = NotifactionFrequency_Any;
    std::vector<SubscribeItems> items;

    AIGC_JSON_HELPER(notify_strategy, items);
};

class SubscribeTopicWrapper : public TopicMessageWrapper {
public:

    explicit SubscribeTopicWrapper(const std::string& sn, const std::string& topic, NotifactionFrequency freq) {
        message_type = "subscribe";
        need_replay = true;
        version = "1";
        message_data.notify_strategy = freq;
        message_data.items.push_back({
            .device_sn = sn,
            .topics = {topic},
        });
    }

    bool isValid() override {
        if (TopicMessageWrapper::isValid() == false) {
            return false;
        }
        return true;
    }

    SubscribeTopicMessageData message_data;

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

class SubscribeTopicReplyMessageData {
public:
    int code = 0;
    std::string message;

    AIGC_JSON_HELPER(code, message);
};

class SubscribeTopicReplyWrapper : public TopicMessageWrapper {
public:

    SubscribeTopicReplyMessageData message_data;

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

class UnSubscribeTopicMessageData {
public:
    std::vector<SubscribeItems> items;

    AIGC_JSON_HELPER(items);
};

class UnSubscribeTopicWrapper : public TopicMessageWrapper {
public:

    explicit UnSubscribeTopicWrapper(const std::string& sn, const std::string& topic) {
        message_type = "unsubscribe";
        need_replay = true;
        version = "1";
        message_data.items.push_back({
            .device_sn = sn,
            .topics = {topic},
        });
    }

    bool isValid() override {
        if (TopicMessageWrapper::isValid() == false) {
            return false;
        }
        return true;
    }

    UnSubscribeTopicMessageData message_data;

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

class PublishTopicWrapper : public TopicMessageWrapper {
public:
    bool isValid() override {
        if (TopicMessageWrapper::isValid() == false) {
            NC_LOG_ERROR("PublishTopicWrapper isValid: TopicMessageWrapper::isValid() == false");
            return false;
        }
        if (message_topic.empty()) {
            NC_LOG_ERROR("PublishTopicWrapper isValid: message_topic.empty()");
            return false;
        }
        return true;
    }

    std::string device_sn;
    std::string message_topic;

    std::string message_data; // 这个保存原始的数据，不进行 json 操作, 交给用户进行解析

    AIGC_JSON_HELPER(device_sn, message_topic, message_data);

    AIGC_JSON_HELPER_BASE((TopicMessageWrapper *)this)

    std::string ToJsonString() override {
        std::string err, json_string;
        aigc::JsonHelper::ObjectToJson(*this, json_string, &err);
        return json_string;
    }
    void FromJsonString(const std::string& json) override {
        std::string err;
        aigc::JsonHelper::JsonToObject(*this, json, {}, &err);
        
        // 使用rapidjson直接从JSON字符串中提取message_data字段
        rapidjson::Document doc;
        doc.Parse(json.c_str());

        if (message_data.empty() == false) {
            return;
        }

        if (!doc.HasParseError() && doc.HasMember("message_data")) {
            const rapidjson::Value& message_data_value = doc["message_data"];
            if (message_data_value.IsString()) {
                message_data = message_data_value.GetString();
            } else {
                // 如果不是字符串，将其转换为JSON字符串
                rapidjson::StringBuffer buffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                message_data_value.Accept(writer);
                message_data = buffer.GetString();
            }
        }
        NC_LOG_WARN("PublishTopicWrapper FromJsonString message_data: %s", message_data.c_str());
        
    }
};

class PingTopicWrapper : public TopicMessageWrapper {
public:
    explicit PingTopicWrapper() {
        message_type = "ping";
        need_replay = true; // ping 消息需要回复
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
    explicit PongTopicWrapper(const std::string& json) {
        TopicMessageWrapper::FromJsonString(json);
    }
};