#pragma once
#include <string>
#include <vector>
#include <map>

#include "AIGCJson.hpp"
#include "base/logger/logger.h"


// 飞机 OSD 消息：OSD 数据
/*
{
    "host": {
        "81-0-0": {
            "gimbal_pitch": 0,
            "gimbal_roll": 0,
            "gimbal_yaw": 121.08843265722754,
            "payload_index": "81-0-0",
            "thermal_current_palette_style": 0,
            "thermal_gain_mode": 2,
            "thermal_global_temperature_max": 19.100000381469727,
            "thermal_global_temperature_min": 5.300000190734863,
            "thermal_isotherm_lower_limit": -20,
            "thermal_isotherm_state": 0,
            "thermal_isotherm_upper_limit": 150,
            "zoom_factor": 0.5678233438485805
        },
        "activation_time": 0,
        "attitude_head": 121,
        "attitude_pitch": -4.3,
        "attitude_roll": 0.9,
        "battery": {
            "batteries": [
                {
                    "capacity_percent": 81,
                    "firmware_version": "26.03.08.49",
                    "high_voltage_storage_days": 3,
                    "index": 0,
                    "loop_times": 520,
                    "sn": "6Q7PL89DAP00H5",
                    "sub_type": 0,
                    "temperature": 41.9,
                    "type": 0,
                    "voltage": 15775
                }
            ],
            "capacity_percent": 81,
            "landing_power": 8,
            "remain_flight_time": 1986,
            "return_home_power": 25
        },
        "best_link_gateway": "6QCDL810020011",
        "cameras": [
            {
                "camera_mode": 2,
                "ir_metering_mode": 0,
                "ir_metering_point": {
                    "temperature": 0,
                    "x": 0.5,
                    "y": 0.5
                },
                "ir_zoom_factor": 2,
                "liveview_world_region": {
                    "bottom": 0.552438497543335,
                    "left": 0.4323032796382904,
                    "right": 0.5633704662322998,
                    "top": 0.4231345057487488
                },
                "payload_index": "81-0-0",
                "photo_state": 0,
                "photo_storage_settings": [
                    "vision"
                ],
                "record_time": 0,
                "recording_state": 0,
                "remain_photo_num": 9113,
                "remain_record_duration": 0,
                "screen_split_enable": false,
                "wide_calibrate_farthest_focus_value": 36,
                "wide_calibrate_nearest_focus_value": 66,
                "wide_exposure_mode": 1,
                "wide_exposure_value": 16,
                "wide_focus_mode": 0,
                "wide_focus_state": 0,
                "wide_focus_value": 36,
                "wide_iso": 3,
                "wide_max_focus_value": 66,
                "wide_min_focus_value": 35,
                "wide_shutter_speed": 3,
                "zoom_calibrate_farthest_focus_value": 36,
                "zoom_calibrate_nearest_focus_value": 66,
                "zoom_exposure_mode": 1,
                "zoom_exposure_value": 16,
                "zoom_factor": 6.999994214380596,
                "zoom_focus_mode": 0,
                "zoom_focus_state": 0,
                "zoom_focus_value": 36,
                "zoom_iso": 3,
                "zoom_max_focus_value": 66,
                "zoom_min_focus_value": 35,
                "zoom_shutter_speed": 3
            }
        ],
        "country": "CN",
        "distance_limit_status": {
            "distance_limit": 300,
            "is_near_distance_limit": 0,
            "state": 1
        },
        "elevation": 78.9,
        "gear": 1,
        "height": 128.22119750976563,
        "height_limit": 80,
        "home_distance": 0.08395953476428986,
        "horizontal_speed": 0,
        "is_near_area_limit": 1,
        "is_near_height_limit": 1,
        "latitude": 22.793217667699324,
        "longitude": 114.35788532514945,
        "maintain_status": {
            "maintain_status_array": [
                {
                    "last_maintain_flight_sorties": 0,
                    "last_maintain_flight_time": 0,
                    "last_maintain_time": 0,
                    "last_maintain_type": 1,
                    "state": 0
                },
                {
                    "last_maintain_flight_sorties": 0,
                    "last_maintain_flight_time": 0,
                    "last_maintain_time": 0,
                    "last_maintain_type": 2,
                    "state": 0
                },
                {
                    "last_maintain_flight_sorties": 0,
                    "last_maintain_flight_time": 0,
                    "last_maintain_time": 0,
                    "last_maintain_type": 3,
                    "state": 0
                }
            ]
        },
        "mode_code": 3,
        "night_lights_state": 0,
        "obstacle_avoidance": {
            "downside": 1,
            "horizon": 1,
            "upside": 1
        },
        "position_state": {
            "gps_number": 32,
            "is_fixed": 2,
            "quality": 5,
            "rtk_number": 48
        },
        "rc_lost_action": 2,
        "rid_state": true,
        "rth_altitude": 300,
        "storage": {
            "total": 60068000,
            "used": 3161000
        },
        "total_flight_distance": 5213978.459460843,
        "total_flight_sorties": 1099,
        "total_flight_time": 612889.8263008446,
        "track_id": "1e6cbb25-52b6-4de5-8db8-6f82af559d77",
        "vertical_speed": 0,
        "wind_direction": 4,
        "wind_speed": 54
    },
    "sn": "1581F6Q8D242100CPKTJ"
}
*/

