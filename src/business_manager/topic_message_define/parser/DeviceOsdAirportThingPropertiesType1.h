#pragma once
#include <string>

#include "AIGCJson.hpp"
#include "base/logger/logger.h"

// 机场 OSD 消息：物模型数据，类型 1
/*
{
    "host": {
        "departure_point": {
            "height": 0,
            "latitude": 0,
            "longitude": 0
        },
        "departure_point_status": 0,
        "drc_state": 0,
        "flighttask_prepare_capacity": 1,
        "flighttask_step_code": 5,
        "media_file_detail": {
            "remain_upload": 0
        },
        "sdr": {
            "down_quality": 0,
            "frequency_band": 5.8,
            "up_quality": 0
        },
        "tilt_angle": {
            "valid": 1,
            "value": 0.1908319890499115
        },
        "user_experience_improvement": 2,
        "wireless_link": {
            "4g_freq_band": 5.8,
            "4g_gnd_quality": 65535,
            "4g_link_state": 0,
            "4g_quality": 65535,
            "4g_uav_quality": 65535,
            "dongle_number": 0,
            "link_workmode": 1,
            "sdr_freq_band": 5.8,
            "sdr_link_state": 0,
            "sdr_quality": 65535
        }
    },
    "sn": "8UUXN2D00A00VL"
}
*/

// 起飞点信息
class DeparturePoint {
public:
    double height = 0.0;
    double latitude = 0.0;
    double longitude = 0.0;

    AIGC_JSON_HELPER(height, latitude, longitude);
};

// 媒体文件详情
class MediaFileDetail {
public:
    int remain_upload = 0;

    AIGC_JSON_HELPER(remain_upload);
};

// SDR信息
class Sdr {
public:
    int down_quality = 0;
    double frequency_band = 0.0;
    int up_quality = 0;

    AIGC_JSON_HELPER(down_quality, frequency_band, up_quality);
};

// 倾斜角度
class TiltAngle {
public:
    int valid = 0;
    double value = 0.0;

    AIGC_JSON_HELPER(valid, value);
};

// 无线链路信息
class WirelessLink {
public:
    double freq_band_4g = 0.0;
    int gnd_quality_4g = 0;
    int link_state_4g = 0;
    int quality_4g = 0;
    int uav_quality_4g = 0;
    int dongle_number = 0;
    int link_workmode = 0;
    double sdr_freq_band = 0.0;
    int sdr_link_state = 0;
    int sdr_quality = 0;

    AIGC_JSON_HELPER(freq_band_4g, gnd_quality_4g, link_state_4g, quality_4g, uav_quality_4g,
                     dongle_number, link_workmode, sdr_freq_band, sdr_link_state, sdr_quality);
};

// 机场物模型类型1主机信息
class DeviceOsdAirportThingPropertiesType1HostMsg {
public:
    DeparturePoint departure_point;
    int departure_point_status = 0;
    int drc_state = 0;
    int flighttask_prepare_capacity = 0;
    int flighttask_step_code = 0;
    MediaFileDetail media_file_detail;
    Sdr sdr;
    TiltAngle tilt_angle;
    int user_experience_improvement = 0;
    WirelessLink wireless_link;

    AIGC_JSON_HELPER(departure_point, departure_point_status, drc_state, flighttask_prepare_capacity,
                     flighttask_step_code, media_file_detail, sdr, tilt_angle, 
                     user_experience_improvement, wireless_link);
};

// 机场OSD消息结构
class DeviceOsdAirportThingPropertiesType1Msg {
public:
    DeviceOsdAirportThingPropertiesType1HostMsg host;
    std::string sn;

    AIGC_JSON_HELPER(host, sn);
};

// 用于解析机场 osd 消息，类型为物模型，type1
class DeviceOsdAirportThingPropertiesType1 {
public:

    explicit DeviceOsdAirportThingPropertiesType1(const std::string &json_string) {
        json_string_ = json_string;
        analyze();
    }

    // 获取设备序列号
    std::string getSn() const {
        return msg.sn;
    }

    // 获取主机信息
    const DeviceOsdAirportThingPropertiesType1HostMsg& getHost() const {
        return msg.host;
    }

    // 获取起飞点信息
    const DeparturePoint& getDeparturePoint() const {
        return msg.host.departure_point;
    }

    // 获取起飞点状态
    int getDeparturePointStatus() const {
        return msg.host.departure_point_status;
    }

    // 获取DRC状态
    int getDrcState() const {
        return msg.host.drc_state;
    }

    // 获取飞行任务准备容量
    int getFlighttaskPrepareCapacity() const {
        return msg.host.flighttask_prepare_capacity;
    }

    // 获取飞行任务步骤代码
    int getFlighttaskStepCode() const {
        return msg.host.flighttask_step_code;
    }

    // 获取媒体文件详情
    const MediaFileDetail& getMediaFileDetail() const {
        return msg.host.media_file_detail;
    }

    // 获取SDR信息
    const Sdr& getSdr() const {
        return msg.host.sdr;
    }

    // 获取倾斜角度
    const TiltAngle& getTiltAngle() const {
        return msg.host.tilt_angle;
    }

    // 获取用户体验改进
    int getUserExperienceImprovement() const {
        return msg.host.user_experience_improvement;
    }

    // 获取无线链路信息
    const WirelessLink& getWirelessLink() const {
        return msg.host.wireless_link;
    }

    // 检查解析是否成功
    bool isParsedSuccessfully() const {
        return parsed_successfully_;
    }

private:
    void analyze() {
        std::string err;
        aigc::JsonHelper::JsonToObject(msg, json_string_, {}, &err);
        if (!err.empty()) {
            NC_LOG_ERROR("DeviceOsdAirportThingPropertiesType1 analyze error, err: %s, json: %s", err.c_str(), json_string_.c_str());
            parsed_successfully_ = false;
            return;
        }
        
        if (msg.sn.empty()) {
            NC_LOG_ERROR("DeviceOsdAirportThingPropertiesType1 analyze error, sn is empty");
            parsed_successfully_ = false;
            return;
        }
        
        parsed_successfully_ = true;
        NC_LOG_DEBUG("DeviceOsdAirportThingPropertiesType1: successfully parsed dock data for SN: %s", msg.sn.c_str());
    }

    std::string json_string_;
    DeviceOsdAirportThingPropertiesType1Msg msg;
    bool parsed_successfully_ = false;
};