#pragma once

#include "topic_engine/TopicMessageWrapper.h"
#include "base/logger/logger.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

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
    uint64_t progress_version = 0; // 消息的时间戳，用来处理乱序问题
    std::string resumable_status;/*ResumableStatus*/
    std::string obstacle_avoidance_notify;/*ObstacleNotifyType*/
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

        MessageParserHook();
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
private:
    bool is_valid_ = true;

    void MessageParserHook() {
        // 使用rapidjson手动解析数组
        rapidjson::Document doc;
        doc.Parse(message_data.c_str());

        NC_LOG_INFO("PublishFlightTasksTopic MessageParserHook: %s", message_data.c_str());
        
        if (doc.HasParseError()) {
            NC_LOG_INFO("解析FlightTasks JSON失败: 解析错误");
            is_valid_ = false;
            return;
        }
        
        if (!doc.IsArray()) {
            NC_LOG_INFO("解析FlightTasks JSON失败: 不是数组格式");
            is_valid_ = false;
            return;
        }
        
        // 清空现有数据
        msg.flight_tasks.clear();
        
        // 遍历数组中的每个元素
        for (rapidjson::SizeType i = 0; i < doc.Size(); i++) {
            const rapidjson::Value& item = doc[i];
            
            // 将数组元素转换为字符串
            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            item.Accept(writer);
            std::string item_str = buffer.GetString();
            
            // 使用AIGCJson解析为FlightTaskMsg结构
            FlightTaskMsg flight_task;
            std::string err;
            aigc::JsonHelper::JsonToObject(flight_task, item_str, {}, &err);
            
            if (!err.empty()) {
                NC_LOG_INFO("解析FlightTask[%d]失败: %s", i, err.c_str());
                continue; // 跳过这个元素，继续解析其他元素
            }
            
            // 添加到结果数组中
            msg.flight_tasks.push_back(flight_task);
        }
        
        is_valid_ = true;
    }
};