// 云台信息
class GimbalInfo {
public:
    double gimbal_pitch = 0.0;
    double gimbal_roll = 0.0;
    double gimbal_yaw = 0.0;
    std::string payload_index;
    int thermal_current_palette_style = 0;
    int thermal_gain_mode = 0;
    double thermal_global_temperature_max = 0.0;
    double thermal_global_temperature_min = 0.0;
    int thermal_isotherm_lower_limit = 0;
    int thermal_isotherm_state = 0;
    int thermal_isotherm_upper_limit = 0;
    double zoom_factor = 0.0;

    AIGC_JSON_HELPER(gimbal_pitch, gimbal_roll, gimbal_yaw, payload_index, 
                     thermal_current_palette_style, thermal_gain_mode, 
                     thermal_global_temperature_max, thermal_global_temperature_min,
                     thermal_isotherm_lower_limit, thermal_isotherm_state, 
                     thermal_isotherm_upper_limit, zoom_factor);
};

// 电池信息
class SingleBatteryInfo {
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
class BatteryGroup {
public:
    std::vector<SingleBatteryInfo> batteries;
    int capacity_percent = 0;
    int landing_power = 0;
    int remain_flight_time = 0;
    int return_home_power = 0;

    AIGC_JSON_HELPER(batteries, capacity_percent, landing_power, remain_flight_time, return_home_power);
};

// 红外测光点
class IrMeteringPoint {
public:
    double temperature = 0.0;
    double x = 0.0;
    double y = 0.0;

    AIGC_JSON_HELPER(temperature, x, y);
};

// 实时视图世界区域
class LiveviewWorldRegion {
public:
    double bottom = 0.0;
    double left = 0.0;
    double right = 0.0;
    double top = 0.0;

    AIGC_JSON_HELPER(bottom, left, right, top);
};

// 相机信息
class CameraInfo {
public:
    int camera_mode = 0;
    int ir_metering_mode = 0;
    IrMeteringPoint ir_metering_point;
    double ir_zoom_factor = 0.0;
    LiveviewWorldRegion liveview_world_region;
    std::string payload_index;
    int photo_state = 0;
    std::vector<std::string> photo_storage_settings;
    int record_time = 0;
    int recording_state = 0;
    int remain_photo_num = 0;
    int remain_record_duration = 0;
    bool screen_split_enable = false;
    int wide_calibrate_farthest_focus_value = 0;
    int wide_calibrate_nearest_focus_value = 0;
    int wide_exposure_mode = 0;
    int wide_exposure_value = 0;
    int wide_focus_mode = 0;
    int wide_focus_state = 0;
    int wide_focus_value = 0;
    int wide_iso = 0;
    int wide_max_focus_value = 0;
    int wide_min_focus_value = 0;
    int wide_shutter_speed = 0;
    int zoom_calibrate_farthest_focus_value = 0;
    int zoom_calibrate_nearest_focus_value = 0;
    int zoom_exposure_mode = 0;
    int zoom_exposure_value = 0;
    double zoom_factor = 0.0;
    int zoom_focus_mode = 0;
    int zoom_focus_state = 0;
    int zoom_focus_value = 0;
    int zoom_iso = 0;
    int zoom_max_focus_value = 0;
    int zoom_min_focus_value = 0;
    int zoom_shutter_speed = 0;

