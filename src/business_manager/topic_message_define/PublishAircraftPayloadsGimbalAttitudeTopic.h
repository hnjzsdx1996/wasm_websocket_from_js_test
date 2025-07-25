#pragma once

#include "topic_engine/TopicMessageWrapper.h"

class AircraftPayloadGimbalAttitudeMsg {
public:
    double gimbal_pitch = 0.0;
    double gimbal_roll = 0.0;
    double gimbal_yaw = 0.0;

    AIGC_JSON_HELPER(gimbal_pitch, gimbal_roll, gimbal_yaw);
};

class AircraftPayloadsGimbalAttitudeMsg {
public:
    std::unordered_map<std::string, AircraftPayloadGimbalAttitudeMsg> payloads_gimbal_attitude;

    AIGC_JSON_HELPER(payloads_gimbal_attitude);
};

class PublishAircraftPayloadsGimbalAttitudeTopic : public PublishTopicWrapper {
public:
    bool isValid() override {
        return is_valid_;
    }

    explicit PublishAircraftPayloadsGimbalAttitudeTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        MessageParserHook();
    }

    AircraftPayloadsGimbalAttitudeMsg msg;

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
private:
    bool is_valid_ = true;

    void MessageParserHook() {
        // hook 消息解析
        DeviceOsdType device_type(message_data);
        if (device_type.isDrone() == false) {
            // 机场的 osd，不处理
            is_valid_ = false;
            return;
        }
        // 解析飞机 osd 消息
        DeviceOsdAircraft aircraft_parser(message_data);
        if (aircraft_parser.isParsedSuccessfully() == false) {
            is_valid_ = false;
            return;
        }
        // 获取需要的字段
        auto payloads_indexes = aircraft_parser.getGimbalPayloadIndices();

        for (const auto& payload_index : payloads_indexes) {
            auto payload_info = aircraft_parser.getGimbalInfo(payload_index);
            if (payload_info != nullptr) {
                AircraftPayloadGimbalAttitudeMsg payload_gimbal_attitude_msg{
                    .gimbal_pitch = payload_info->gimbal_pitch,
                    .gimbal_roll = payload_info->gimbal_roll,
                    .gimbal_yaw = payload_info->gimbal_yaw,
                };
                msg.payloads_gimbal_attitude[payload_info->payload_index] = payload_gimbal_attitude_msg;
            }
        }

    }
};