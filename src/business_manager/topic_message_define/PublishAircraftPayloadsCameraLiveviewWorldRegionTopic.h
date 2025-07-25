#pragma once

#include "topic_engine/TopicMessageWrapper.h"

class CameraLiveviewWorldRegionMsg {
public:
    double bottom = 0.0;
    double left = 0.0;
    double right = 0.0;
    double top = 0.0;

    AIGC_JSON_HELPER(bottom, left, right, top);
};

class AircraftPayloadsCameraLiveviewWorldRegionMsg {
public:
    std::unordered_map<std::string, CameraLiveviewWorldRegionMsg> payloads_list;

    AIGC_JSON_HELPER(payloads_list);
};

class PublishAircraftPayloadsCameraLiveviewWorldRegionTopic : public PublishTopicWrapper {
public:
    bool isValid() override {
        return is_valid_;
    }

    explicit PublishAircraftPayloadsCameraLiveviewWorldRegionTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        MessageParserHook();
    }

    AircraftPayloadsCameraLiveviewWorldRegionMsg msg;

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
        auto payloads_indexes = aircraft_parser.getCameras();

        for (const auto& camera_info: payloads_indexes) {
            msg.payloads_list[camera_info.payload_index] = CameraLiveviewWorldRegionMsg{
                .bottom = camera_info.liveview_world_region.bottom,
                .left = camera_info.liveview_world_region.left,
                .right = camera_info.liveview_world_region.right,
                .top = camera_info.liveview_world_region.top,
            };
        }

    }
};