    AIGC_JSON_HELPER(camera_mode, ir_metering_mode, ir_metering_point, ir_zoom_factor,
                     liveview_world_region, payload_index, photo_state, photo_storage_settings,
                     record_time, recording_state, remain_photo_num, remain_record_duration,
                     screen_split_enable, wide_calibrate_farthest_focus_value, wide_calibrate_nearest_focus_value,
                     wide_exposure_mode, wide_exposure_value, wide_focus_mode, wide_focus_state,
                     wide_focus_value, wide_iso, wide_max_focus_value, wide_min_focus_value,
                     wide_shutter_speed, zoom_calibrate_farthest_focus_value, zoom_calibrate_nearest_focus_value,
                     zoom_exposure_mode, zoom_exposure_value, zoom_factor, zoom_focus_mode,
                     zoom_focus_state, zoom_focus_value, zoom_iso, zoom_max_focus_value,
                     zoom_min_focus_value, zoom_shutter_speed);
};

// 距离限制状态
class DistanceLimitStatus {
public:
    int distance_limit = 0;
    int is_near_distance_limit = 0;
    int state = 0;

    AIGC_JSON_HELPER(distance_limit, is_near_distance_limit, state);
};

// 维护状态项
class MaintainStatusItem {
public:
    int last_maintain_flight_sorties = 0;
    int last_maintain_flight_time = 0;
    int64_t last_maintain_time = 0;
    int last_maintain_type = 0;
    int state = 0;

    AIGC_JSON_HELPER(last_maintain_flight_sorties, last_maintain_flight_time, 
                     last_maintain_time, last_maintain_type, state);
};

// 维护状态
class MaintainStatus {
public:
    std::vector<MaintainStatusItem> maintain_status_array;

    AIGC_JSON_HELPER(maintain_status_array);
};

// 避障状态
class ObstacleAvoidance {
public:
    int downside = 0;
    int horizon = 0;
    int upside = 0;

    AIGC_JSON_HELPER(downside, horizon, upside);
};

// 位置状态
class AircraftPositionState {
public:
    int gps_number = 0;
    int is_fixed = 0;
    int quality = 0;
    int rtk_number = 0;

    AIGC_JSON_HELPER(gps_number, is_fixed, quality, rtk_number);
};

// 存储信息
class AircraftStorage {
public:
    int64_t total = 0;
    int64_t used = 0;

    AIGC_JSON_HELPER(total, used);
};

// 飞机主机信息
class DeviceOsdAircraftHostMsg {
public:
    int64_t activation_time = 0;
    double attitude_head = 0.0;
    double attitude_pitch = 0.0;
    double attitude_roll = 0.0;
    BatteryGroup battery;
    std::string best_link_gateway;
    std::vector<CameraInfo> cameras;
    std::string country;
    DistanceLimitStatus distance_limit_status;
    double elevation = 0.0;
    int gear = 0;
    double height = 0.0;
    int height_limit = 0;
    double home_distance = 0.0;
    double horizontal_speed = 0.0;
    int is_near_area_limit = 0;
    int is_near_height_limit = 0;
    double latitude = 0.0;
    double longitude = 0.0;
    MaintainStatus maintain_status;
    int mode_code = 0;
    int night_lights_state = 0;
    ObstacleAvoidance obstacle_avoidance;
    AircraftPositionState position_state;
    int rc_lost_action = 0;
    bool rid_state = false;
    int rth_altitude = 0;
    AircraftStorage storage;
    double total_flight_distance = 0.0;
    int total_flight_sorties = 0;
    double total_flight_time = 0.0;
    std::string track_id;
    double vertical_speed = 0.0;
    int wind_direction = 0;
    double wind_speed = 0.0;

