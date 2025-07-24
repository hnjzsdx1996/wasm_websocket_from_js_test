/* notificationcenter.i */

%module(directors="1") notificationcenter
%javaconst(1);

// Enable C++11 features and directors for cross-language polymorphism
%feature("c++11");
%feature("director");

%{
#include "SDKManager.h"
#include "BusinessManager.h"
#include "WebsocketEvent.h"
#include "business_manager/BusinessManagerDefine.h"
#include "topic_engine/TopicMessageWrapper.h"
#include "business_manager/topic_message_define/PublishAircraftLocationTopic.h"
#include "business_manager/topic_message_define/PublishAircraftAttitudeTopic.h"
#include "business_manager/topic_message_define/PublishAircraftSpeedTopic.h"
#include "business_manager/topic_message_define/PublishDeviceOsdTopic.h"
#include "business_manager/topic_message_define/PublishAircraftBatteryInfoTopic.h"
#include "business_manager/topic_message_define/PublishAircraftControlCodeTopic.h"
#include "business_manager/topic_message_define/PublishAircraftWindSpeedTopic.h"
#include "business_manager/topic_message_define/PublishAircraftModeCodeTopic.h"
#include "business_manager/topic_message_define/PublishDroneInDockTopic.h"
#include "message_define/common.h"
%}

// Standard library support
%include "std_string.i"
%include "std_shared_ptr.i"
%include "std_vector.i"

// Instantiate templates for each class that will be held by a shared_ptr
%shared_ptr(SDKManager);
%shared_ptr(BusinessManager);
%shared_ptr(WebsocketEvent);

// Rename C++ entities for better Java API
%rename(ConnectionListener) WebsocketEvent;
%rename(destroy) ~SDKManager;
%rename(destroy) ~BusinessManager;

// Ignore methods we don't want to expose
%ignore SDKManager::setWebSocket;

