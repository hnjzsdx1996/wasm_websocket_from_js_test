#pragma once

#include "topic_engine/TopicMessageWrapper.h"

class FolderInfoDataMsg {
public:
    int folder_id = 0; //  媒体文件夹id
    int expected_file_count = 0; // 媒体文件总数
    int uploaded_file_count = 0; // 媒体文件上传总数

    AIGC_JSON_HELPER(folder_id, expected_file_count, uploaded_file_count)
};


class FlightTaskMsg {
public:
    std::string uuid;
    std::string name;
    int task_type = 0;
    int status = 0;
    int progress = 0;
    std::string sn;
    FolderInfoDataMsg folder_info;
    uint64_t run_at = 0; // 任务实际运行时间
    uint64_t complete_at = 0; // 任务结束时间
    int total_waypoints = 0; // 总航线数
    int current_waypoint_index = 0; // 已飞完的航点数
    int progress_version = 0; // 消息的时间戳，用来处理乱序问题
    int resumable_status = 0;/*ResumableStatus*/
    int obstacle_avoidance_notify = 0;/*ObstacleNotifyType*/
    std::string wayline_uuid;

    AIGC_JSON_HELPER(uuid, name, task_type, status, progress, sn, folder_info, run_at, complete_at, total_waypoints,
                     current_waypoint_index, progress_version, resumable_status, obstacle_avoidance_notify,
                     wayline_uuid);
};

class FlightTasksMsg {
public:
    std::vector<FlightTaskMsg> flight_tasks;

    AIGC_JSON_HELPER(flight_tasks);
};

class PublishFlightTasksTopic : public PublishTopicWrapper {
public:
    explicit PublishFlightTasksTopic(const std::shared_ptr<PublishTopicWrapper>& publish_msg) {
        message_topic = publish_msg->message_topic;
        message_data = publish_msg->message_data;
        need_replay = publish_msg->need_replay;
        message_id = publish_msg->message_id;
        message_type = publish_msg->message_type;
        timestamp = publish_msg->timestamp;
        version = publish_msg->version;

        std::string err;
        aigc::JsonHelper::JsonToObject(msg, publish_msg->message_data, {}, &err);
        if (!err.empty()) {
            NC_LOG_ERROR("PublishDeviceOnlineStatusTopic ctor error, err: %s, json: %s", err.c_str(), publish_msg->ToJsonString().c_str());
        }
    }

    FlightTasksMsg msg;

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
};