    AIGC_JSON_HELPER(activation_time, attitude_head, attitude_pitch, attitude_roll,
                     battery, best_link_gateway, cameras, country, distance_limit_status,
                     elevation, gear, height, height_limit, home_distance, horizontal_speed,
                     is_near_area_limit, is_near_height_limit, latitude, longitude, maintain_status,
                     mode_code, night_lights_state, obstacle_avoidance, position_state,
                     rc_lost_action, rid_state, rth_altitude, storage, total_flight_distance,
                     total_flight_sorties, total_flight_time, track_id, vertical_speed,
                     wind_direction, wind_speed);
};

// 飞机OSD消息结构
class DeviceOsdAircraftMsg {
public:
    DeviceOsdAircraftHostMsg host;
    std::string sn;

    AIGC_JSON_HELPER(host, sn);
};

// 用于解析飞机 osd 消息
class DeviceOsdAircraft {
public:

    explicit DeviceOsdAircraft(const std::string &json_string) {
        json_string_ = json_string;
        analyze();
    }

    // 获取设备序列号
    std::string getSn() const {
        return msg.sn;
    }

    // 获取主机信息
    const DeviceOsdAircraftHostMsg& getHost() const {
        return msg.host;
    }

    // 获取云台信息（通过payload_index）
    const GimbalInfo* getGimbalInfo(const std::string& payload_index) const {
        auto it = gimbal_info_map_.find(payload_index);
        if (it != gimbal_info_map_.end()) {
            return &(it->second);
        }
        return nullptr;
    }

    // 获取所有云台信息的payload_index列表
    std::vector<std::string> getGimbalPayloadIndices() const {
        std::vector<std::string> indices;
        for (const auto& pair : gimbal_info_map_) {
            indices.push_back(pair.first);
        }
        return indices;
    }

    // 获取电池组信息
    const BatteryGroup& getBatteryGroup() const {
        return msg.host.battery;
    }

    // 获取相机列表
    const std::vector<CameraInfo>& getCameras() const {
        return msg.host.cameras;
    }

    // 获取距离限制状态
    const DistanceLimitStatus& getDistanceLimitStatus() const {
        return msg.host.distance_limit_status;
    }

    // 获取维护状态
    const MaintainStatus& getMaintainStatus() const {
        return msg.host.maintain_status;
    }

    // 获取避障状态
    const ObstacleAvoidance& getObstacleAvoidance() const {
        return msg.host.obstacle_avoidance;
    }

    // 获取位置状态
    const AircraftPositionState& getPositionState() const {
        return msg.host.position_state;
    }

    // 获取存储信息
    const AircraftStorage& getStorage() const {
        return msg.host.storage;
    }

    // 获取姿态信息
    double getAttitudeHead() const {
        return msg.host.attitude_head;
    }

    double getAttitudePitch() const {
        return msg.host.attitude_pitch;
    }

    double getAttitudeRoll() const {
        return msg.host.attitude_roll;
    }

    // 获取位置信息
    double getLatitude() const {
        return msg.host.latitude;
    }

    double getLongitude() const {
        return msg.host.longitude;
    }

    double getHeight() const {
        return msg.host.height;
    }

    double getElevation() const {
        return msg.host.elevation;
    }

    // 获取速度信息
    double getHorizontalSpeed() const {
        return msg.host.horizontal_speed;
    }

    double getVerticalSpeed() const {
        return msg.host.vertical_speed;
    }

    // 获取环境信息
    double getWindSpeed() const {
        return msg.host.wind_speed;
    }

    int getWindDirection() const {
        return msg.host.wind_direction;
    }

