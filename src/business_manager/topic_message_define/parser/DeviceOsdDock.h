#pragma once
#include <string>

#include "AIGCJson.hpp"
#include "base/logger/logger.h"


// 机场 OSD 消息：OSD 数据
/*
{
    "host": {
        "air_conditioner": {
            "air_conditioner_state": 0,
            "switch_time": 0
        },
        "alarm_state": 0,
        "alternate_land_point": {
            "height": 0,
            "is_configured": 0,
            "latitude": 0,
            "longitude": 0,
            "safe_land_height": 30
        },
        "battery_store_mode": 2,
        "cover_state": 0,
        "drone_charge_state": {
            "capacity_percent": 82,
            "state": 0
        },
        "drone_in_dock": 0,
        "emergency_stop_state": 0,
        "environment_temperature": 28.6,
        "first_power_on": 1631945855969,
        "heading": 0,
        "height": 0,
        "home_position_is_valid": 0,
        "humidity": 62,
        "latitude": 0,
        "longitude": 0,
        "mode_code": 5,
        "network_state": {
            "quality": 0,
            "rate": 78,
            "type": 2
        },
        "position_state": {
            "gps_number": 0,
            "is_calibration": 0,
            "is_fixed": 3,
            "quality": 1,
            "rtk_number": 0
        },
        "putter_state": 0,
        "rainfall": 0,
        "silent_mode": 0,
        "storage": {
            "total": 53082240,
            "used": 2186712
        },
        "sub_device": {
            "device_model_key": "0-100-1",
            "device_online_status": 1,
            "device_paired": 1,
            "device_sn": "1581F8HGD25110010011"
        },
        "supplement_light_state": 0,
        "temperature": 36.6,
        "wind_speed": 0
    },
    "sn": "8UUDMAQ00A0047"
}
*/

// 空调状态
class AirConditioner {
public:
    int air_conditioner_state = 0;
    int64_t switch_time = 0;

    AIGC_JSON_HELPER(air_conditioner_state, switch_time);
};

// 备用降落点
class AlternateLandPoint {
public:
    double height = 0.0;
    int is_configured = 0;
    double latitude = 0.0;
    double longitude = 0.0;
    double safe_land_height = 30.0;

    AIGC_JSON_HELPER(height, is_configured, latitude, longitude, safe_land_height);
};

// 无人机充电状态
class DroneChargeState {
public:
    int capacity_percent = 0;
    int state = 0;

    AIGC_JSON_HELPER(capacity_percent, state);
};

// 网络状态
class NetworkState {
public:
    int quality = 0;
    int rate = 0;
    int type = 0;

    AIGC_JSON_HELPER(quality, rate, type);
};

// 位置状态
class PositionState {
public:
    int gps_number = 0;
    int is_calibration = 0;
    int is_fixed = 0;
    int quality = 0;
    int rtk_number = 0;

    AIGC_JSON_HELPER(gps_number, is_calibration, is_fixed, quality, rtk_number);
};

// 存储信息
class Storage {
public:
    int64_t total = 0;
    int64_t used = 0;

    AIGC_JSON_HELPER(total, used);
};

// 子设备信息
class SubDevice {
public:
    std::string device_model_key;
    int device_online_status = 0;
    int device_paired = 0;
    std::string device_sn;

    AIGC_JSON_HELPER(device_model_key, device_online_status, device_paired, device_sn);
};

// 主机信息
class DeviceOsdDockHostMsg {
public:
    AirConditioner air_conditioner;
    int alarm_state = 0;
    AlternateLandPoint alternate_land_point;
    int battery_store_mode = 0;
    int cover_state = 0;
    DroneChargeState drone_charge_state;
    int drone_in_dock = 0;
    int emergency_stop_state = 0;
    double environment_temperature = 0.0;
    int64_t first_power_on = 0;
    double heading = 0.0;
    double height = 0.0;
    int home_position_is_valid = 0;
    int humidity = 0;
    double latitude = 0.0;
    double longitude = 0.0;
    int mode_code = 0;
    NetworkState network_state;
    PositionState position_state;
    int putter_state = 0;
    int rainfall = 0;
    int silent_mode = 0;
    Storage storage;
    SubDevice sub_device;
    int supplement_light_state = 0;
    double temperature = 0.0;
    double wind_speed = 0.0;

