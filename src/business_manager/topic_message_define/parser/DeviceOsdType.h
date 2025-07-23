#pragma once
#include <string>

#include "AIGCJson.hpp"
#include "base/logger/logger.h"

class DeviceOsdTypeSn {
public:
    std::string sn;

    AIGC_JSON_HELPER(sn);
};

// 用于判断 device_osd 来源是飞机、机场还是其他设备
class DeviceOsdType {
public:

    explicit DeviceOsdType(const std::string &json_string) {
        json_string_ = json_string;
        analyze();
    }

    bool isDrone() const {
        return is_drone_;
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
            NC_LOG_INFO("DeviceOsdType: detected drone device with SN: %s", sn.sn.c_str());
        } else {
            is_drone_ = false;
            NC_LOG_INFO("DeviceOsdType: detected non-drone device with SN: %s", sn.sn.c_str());
        }
    }

    std::string json_string_;

    bool is_drone_ = false;
};
