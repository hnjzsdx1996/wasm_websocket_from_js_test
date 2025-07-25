#pragma once

#include "parser/DeviceOsdAircraft.h"
#include "parser/DeviceOsdDock.h"
#include "parser/DeviceOsdType.h"
#include "topic_engine/TopicMessageWrapper.h"

// 电池信息
class BatteryInfoMsg {
public:
    int capacity_percent = 0;
    std::string firmware_version;
    int high_voltage_storage_days = 0;
    int index = 0;
    int loop_times = 0;
    std::string sn;
    int sub_type = 0;
    double temperature = 0.0;
    int type = 0;
    int voltage = 0;

    AIGC_JSON_HELPER(capacity_percent, firmware_version, high_voltage_storage_days,
                     index, loop_times, sn, sub_type, temperature, type, voltage);
};

// 电池组信息
class AircraftBatteryInfoMsg {
public:
    std::vector<BatteryInfoMsg> batteries;
    int capacity_percent = 0;
    int landing_power = 0;
    int remain_flight_time = 0;
    int return_home_power = 0;

    AIGC_JSON_HELPER(batteries, capacity_percent, landing_power, remain_flight_time, return_home_power);
};

class PublishAircraftBatteryInfoTopic : public PublishTopicWrapper {
public:
    bool isValid() override {
        return is_valid_;
    }

    explicit PublishAircraftBatteryInfoTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        MessageParserHook();
    }

    AircraftBatteryInfoMsg msg;

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
        const auto batteries = aircraft_parser.getBatteryGroup().batteries;
        for (const auto& battery : batteries) {
            BatteryInfoMsg battery_info_msg{
                .capacity_percent = battery.capacity_percent,
                .firmware_version = battery.firmware_version,
                .high_voltage_storage_days = battery.high_voltage_storage_days,
                .index = battery.index,
                .loop_times = battery.loop_times,
                .sn = battery.sn,
                .sub_type = battery.sub_type,
                .temperature = battery.temperature,
                .type = battery.type,
                .voltage = battery.voltage,
            };
            msg.batteries.emplace_back(battery_info_msg);
        }
        msg.capacity_percent = aircraft_parser.getBatteryGroup().capacity_percent;
        msg.landing_power = aircraft_parser.getBatteryGroup().landing_power;
        msg.remain_flight_time = aircraft_parser.getBatteryGroup().remain_flight_time;
        msg.return_home_power = aircraft_parser.getBatteryGroup().return_home_power;
    }
};