#pragma once
#include <string>

#include "AIGCJson.hpp"
#include "base/logger/logger.h"
#include "rapidjson/document.h"

class DeviceOsdTypeSn {
public:
    std::string sn;

    AIGC_JSON_HELPER(sn);
};

enum class DeviceOsdTypeEnum {
    DeviceOsdUnknown, // 未知
    DeviceOsdAircraft, // 飞机 OSD
    DeviceOsdDock, // 机场 OSD
    DeviceOsdAirportThingPropertiesType1, // 机场物模型类型 1: 包含departure_point
    DeviceOsdAirportThingPropertiesType2, // 机场物模型类型 2: 包含acc_time
};

// 用于判断 device_osd 来源是飞机、机场还是其他设备
class DeviceOsdType {
public:

    explicit DeviceOsdType(const std::string &json_string) {
        json_string_ = json_string;
        analyze();
    }

    [[deprecated("请使用getDeviceOsdTypeEnum函数, 该函数提供更丰富 OSD 类型信息")]]
    bool isDrone() const {
        return is_drone_;
    }

    DeviceOsdTypeEnum getDeviceOsdTypeEnum() const {
        return osd_type_;
    }

private:
    void analyze() {
        DeviceOsdTypeSn sn;

        std::string err;
        aigc::JsonHelper::JsonToObject(sn, json_string_, {}, &err);
        if (!err.empty()) {
            NC_LOG_ERROR("DeviceOsdType analyze error, err: %s, json: %s", err.c_str(), json_string_.c_str());
            return;
        }
        if (sn.sn.empty()) {
            NC_LOG_ERROR("DeviceOsdType analyze error, sn is empty");
            return;
        }
        
        // 判断设备类型：如果 sn 开头是 1581F，那么就是飞机
        if (sn.sn.length() >= 5 && sn.sn.substr(0, 5) == "1581F") {
            is_drone_ = true;
            NC_LOG_DEBUG("DeviceOsdType: detected drone device with SN: %s", sn.sn.c_str());
        } else {
            is_drone_ = false;
            NC_LOG_DEBUG("DeviceOsdType: detected non-drone device with SN: %s", sn.sn.c_str());
        }
        guessOsdType();
    }

    // 根据 json 数据的蛛丝马迹推测收到的是什么类型的数据
    void guessOsdType() {
        // 默认设置为未知类型
        osd_type_ = DeviceOsdTypeEnum::DeviceOsdUnknown;
        
        // 使用rapidjson解析JSON字符串，检查特征字段
        rapidjson::Document doc;
        doc.Parse(json_string_.c_str());
        
        if (doc.HasParseError()) {
            NC_LOG_ERROR("DeviceOsdType guessOsdType: JSON parse error");
            return;
        }
        
        // 检查是否有host字段
        if (!doc.HasMember("host") || !doc["host"].IsObject()) {
            NC_LOG_DEBUG("DeviceOsdType guessOsdType: no host field or host is not object");
            return;
        }
        
        const rapidjson::Value& host = doc["host"];
        
        // 检查机场物模型类型1的特征字段：departure_point
        if (host.HasMember("departure_point")) {
            osd_type_ = DeviceOsdTypeEnum::DeviceOsdAirportThingPropertiesType1;
            NC_LOG_DEBUG("DeviceOsdType guessOsdType: detected DeviceOsdAirportThingPropertiesType1 (departure_point)");
            return;
        }
        
        // 检查机场物模型类型2的特征字段：acc_time
        if (host.HasMember("acc_time")) {
            osd_type_ = DeviceOsdTypeEnum::DeviceOsdAirportThingPropertiesType2;
            NC_LOG_DEBUG("DeviceOsdType guessOsdType: detected DeviceOsdAirportThingPropertiesType2 (acc_time)");
            return;
        }
        
        // 检查机场OSD的特征字段：drone_in_dock
        if (host.HasMember("drone_in_dock")) {
            osd_type_ = DeviceOsdTypeEnum::DeviceOsdDock;
            NC_LOG_DEBUG("DeviceOsdType guessOsdType: detected DeviceOsdDock (drone_in_dock)");
            return;
        }
        
        // 检查飞机OSD的特征字段：attitude_head
        if (host.HasMember("attitude_head")) {
            osd_type_ = DeviceOsdTypeEnum::DeviceOsdAircraft;
            NC_LOG_DEBUG("DeviceOsdType guessOsdType: detected DeviceOsdAircraft (attitude_head)");
            return;
        }
        
        // 如果没有找到特征字段，保持未知类型
        NC_LOG_DEBUG("DeviceOsdType guessOsdType: unknown OSD type, no characteristic fields found");
    }

    std::string json_string_;

    bool is_drone_ = false;
    DeviceOsdTypeEnum osd_type_;
};