// 定义新的AircraftLocation类
%inline %{
class AircraftLocation {
public:
    double height;    // 椭球高度
    double elevation; // 相对起飞点高度
    double latitude;  // 纬度
    double longitude; // 经度
    
    AircraftLocation() : height(0.0), elevation(0.0), latitude(0.0), longitude(0.0) {}
    AircraftLocation(double height, double elevation, double latitude, double longitude) 
        : height(height), elevation(elevation), latitude(latitude), longitude(longitude) {}
};

// 定义新的AircraftAttitude类
class AircraftAttitude {
public:
    double attitude_head;  // 机头朝向角度
    double attitude_pitch; // 俯仰轴角度
    double attitude_roll;  // 横滚轴角度
    
    AircraftAttitude() : attitude_head(0.0), attitude_pitch(0.0), attitude_roll(0.0) {}
    AircraftAttitude(double attitude_head, double attitude_pitch, double attitude_roll) 
        : attitude_head(attitude_head), attitude_pitch(attitude_pitch), attitude_roll(attitude_roll) {}
};

// 定义新的AircraftSpeed类
class AircraftSpeed {
public:
    double horizontal_speed; // 水平速度 m/s
    double vertical_speed;   // 垂直速度 m/s
    
    AircraftSpeed() : horizontal_speed(0.0), vertical_speed(0.0) {}
    AircraftSpeed(double horizontal_speed, double vertical_speed) 
        : horizontal_speed(horizontal_speed), vertical_speed(vertical_speed) {}
};

// 定义新的DeviceOsdHost类（用于Java接口）
class DeviceOsdHost {
public:
    int mode_code; // 模式代码
    double attitude_head; // 机头朝向角度
    double attitude_pitch; // 俯仰轴角度
    double attitude_roll; // 横滚轴角度
    
    DeviceOsdHost() : mode_code(0), attitude_head(0.0), attitude_pitch(0.0), attitude_roll(0.0) {}
    DeviceOsdHost(int mode_code, double attitude_head, double attitude_pitch, double attitude_roll) 
        : mode_code(mode_code), attitude_head(attitude_head), attitude_pitch(attitude_pitch), attitude_roll(attitude_roll) {}
};

// 定义新的DeviceOsd类（用于Java接口）
class DeviceOsd {
public:
    DeviceOsdHost host; // 主机信息
    std::string sn; // 设备序列号
    
    DeviceOsd() {}
    DeviceOsd(const DeviceOsdHost& host, const std::string& sn) : host(host), sn(sn) {}
};

// 定义新的BatteryInfo类
class BatteryInfo {
public:
    int capacity_percent;
    std::string firmware_version;
    int high_voltage_storage_days;
    int index;
    int loop_times;
    std::string sn;
    int sub_type;
    double temperature;
    int type;
    int voltage;
    
    BatteryInfo() : capacity_percent(0), high_voltage_storage_days(0), index(0), 
                   loop_times(0), sub_type(0), temperature(0.0), type(0), voltage(0) {}
};

// 定义新的AircraftBatteryInfo类
class AircraftBatteryInfo {
public:
    std::vector<BatteryInfo> batteries;
    int capacity_percent;
    int landing_power;
    int remain_flight_time;
    int return_home_power;
    
    AircraftBatteryInfo() : capacity_percent(0), landing_power(0), 
                           remain_flight_time(0), return_home_power(0) {}
};

// 定义新的AircraftControlCode类
class AircraftControlCode {
public:
    int control_mode; // "0": "航线控制-执行态", "1": "指令控制-手动飞行", "2": "指令控制-自动飞行", "3": "航线控制-中断态"
    
    AircraftControlCode() : control_mode(0) {}
    AircraftControlCode(int control_mode) : control_mode(control_mode) {}
};

// 定义新的AircraftWindSpeed类
class AircraftWindSpeed {
public:
    int wind_direction; // 当前风向 "enum_desc": "1:正北  2：东北  3:东  4:东南  5:南  6:西南  7:西   8:西北"
    double wind_speed; // 风速估计，该风速是通过飞机姿态推测出的，有一定的误差，仅供参考，不能作为气象数据使用
    
    AircraftWindSpeed() : wind_direction(0), wind_speed(0.0) {}
    AircraftWindSpeed(int wind_direction, double wind_speed) : wind_direction(wind_direction), wind_speed(wind_speed) {}
};

// 定义新的AircraftModeCode类
class AircraftModeCode {
public:
    int mode_code;
    
    AircraftModeCode() : mode_code(0) {}
    AircraftModeCode(int mode_code) : mode_code(mode_code) {}
};

// 定义新的DroneInDock类
class DroneInDock {
public:
    int drone_in_dock;
    
    DroneInDock() : drone_in_dock(0) {}
    DroneInDock(int drone_in_dock) : drone_in_dock(drone_in_dock) {}
};

// 定义通知频率枚举
enum NotificationFrequency {
    ANY = 0,
    ON_CHANGED = 1,
    PUSH_1S = 1000,
    PUSH_2S = 2000,
    PUSH_3S = 3000,
    PUSH_4S = 4000,
    PUSH_5S = 5000,
    PUSH_10S = 10000,
    PUSH_20S = 20000,
    PUSH_30S = 30000
};

// 定义具体的回调接口
class AircraftLocationCallback {
public:
    virtual ~AircraftLocationCallback() {}
    virtual void invoke(const AircraftLocation& message) = 0;
};

class AircraftAttitudeCallback {
public:
    virtual ~AircraftAttitudeCallback() {}
    virtual void invoke(const AircraftAttitude& message) = 0;
};

class AircraftSpeedCallback {
public:
    virtual ~AircraftSpeedCallback() {}
    virtual void invoke(const AircraftSpeed& message) = 0;
};

class DeviceOsdCallback {
public:
    virtual ~DeviceOsdCallback() {}
    virtual void invoke(const DeviceOsd& message) = 0;
};

class AircraftBatteryInfoCallback {
public:
    virtual ~AircraftBatteryInfoCallback() {}
    virtual void invoke(const AircraftBatteryInfo& message) = 0;
};

class AircraftControlCodeCallback {
public:
    virtual ~AircraftControlCodeCallback() {}
    virtual void invoke(const AircraftControlCode& message) = 0;
};

class AircraftWindSpeedCallback {
public:
    virtual ~AircraftWindSpeedCallback() {}
    virtual void invoke(const AircraftWindSpeed& message) = 0;
};

class AircraftModeCodeCallback {
public:
    virtual ~AircraftModeCodeCallback() {}
    virtual void invoke(const AircraftModeCode& message) = 0;
};

class DroneInDockCallback {
public:
    virtual ~DroneInDockCallback() {}
    virtual void invoke(const DroneInDock& message) = 0;
};

class SDKSubscribeResultCallback {
public:
    virtual ~SDKSubscribeResultCallback() {}
    virtual void invoke(const NotificationCenterErrorCode& result) = 0;
};
%}