    // 获取飞行统计信息
    double getTotalFlightDistance() const {
        return msg.host.total_flight_distance;
    }

    int getTotalFlightSorties() const {
        return msg.host.total_flight_sorties;
    }

    double getTotalFlightTime() const {
        return msg.host.total_flight_time;
    }

    // 获取模式代码
    int getModeCode() const {
        return msg.host.mode_code;
    }

    // 获取激活时间
    int64_t getActivationTime() const {
        return msg.host.activation_time;
    }

    // 获取最佳链路网关
    std::string getBestLinkGateway() const {
        return msg.host.best_link_gateway;
    }

    // 获取国家代码
    std::string getCountry() const {
        return msg.host.country;
    }

    // 获取轨道ID
    std::string getTrackId() const {
        return msg.host.track_id;
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
            NC_LOG_ERROR("DeviceOsdAircraft analyze error, err: %s, json: %s", err.c_str(), json_string_.c_str());
            parsed_successfully_ = false;
            return;
        }
        
        if (msg.sn.empty()) {
            NC_LOG_ERROR("DeviceOsdAircraft analyze error, sn is empty");
            parsed_successfully_ = false;
            return;
        }

        // 解析动态的云台信息
        parseGimbalInfo();
        
        parsed_successfully_ = true;
        NC_LOG_DEBUG("DeviceOsdAircraft: successfully parsed aircraft data for SN: %s, mode_code: %d", msg.sn.c_str(), msg.host.mode_code);
    }

    void parseGimbalInfo() {
        // 使用rapidjson直接解析JSON来获取动态的云台信息
        rapidjson::Document doc;
        doc.Parse(json_string_.c_str());
        
        if (doc.HasParseError()) {
            NC_LOG_ERROR("DeviceOsdAircraft parseGimbalInfo error: JSON parse failed");
            return;
        }

        if (!doc.HasMember("host") || !doc["host"].IsObject()) {
            NC_LOG_ERROR("DeviceOsdAircraft parseGimbalInfo error: no host object");
            return;
        }

        const rapidjson::Value& host = doc["host"];
        
        // 从cameras数组中获取所有payload_index
        std::vector<std::string> payload_indices;
        for (const auto& camera : msg.host.cameras) {
            if (!camera.payload_index.empty()) {
                payload_indices.push_back(camera.payload_index);
            }
        }
        
        // 根据payload_index查找对应的云台信息
        for (const auto& payload_index : payload_indices) {
            if (host.HasMember(payload_index.c_str()) && host[payload_index.c_str()].IsObject()) {
                const rapidjson::Value& gimbal_obj = host[payload_index.c_str()];
                
                // 检查是否包含payload_index字段（确认这是云台信息）
                if (gimbal_obj.HasMember("payload_index")) {
                    GimbalInfo gimbal_info;
                    std::string gimbal_json;
                    
                    // 将云台对象转换为JSON字符串
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    gimbal_obj.Accept(writer);
                    gimbal_json = buffer.GetString();
                    
                    // 使用AIGCJson解析云台信息
                    std::string gimbal_err;
                    aigc::JsonHelper::JsonToObject(gimbal_info, gimbal_json, {}, &gimbal_err);
                    
                    if (gimbal_err.empty()) {
                        gimbal_info_map_[payload_index] = gimbal_info;
                        NC_LOG_DEBUG("DeviceOsdAircraft: parsed gimbal info for payload_index: %s", payload_index.c_str());
                    } else {
                        NC_LOG_ERROR("DeviceOsdAircraft: failed to parse gimbal info for payload_index %s, error: %s", 
                                    payload_index.c_str(), gimbal_err.c_str());
                    }
                }
            } else {
                NC_LOG_WARN("DeviceOsdAircraft: no gimbal info found for payload_index: %s", payload_index.c_str());
            }
        }
    }

    std::string json_string_;
    DeviceOsdAircraftMsg msg;
    std::map<std::string, GimbalInfo> gimbal_info_map_;  // 存储动态的云台信息
    bool parsed_successfully_ = false;
};