    AIGC_JSON_HELPER(air_conditioner, alarm_state, alternate_land_point, battery_store_mode, 
                     cover_state, drone_charge_state, drone_in_dock, emergency_stop_state,
                     environment_temperature, first_power_on, heading, height, 
                     home_position_is_valid, humidity, latitude, longitude, mode_code,
                     network_state, position_state, putter_state, rainfall, silent_mode,
                     storage, sub_device, supplement_light_state, temperature, wind_speed);
};

// 机场OSD消息结构
class DeviceOsdDockMsg {
public:
    DeviceOsdDockHostMsg host;
    std::string sn;

    AIGC_JSON_HELPER(host, sn);
};

// 用于解析机场 osd 消息
class DeviceOsdDock {
public:

    explicit DeviceOsdDock(const std::string &json_string) {
        json_string_ = json_string;
        analyze();
    }

    int getDroneInDock() {
        return msg.host.drone_in_dock;
    }

    // 获取设备序列号
    std::string getSn() const {
        return msg.sn;
    }

    // 获取主机信息
    const DeviceOsdDockHostMsg& getHost() const {
        return msg.host;
    }

    // 获取空调状态
    const AirConditioner& getAirConditioner() const {
        return msg.host.air_conditioner;
    }

    // 获取备用降落点
    const AlternateLandPoint& getAlternateLandPoint() const {
        return msg.host.alternate_land_point;
    }

    // 获取无人机充电状态
    const DroneChargeState& getDroneChargeState() const {
        return msg.host.drone_charge_state;
    }

    // 获取网络状态
    const NetworkState& getNetworkState() const {
        return msg.host.network_state;
    }

    // 获取位置状态
    const PositionState& getPositionState() const {
        return msg.host.position_state;
    }

    // 获取存储信息
    const Storage& getStorage() const {
        return msg.host.storage;
    }

    // 获取子设备信息
    const SubDevice& getSubDevice() const {
        return msg.host.sub_device;
    }

    // 获取环境温度
    double getEnvironmentTemperature() const {
        return msg.host.environment_temperature;
    }

    // 获取湿度
    int getHumidity() const {
        return msg.host.humidity;
    }

    // 获取温度
    double getTemperature() const {
        return msg.host.temperature;
    }

    // 获取风速
    double getWindSpeed() const {
        return msg.host.wind_speed;
    }

    // 获取降雨量
    int getRainfall() const {
        return msg.host.rainfall;
    }

    // 获取模式代码
    int getModeCode() const {
        return msg.host.mode_code;
    }

    // 获取航向
    double getHeading() const {
        return msg.host.heading;
    }

    // 获取高度
    double getHeight() const {
        return msg.host.height;
    }

    // 获取纬度
    double getLatitude() const {
        return msg.host.latitude;
    }

    // 获取经度
    double getLongitude() const {
        return msg.host.longitude;
    }

    // 获取首次开机时间
    int64_t getFirstPowerOn() const {
        return msg.host.first_power_on;
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
            NC_LOG_ERROR("DeviceOsdDock analyze error, err: %s, json: %s", err.c_str(), json_string_.c_str());
            parsed_successfully_ = false;
            return;
        }
        
        if (msg.sn.empty()) {
            NC_LOG_ERROR("DeviceOsdDock analyze error, sn is empty");
            parsed_successfully_ = false;
            return;
        }
        
        parsed_successfully_ = true;
        NC_LOG_DEBUG("DeviceOsdDock: successfully parsed dock data for SN: %s, drone_in_dock: %d", msg.sn.c_str(), msg.host.drone_in_dock);
    }

    std::string json_string_;
    DeviceOsdDockMsg msg;
    bool parsed_successfully_ = false;
};