// Create Java-friendly callback interfaces and data structures
%feature("director") AircraftLocationCallback;
%feature("director") AircraftAttitudeCallback;
%feature("director") AircraftSpeedCallback;
%feature("director") DeviceOsdCallback;
%feature("director") AircraftBatteryInfoCallback;
%feature("director") AircraftControlCodeCallback;
%feature("director") AircraftWindSpeedCallback;
%feature("director") AircraftModeCodeCallback;
%feature("director") DroneInDockCallback;
%feature("director") SDKSubscribeResultCallback;

// Include the type definitions and enums
%include "message_define/common.h"
%include "business_manager/BusinessManagerDefine.h"

// Process the actual header files
%include "SDKManager.h"
%include "BusinessManager.h"
%include "WebsocketEvent.h"

// Add Java-friendly wrapper methods to BusinessManager
%extend BusinessManager {
    long ListenAircraftLocation(
        AircraftLocationCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftLocationMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftLocationMsg转换为AircraftLocation
                AircraftLocation aircraft_location(
                    msg.height,
                    msg.elevation,
                    msg.latitude,
                    msg.longitude
                );
                onSubscribeMessageCallback->invoke(aircraft_location);
            }
        };
        
        auto result_cb = [onSubscribeResultCallback](const NotificationCenterErrorCode& error_code) {
            if (onSubscribeResultCallback) {
                onSubscribeResultCallback->invoke(error_code);
            }
        };
        
        // Convert NotificationFrequency to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (notificationFrequency) {
            case NotificationFrequency::ANY: notify_freq = NotifactionFrequency_Any; break;
            case NotificationFrequency::ON_CHANGED: notify_freq = NotifactionFrequency_OnChanged; break;
            case NotificationFrequency::PUSH_1S: notify_freq = NotifactionFrequency_Push_1s; break;
            case NotificationFrequency::PUSH_2S: notify_freq = NotifactionFrequency_Push_2s; break;
            case NotificationFrequency::PUSH_3S: notify_freq = NotifactionFrequency_Push_3s; break;
            case NotificationFrequency::PUSH_4S: notify_freq = NotifactionFrequency_Push_4s; break;
            case NotificationFrequency::PUSH_5S: notify_freq = NotifactionFrequency_Push_5s; break;
            case NotificationFrequency::PUSH_10S: notify_freq = NotifactionFrequency_Push_10s; break;
            case NotificationFrequency::PUSH_20S: notify_freq = NotifactionFrequency_Push_20s; break;
            case NotificationFrequency::PUSH_30S: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return $self->ListenAircraftLocation(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenAircraftAttitude(
        AircraftAttitudeCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftAttitudeMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftAttitudeMsg转换为AircraftAttitude
                AircraftAttitude aircraft_attitude(
                    msg.attitude_head,
                    msg.attitude_pitch,
                    msg.attitude_roll
                );
                onSubscribeMessageCallback->invoke(aircraft_attitude);
            }
        };
        
        auto result_cb = [onSubscribeResultCallback](const NotificationCenterErrorCode& error_code) {
            if (onSubscribeResultCallback) {
                onSubscribeResultCallback->invoke(error_code);
            }
        };
        
        // Convert NotificationFrequency to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (notificationFrequency) {
            case NotificationFrequency::ANY: notify_freq = NotifactionFrequency_Any; break;
            case NotificationFrequency::ON_CHANGED: notify_freq = NotifactionFrequency_OnChanged; break;
            case NotificationFrequency::PUSH_1S: notify_freq = NotifactionFrequency_Push_1s; break;
            case NotificationFrequency::PUSH_2S: notify_freq = NotifactionFrequency_Push_2s; break;
            case NotificationFrequency::PUSH_3S: notify_freq = NotifactionFrequency_Push_3s; break;
            case NotificationFrequency::PUSH_4S: notify_freq = NotifactionFrequency_Push_4s; break;
            case NotificationFrequency::PUSH_5S: notify_freq = NotifactionFrequency_Push_5s; break;
            case NotificationFrequency::PUSH_10S: notify_freq = NotifactionFrequency_Push_10s; break;
            case NotificationFrequency::PUSH_20S: notify_freq = NotifactionFrequency_Push_20s; break;
            case NotificationFrequency::PUSH_30S: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return $self->ListenAircraftAttitude(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenAircraftSpeed(
        AircraftSpeedCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftSpeedMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftSpeedMsg转换为AircraftSpeed
                AircraftSpeed aircraft_speed(
                    msg.horizontal_speed,
                    msg.vertical_speed
                );
                onSubscribeMessageCallback->invoke(aircraft_speed);
            }
        };
        
        auto result_cb = [onSubscribeResultCallback](const NotificationCenterErrorCode& error_code) {
            if (onSubscribeResultCallback) {
                onSubscribeResultCallback->invoke(error_code);
            }
        };
        
        // Convert NotificationFrequency to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (notificationFrequency) {
            case NotificationFrequency::ANY: notify_freq = NotifactionFrequency_Any; break;
            case NotificationFrequency::ON_CHANGED: notify_freq = NotifactionFrequency_OnChanged; break;
            case NotificationFrequency::PUSH_1S: notify_freq = NotifactionFrequency_Push_1s; break;
            case NotificationFrequency::PUSH_2S: notify_freq = NotifactionFrequency_Push_2s; break;
            case NotificationFrequency::PUSH_3S: notify_freq = NotifactionFrequency_Push_3s; break;
            case NotificationFrequency::PUSH_4S: notify_freq = NotifactionFrequency_Push_4s; break;
            case NotificationFrequency::PUSH_5S: notify_freq = NotifactionFrequency_Push_5s; break;
            case NotificationFrequency::PUSH_10S: notify_freq = NotifactionFrequency_Push_10s; break;
            case NotificationFrequency::PUSH_20S: notify_freq = NotifactionFrequency_Push_20s; break;
            case NotificationFrequency::PUSH_30S: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return $self->ListenAircraftSpeed(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenDeviceOsd(
        DeviceOsdCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const DeviceOsdMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将DeviceOsdMsg转换为DeviceOsd
                DeviceOsdHost host(msg.host.mode_code, msg.host.attitude_head, 
                                 msg.host.attitude_pitch, msg.host.attitude_roll);
                DeviceOsd device_osd(host, msg.sn);
                onSubscribeMessageCallback->invoke(device_osd);
            }
        };
        
        auto result_cb = [onSubscribeResultCallback](const NotificationCenterErrorCode& error_code) {
            if (onSubscribeResultCallback) {
                onSubscribeResultCallback->invoke(error_code);
            }
        };
        
        // Convert NotificationFrequency to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (notificationFrequency) {
            case NotificationFrequency::ANY: notify_freq = NotifactionFrequency_Any; break;
            case NotificationFrequency::ON_CHANGED: notify_freq = NotifactionFrequency_OnChanged; break;
            case NotificationFrequency::PUSH_1S: notify_freq = NotifactionFrequency_Push_1s; break;
            case NotificationFrequency::PUSH_2S: notify_freq = NotifactionFrequency_Push_2s; break;
            case NotificationFrequency::PUSH_3S: notify_freq = NotifactionFrequency_Push_3s; break;
            case NotificationFrequency::PUSH_4S: notify_freq = NotifactionFrequency_Push_4s; break;
            case NotificationFrequency::PUSH_5S: notify_freq = NotifactionFrequency_Push_5s; break;
            case NotificationFrequency::PUSH_10S: notify_freq = NotifactionFrequency_Push_10s; break;
            case NotificationFrequency::PUSH_20S: notify_freq = NotifactionFrequency_Push_20s; break;
            case NotificationFrequency::PUSH_30S: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return $self->ListenDeviceOsd(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenAircraftBatteryInfo(
        AircraftBatteryInfoCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftBatteryInfoMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftBatteryInfoMsg转换为AircraftBatteryInfo
                AircraftBatteryInfo aircraft_battery_info;
                aircraft_battery_info.capacity_percent = msg.capacity_percent;
                aircraft_battery_info.landing_power = msg.landing_power;
                aircraft_battery_info.remain_flight_time = msg.remain_flight_time;
                aircraft_battery_info.return_home_power = msg.return_home_power;
                
                // 转换电池信息
                for (const auto& battery : msg.batteries) {
                    BatteryInfo battery_info;
                    battery_info.capacity_percent = battery.capacity_percent;
                    battery_info.firmware_version = battery.firmware_version;
                    battery_info.high_voltage_storage_days = battery.high_voltage_storage_days;
                    battery_info.index = battery.index;
                    battery_info.loop_times = battery.loop_times;
                    battery_info.sn = battery.sn;
                    battery_info.sub_type = battery.sub_type;
                    battery_info.temperature = battery.temperature;
                    battery_info.type = battery.type;
                    battery_info.voltage = battery.voltage;
                    aircraft_battery_info.batteries.push_back(battery_info);
                }
                
                onSubscribeMessageCallback->invoke(aircraft_battery_info);
            }
        };
        
        auto result_cb = [onSubscribeResultCallback](const NotificationCenterErrorCode& error_code) {
            if (onSubscribeResultCallback) {
                onSubscribeResultCallback->invoke(error_code);
            }
        };
        
        // Convert NotificationFrequency to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (notificationFrequency) {
            case NotificationFrequency::ANY: notify_freq = NotifactionFrequency_Any; break;
            case NotificationFrequency::ON_CHANGED: notify_freq = NotifactionFrequency_OnChanged; break;
            case NotificationFrequency::PUSH_1S: notify_freq = NotifactionFrequency_Push_1s; break;
            case NotificationFrequency::PUSH_2S: notify_freq = NotifactionFrequency_Push_2s; break;
            case NotificationFrequency::PUSH_3S: notify_freq = NotifactionFrequency_Push_3s; break;
            case NotificationFrequency::PUSH_4S: notify_freq = NotifactionFrequency_Push_4s; break;
            case NotificationFrequency::PUSH_5S: notify_freq = NotifactionFrequency_Push_5s; break;
            case NotificationFrequency::PUSH_10S: notify_freq = NotifactionFrequency_Push_10s; break;
            case NotificationFrequency::PUSH_20S: notify_freq = NotifactionFrequency_Push_20s; break;
            case NotificationFrequency::PUSH_30S: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return $self->ListenAircraftBatteryInfo(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenAircraftControlCode(
        AircraftControlCodeCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftControlCodeMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftControlCodeMsg转换为AircraftControlCode
                AircraftControlCode aircraft_control_code(msg.control_mode);
                onSubscribeMessageCallback->invoke(aircraft_control_code);
            }
        };
        
        auto result_cb = [onSubscribeResultCallback](const NotificationCenterErrorCode& error_code) {
            if (onSubscribeResultCallback) {
                onSubscribeResultCallback->invoke(error_code);
            }
        };
        
        // Convert NotificationFrequency to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (notificationFrequency) {
            case NotificationFrequency::ANY: notify_freq = NotifactionFrequency_Any; break;
            case NotificationFrequency::ON_CHANGED: notify_freq = NotifactionFrequency_OnChanged; break;
            case NotificationFrequency::PUSH_1S: notify_freq = NotifactionFrequency_Push_1s; break;
            case NotificationFrequency::PUSH_2S: notify_freq = NotifactionFrequency_Push_2s; break;
            case NotificationFrequency::PUSH_3S: notify_freq = NotifactionFrequency_Push_3s; break;
            case NotificationFrequency::PUSH_4S: notify_freq = NotifactionFrequency_Push_4s; break;
            case NotificationFrequency::PUSH_5S: notify_freq = NotifactionFrequency_Push_5s; break;
            case NotificationFrequency::PUSH_10S: notify_freq = NotifactionFrequency_Push_10s; break;
            case NotificationFrequency::PUSH_20S: notify_freq = NotifactionFrequency_Push_20s; break;
            case NotificationFrequency::PUSH_30S: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return $self->ListenAircraftControlCode(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenAircraftWindSpeed(
        AircraftWindSpeedCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftWindSpeedMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftWindSpeedMsg转换为AircraftWindSpeed
                AircraftWindSpeed aircraft_wind_speed(msg.wind_direction, msg.wind_speed);
                onSubscribeMessageCallback->invoke(aircraft_wind_speed);
            }
        };
        
        auto result_cb = [onSubscribeResultCallback](const NotificationCenterErrorCode& error_code) {
            if (onSubscribeResultCallback) {
                onSubscribeResultCallback->invoke(error_code);
            }
        };
        
        // Convert NotificationFrequency to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (notificationFrequency) {
            case NotificationFrequency::ANY: notify_freq = NotifactionFrequency_Any; break;
            case NotificationFrequency::ON_CHANGED: notify_freq = NotifactionFrequency_OnChanged; break;
            case NotificationFrequency::PUSH_1S: notify_freq = NotifactionFrequency_Push_1s; break;
            case NotificationFrequency::PUSH_2S: notify_freq = NotifactionFrequency_Push_2s; break;
            case NotificationFrequency::PUSH_3S: notify_freq = NotifactionFrequency_Push_3s; break;
            case NotificationFrequency::PUSH_4S: notify_freq = NotifactionFrequency_Push_4s; break;
            case NotificationFrequency::PUSH_5S: notify_freq = NotifactionFrequency_Push_5s; break;
            case NotificationFrequency::PUSH_10S: notify_freq = NotifactionFrequency_Push_10s; break;
            case NotificationFrequency::PUSH_20S: notify_freq = NotifactionFrequency_Push_20s; break;
            case NotificationFrequency::PUSH_30S: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return $self->ListenAircraftWindSpeed(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenAircraftModeCode(
        AircraftModeCodeCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftModeCodeMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftModeCodeMsg转换为AircraftModeCode
                AircraftModeCode aircraft_mode_code(msg.mode_code);
                onSubscribeMessageCallback->invoke(aircraft_mode_code);
            }
        };
        
        auto result_cb = [onSubscribeResultCallback](const NotificationCenterErrorCode& error_code) {
            if (onSubscribeResultCallback) {
                onSubscribeResultCallback->invoke(error_code);
            }
        };
        
        // Convert NotificationFrequency to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (notificationFrequency) {
            case NotificationFrequency::ANY: notify_freq = NotifactionFrequency_Any; break;
            case NotificationFrequency::ON_CHANGED: notify_freq = NotifactionFrequency_OnChanged; break;
            case NotificationFrequency::PUSH_1S: notify_freq = NotifactionFrequency_Push_1s; break;
            case NotificationFrequency::PUSH_2S: notify_freq = NotifactionFrequency_Push_2s; break;
            case NotificationFrequency::PUSH_3S: notify_freq = NotifactionFrequency_Push_3s; break;
            case NotificationFrequency::PUSH_4S: notify_freq = NotifactionFrequency_Push_4s; break;
            case NotificationFrequency::PUSH_5S: notify_freq = NotifactionFrequency_Push_5s; break;
            case NotificationFrequency::PUSH_10S: notify_freq = NotifactionFrequency_Push_10s; break;
            case NotificationFrequency::PUSH_20S: notify_freq = NotifactionFrequency_Push_20s; break;
            case NotificationFrequency::PUSH_30S: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return $self->ListenAircraftModeCode(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenDroneInDock(
        DroneInDockCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const DroneInDockMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将DroneInDockMsg转换为DroneInDock
                DroneInDock drone_in_dock(msg.drone_in_dock);
                onSubscribeMessageCallback->invoke(drone_in_dock);
            }
        };
        
        auto result_cb = [onSubscribeResultCallback](const NotificationCenterErrorCode& error_code) {
            if (onSubscribeResultCallback) {
                onSubscribeResultCallback->invoke(error_code);
            }
        };
        
        // Convert NotificationFrequency to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (notificationFrequency) {
            case NotificationFrequency::ANY: notify_freq = NotifactionFrequency_Any; break;
            case NotificationFrequency::ON_CHANGED: notify_freq = NotifactionFrequency_OnChanged; break;
            case NotificationFrequency::PUSH_1S: notify_freq = NotifactionFrequency_Push_1s; break;
            case NotificationFrequency::PUSH_2S: notify_freq = NotifactionFrequency_Push_2s; break;
            case NotificationFrequency::PUSH_3S: notify_freq = NotifactionFrequency_Push_3s; break;
            case NotificationFrequency::PUSH_4S: notify_freq = NotifactionFrequency_Push_4s; break;
            case NotificationFrequency::PUSH_5S: notify_freq = NotifactionFrequency_Push_5s; break;
            case NotificationFrequency::PUSH_10S: notify_freq = NotifactionFrequency_Push_10s; break;
            case NotificationFrequency::PUSH_20S: notify_freq = NotifactionFrequency_Push_20s; break;
            case NotificationFrequency::PUSH_30S: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return $self->ListenDroneInDock(msg_cb, result_cb, sn, notify_freq);
    }
    
    void cancelObserve(long listen_id) {
        $self->CancelObserve(listen_id);
    }
}

// Add Java-friendly constructor for SdkInitializeInfo
%extend SdkInitializeInfo {
    SdkInitializeInfo(const std::string& log_path = "", SdkLogLevel log_level = SdkLogLevel::NONE) {
        SdkInitializeInfo* info = new SdkInitializeInfo();
        info->log_path = log_path;
        info->log_level = log_level;
        return info;
    }
} 