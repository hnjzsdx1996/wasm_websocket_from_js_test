#pragma once
#include <string>
#include <vector>

#include "AIGCJson.hpp"
#include "base/logger/logger.h"

// 机场 OSD 消息：物模型数据，类型 2
/*
{
    "host": {
        "acc_time": 7088705,
        "acdc_power_input": 47.77777862548828,
        "activation_time": 0,
        "backup_battery": {
            "switch": 1,
            "temperature": 38.9,
            "voltage": 13011
        },
        "deployment_mode": 1,
        "drone_battery_maintenance_info": {
            "batteries": [
                {
                    "capacity_percent": 32767,
                    "index": 0,
                    "temperature": 3276.7,
                    "voltage": 32767
                }
            ],
            "heat_state": 0,
            "maintenance_state": 0,
            "maintenance_time_left": 0
        },
        "electric_supply_voltage": 229,
        "gimbal_holder_state": 0,
        "job_number": 121,
        "maintain_status": {
            "maintain_status_array": [
                {
                    "last_maintain_time": 0,
                    "last_maintain_type": 17,
                    "last_maintain_work_sorties": 0,
                    "state": 0
                },
                {
                    "last_maintain_time": 0,
                    "last_maintain_type": 18,
                    "last_maintain_work_sorties": 0,
                    "state": 0
                }
            ]
        },
        "poe_link_status": 0,
        "poe_power_output": 0,
        "relative_alternate_land_point": {
            "latitude": 0,
            "longitude": 0,
            "safe_land_height": 30,
            "status": 1
        },
        "self_converge_coordinate": {
            "height": 0,
            "latitude": 0,
            "longitude": 0
        },
        "temp_mode_state": false,
        "working_current": 900,
        "working_voltage": 48006
    },
    "sn": "8UUXN2D00A00VL"
}
*/

// 备用电池信息
class BackupBattery {
public:
    int switch_state = 0;
    double temperature = 0.0;
    int voltage = 0;

    AIGC_JSON_HELPER(switch_state, temperature, voltage);
};

// 电池维护信息
class BatteryMaintenanceInfo {
public:
    int capacity_percent = 0;
    int index = 0;
    double temperature = 0.0;
    int voltage = 0;

    AIGC_JSON_HELPER(capacity_percent, index, temperature, voltage);
};

// 无人机电池维护信息
class DroneBatteryMaintenanceInfo {
public:
    std::vector<BatteryMaintenanceInfo> batteries;
    int heat_state = 0;
    int maintenance_state = 0;
    int maintenance_time_left = 0;

    AIGC_JSON_HELPER(batteries, heat_state, maintenance_state, maintenance_time_left);
};

// 维护状态信息
class MaintainStatusItem {
public:
    int64_t last_maintain_time = 0;
    int last_maintain_type = 0;
    int last_maintain_work_sorties = 0;
    int state = 0;

    AIGC_JSON_HELPER(last_maintain_time, last_maintain_type, last_maintain_work_sorties, state);
};

// 维护状态
class MaintainStatus {
public:
    std::vector<MaintainStatusItem> maintain_status_array;

    AIGC_JSON_HELPER(maintain_status_array);
};

// 相对备用降落点
class RelativeAlternateLandPoint {
public:
    double latitude = 0.0;
    double longitude = 0.0;
    double safe_land_height = 30.0;
    int status = 0;

    AIGC_JSON_HELPER(latitude, longitude, safe_land_height, status);
};

// 自收敛坐标
class SelfConvergeCoordinate {
public:
    double height = 0.0;
    double latitude = 0.0;
    double longitude = 0.0;

    AIGC_JSON_HELPER(height, latitude, longitude);
};

// 机场物模型类型2主机信息
class DeviceOsdAirportThingPropertiesType2HostMsg {
public:
    int64_t acc_time = 0;
    double acdc_power_input = 0.0;
    int64_t activation_time = 0;
    BackupBattery backup_battery;
    int deployment_mode = 0;
    DroneBatteryMaintenanceInfo drone_battery_maintenance_info;
    int electric_supply_voltage = 0;
    int gimbal_holder_state = 0;
    int job_number = 0;
    MaintainStatus maintain_status;
    int poe_link_status = 0;
    int poe_power_output = 0;
    RelativeAlternateLandPoint relative_alternate_land_point;
    SelfConvergeCoordinate self_converge_coordinate;
    bool temp_mode_state = false;
    int working_current = 0;
    int working_voltage = 0;

    AIGC_JSON_HELPER(acc_time, acdc_power_input, activation_time, backup_battery, deployment_mode,
                     drone_battery_maintenance_info, electric_supply_voltage, gimbal_holder_state,
                     job_number, maintain_status, poe_link_status, poe_power_output,
                     relative_alternate_land_point, self_converge_coordinate, temp_mode_state,
                     working_current, working_voltage);
};

// 机场OSD消息结构
class DeviceOsdAirportThingPropertiesType2Msg {
public:
    DeviceOsdAirportThingPropertiesType2HostMsg host;
    std::string sn;

    AIGC_JSON_HELPER(host, sn);
};

// 用于解析机场 osd 消息，类型为物模型，type2
class DeviceOsdAirportThingPropertiesType2 {
public:

    explicit DeviceOsdAirportThingPropertiesType2(const std::string &json_string) {
        json_string_ = json_string;
        analyze();
    }

    // 获取设备序列号
    std::string getSn() const {
        return msg.sn;
    }

    // 获取主机信息
    const DeviceOsdAirportThingPropertiesType2HostMsg& getHost() const {
        return msg.host;
    }

    // 获取ACC时间
    int64_t getAccTime() const {
        return msg.host.acc_time;
    }

    // 获取ACDC电源输入
    double getAcdcPowerInput() const {
        return msg.host.acdc_power_input;
    }

    // 获取激活时间
    int64_t getActivationTime() const {
        return msg.host.activation_time;
    }

    // 获取备用电池信息
    const BackupBattery& getBackupBattery() const {
        return msg.host.backup_battery;
    }

    // 获取部署模式
    int getDeploymentMode() const {
        return msg.host.deployment_mode;
    }

    // 获取无人机电池维护信息
    const DroneBatteryMaintenanceInfo& getDroneBatteryMaintenanceInfo() const {
        return msg.host.drone_battery_maintenance_info;
    }

    // 获取电源电压
    int getElectricSupplyVoltage() const {
        return msg.host.electric_supply_voltage;
    }

    // 获取云台支架状态
    int getGimbalHolderState() const {
        return msg.host.gimbal_holder_state;
    }

    // 获取作业编号
    int getJobNumber() const {
        return msg.host.job_number;
    }

    // 获取维护状态
    const MaintainStatus& getMaintainStatus() const {
        return msg.host.maintain_status;
    }

    // 获取POE链路状态
    int getPoeLinkStatus() const {
        return msg.host.poe_link_status;
    }

    // 获取POE电源输出
    int getPoePowerOutput() const {
        return msg.host.poe_power_output;
    }

    // 获取相对备用降落点
    const RelativeAlternateLandPoint& getRelativeAlternateLandPoint() const {
        return msg.host.relative_alternate_land_point;
    }

    // 获取自收敛坐标
    const SelfConvergeCoordinate& getSelfConvergeCoordinate() const {
        return msg.host.self_converge_coordinate;
    }

    // 获取温度模式状态
    bool getTempModeState() const {
        return msg.host.temp_mode_state;
    }

    // 获取工作电流
    int getWorkingCurrent() const {
        return msg.host.working_current;
    }

    // 获取工作电压
    int getWorkingVoltage() const {
        return msg.host.working_voltage;
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
            NC_LOG_ERROR("DeviceOsdAirportThingPropertiesType2 analyze error, err: %s, json: %s", err.c_str(), json_string_.c_str());
            parsed_successfully_ = false;
            return;
        }
        
        if (msg.sn.empty()) {
            NC_LOG_ERROR("DeviceOsdAirportThingPropertiesType2 analyze error, sn is empty");
            parsed_successfully_ = false;
            return;
        }
        
        parsed_successfully_ = true;
        NC_LOG_DEBUG("DeviceOsdAirportThingPropertiesType2: successfully parsed dock data for SN: %s", msg.sn.c_str());
    }

    std::string json_string_;
    DeviceOsdAirportThingPropertiesType2Msg msg;
    bool parsed_successfully_ = false;
};