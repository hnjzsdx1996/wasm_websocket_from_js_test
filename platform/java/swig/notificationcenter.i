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
#include "business_manager/topic_message_define/PublishAircraftBatteryInfoTopic.h"
#include "business_manager/topic_message_define/PublishAircraftControlCodeTopic.h"
#include "business_manager/topic_message_define/PublishAircraftWindSpeedTopic.h"
#include "business_manager/topic_message_define/PublishAircraftModeCodeTopic.h"
#include "business_manager/topic_message_define/PublishDroneInDockTopic.h"
#include "business_manager/topic_message_define/PublishAircraftPayloadsCameraLiveviewWorldRegionTopic.h"
#include "business_manager/topic_message_define/PublishAircraftPayloadsGimbalAttitudeTopic.h"
#include "business_manager/topic_message_define/PublishFlightTasksTopic.h"
#include "message_define/common.h"
%}

// Standard library support
%include "std_string.i"
%include "std_shared_ptr.i"
%include "std_vector.i"
%include "std_unordered_map.i"

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

// 定义新的DeviceOnlineStatus类
class DeviceOnlineStatus {
public:
    bool device_status;
    std::string device_callsign;
    std::string device_model;
    std::string device_type;
    
    DeviceOnlineStatus() : device_status(false) {}
    DeviceOnlineStatus(bool device_status, const std::string& device_callsign, 
                      const std::string& device_model, const std::string& device_type) 
        : device_status(device_status), device_callsign(device_callsign), 
          device_model(device_model), device_type(device_type) {}
};

// 定义新的CameraLiveviewWorldRegion类
class CameraLiveviewWorldRegion {
public:
    double bottom;
    double left;
    double right;
    double top;
    
    CameraLiveviewWorldRegion() : bottom(0.0), left(0.0), right(0.0), top(0.0) {}
    CameraLiveviewWorldRegion(double bottom, double left, double right, double top) 
        : bottom(bottom), left(left), right(right), top(top) {}
};

// 定义新的AircraftPayloadsCameraLiveviewWorldRegion类
class AircraftPayloadsCameraLiveviewWorldRegion {
public:
    std::unordered_map<std::string, CameraLiveviewWorldRegion> payloads_list;
    
    AircraftPayloadsCameraLiveviewWorldRegion() {}
};



// 定义新的AircraftPayloadGimbalAttitude类
class AircraftPayloadGimbalAttitude {
public:
    double gimbal_pitch;
    double gimbal_roll;
    double gimbal_yaw;
    
    AircraftPayloadGimbalAttitude() : gimbal_pitch(0.0), gimbal_roll(0.0), gimbal_yaw(0.0) {}
    AircraftPayloadGimbalAttitude(double gimbal_pitch, double gimbal_roll, double gimbal_yaw) 
        : gimbal_pitch(gimbal_pitch), gimbal_roll(gimbal_roll), gimbal_yaw(gimbal_yaw) {}
};

// 定义新的AircraftPayloadsGimbalAttitude类
class AircraftPayloadsGimbalAttitude {
public:
    std::unordered_map<std::string, AircraftPayloadGimbalAttitude> payloads_gimbal_attitude;
    
    AircraftPayloadsGimbalAttitude() {}
};

// 定义新的FolderInfoData类
class FolderInfoData {
public:
    int folder_id; // 媒体文件夹id
    int expected_file_count; // 媒体文件总数
    int uploaded_file_count; // 媒体文件上传总数
    
    FolderInfoData() : folder_id(0), expected_file_count(0), uploaded_file_count(0) {}
    FolderInfoData(int folder_id, int expected_file_count, int uploaded_file_count) 
        : folder_id(folder_id), expected_file_count(expected_file_count), uploaded_file_count(uploaded_file_count) {}
};

// 定义新的FlightTask类
class FlightTask {
public:
    std::string uuid;
    std::string name;
    int task_type;
    int status;
    int progress;
    std::string sn;
    FolderInfoData folder_info;
    long run_at; // 任务实际运行时间
    long complete_at; // 任务结束时间
    int total_waypoints; // 总航线数
    int current_waypoint_index; // 已飞完的航点数
    int progress_version; // 消息的时间戳，用来处理乱序问题
    int resumable_status; // ResumableStatus
    int obstacle_avoidance_notify; // ObstacleNotifyType
    std::string wayline_uuid;
    
    FlightTask() : task_type(0), status(0), progress(0), run_at(0), complete_at(0), 
                   total_waypoints(0), current_waypoint_index(0), progress_version(0), 
                   resumable_status(0), obstacle_avoidance_notify(0) {}
};

// 定义新的FlightTasks类
class FlightTasks {
public:
    std::vector<FlightTask> flight_tasks;
    
    FlightTasks() {}
};

// 定义新的AircraftPayloadsList类
class AircraftPayloadsList {
public:
    std::vector<std::string> payloads_list;
    
    AircraftPayloadsList() {}
};

// 定义新的DockLocation类
class DockLocation {
public:
    double heading;
    double height;
    double latitude;
    double longitude;
    
    DockLocation() : heading(0.0), height(0.0), latitude(0.0), longitude(0.0) {}
    DockLocation(double heading, double height, double latitude, double longitude) 
        : heading(heading), height(height), latitude(latitude), longitude(longitude) {}
};

// 定义具体的回调接口
class DeviceOnlineStatusCallback {
public:
    virtual ~DeviceOnlineStatusCallback() {}
    virtual void invoke(const DeviceOnlineStatus& message) = 0;
};



class AircraftPayloadsListCallback {
public:
    virtual ~AircraftPayloadsListCallback() {}
    virtual void invoke(const AircraftPayloadsList& message) = 0;
};

class DockLocationCallback {
public:
    virtual ~DockLocationCallback() {}
    virtual void invoke(const DockLocation& message) = 0;
};

class AircraftPayloadsCameraLiveviewWorldRegionCallback {
public:
    virtual ~AircraftPayloadsCameraLiveviewWorldRegionCallback() {}
    virtual void invoke(const AircraftPayloadsCameraLiveviewWorldRegion& message) = 0;
};

class AircraftPayloadsGimbalAttitudeCallback {
public:
    virtual ~AircraftPayloadsGimbalAttitudeCallback() {}
    virtual void invoke(const AircraftPayloadsGimbalAttitude& message) = 0;
};

class FlightTasksCallback {
public:
    virtual ~FlightTasksCallback() {}
    virtual void invoke(const FlightTasks& message) = 0;
};
%}

// Create Java-friendly callback interfaces and data structures
%feature("director") AircraftLocationCallback;
%feature("director") AircraftAttitudeCallback;
%feature("director") AircraftSpeedCallback;
%feature("director") AircraftBatteryInfoCallback;
%feature("director") AircraftControlCodeCallback;
%feature("director") AircraftWindSpeedCallback;
%feature("director") AircraftModeCodeCallback;
%feature("director") DroneInDockCallback;
%feature("director") SDKSubscribeResultCallback;
%feature("director") DeviceOnlineStatusCallback;

%feature("director") AircraftPayloadsListCallback;
%feature("director") DockLocationCallback;
%feature("director") AircraftPayloadsCameraLiveviewWorldRegionCallback;
%feature("director") AircraftPayloadsGimbalAttitudeCallback;
%feature("director") FlightTasksCallback;

%template(StringVector) std::vector<std::string>;
%template(FlightTaskVector) std::vector<FlightTask>;
%template(CameraLiveviewWorldRegionMap) std::unordered_map<std::string, CameraLiveviewWorldRegion>;
%template(AircraftPayloadGimbalAttitudeMap) std::unordered_map<std::string, AircraftPayloadGimbalAttitude>;

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
    
    long ListenDeviceOnlineStatus(
        DeviceOnlineStatusCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const DeviceOnlineStatusMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将DeviceOnlineStatusMsg转换为DeviceOnlineStatus
                DeviceOnlineStatus device_online_status(
                    msg.device_status,
                    msg.device_callsign,
                    msg.device_model,
                    msg.device_type
                );
                onSubscribeMessageCallback->invoke(device_online_status);
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
        
        return $self->ListenDeviceOnlineStatus(msg_cb, result_cb, sn, notify_freq);
    }
    

    

    
    long ListenAircraftPayloadsList(
        AircraftPayloadsListCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftPayloadsListMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftPayloadsListMsg转换为AircraftPayloadsList
                AircraftPayloadsList aircraft_payloads_list;
                aircraft_payloads_list.payloads_list = msg.payloads_list;
                onSubscribeMessageCallback->invoke(aircraft_payloads_list);
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
        
        return $self->ListenAircraftPayloadsList(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenDockLocation(
        DockLocationCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const DockLocationMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将DockLocationMsg转换为DockLocation
                DockLocation dock_location(
                    msg.heading,
                    msg.height,
                    msg.latitude,
                    msg.longitude
                );
                onSubscribeMessageCallback->invoke(dock_location);
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
        
        return $self->ListenDockLocation(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenAircraftPayloadsCameraLiveviewWorldRegion(
        AircraftPayloadsCameraLiveviewWorldRegionCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftPayloadsCameraLiveviewWorldRegionMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftPayloadsCameraLiveviewWorldRegionMsg转换为AircraftPayloadsCameraLiveviewWorldRegion
                AircraftPayloadsCameraLiveviewWorldRegion aircraft_payloads_camera_liveview_world_region;
                for (const auto& payload : msg.payloads_list) {
                    CameraLiveviewWorldRegion camera_liveview_world_region(
                        payload.second.bottom,
                        payload.second.left,
                        payload.second.right,
                        payload.second.top
                    );
                    aircraft_payloads_camera_liveview_world_region.payloads_list[payload.first] = camera_liveview_world_region;
                }
                onSubscribeMessageCallback->invoke(aircraft_payloads_camera_liveview_world_region);
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
        
        return $self->ListenAircraftPayloadsCameraLiveviewWorldRegion(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenAircraftPayloadsGimbalAttitude(
        AircraftPayloadsGimbalAttitudeCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const AircraftPayloadsGimbalAttitudeMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将AircraftPayloadsGimbalAttitudeMsg转换为AircraftPayloadsGimbalAttitude
                AircraftPayloadsGimbalAttitude aircraft_payloads_gimbal_attitude;
                for (const auto& payload : msg.payloads_gimbal_attitude) {
                    AircraftPayloadGimbalAttitude aircraft_payload_gimbal_attitude(
                        payload.second.gimbal_pitch,
                        payload.second.gimbal_roll,
                        payload.second.gimbal_yaw
                    );
                    aircraft_payloads_gimbal_attitude.payloads_gimbal_attitude[payload.first] = aircraft_payload_gimbal_attitude;
                }
                onSubscribeMessageCallback->invoke(aircraft_payloads_gimbal_attitude);
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
        
        return $self->ListenAircraftPayloadsGimbalAttitude(msg_cb, result_cb, sn, notify_freq);
    }
    
    long ListenFlightTasks(
        FlightTasksCallback* onSubscribeMessageCallback,
        SDKSubscribeResultCallback* onSubscribeResultCallback,
        const std::string& sn,
        NotificationFrequency notificationFrequency) {
        
        auto msg_cb = [onSubscribeMessageCallback](const FlightTasksMsg& msg) {
            if (onSubscribeMessageCallback) {
                // 将FlightTasksMsg转换为FlightTasks
                FlightTasks flight_tasks;
                for (const auto& task : msg.flight_tasks) {
                    FlightTask flight_task;
                    flight_task.uuid = task.uuid;
                    flight_task.name = task.name;
                    flight_task.task_type = task.task_type;
                    flight_task.status = task.status;
                    flight_task.progress = task.progress;
                    flight_task.sn = task.sn;
                    flight_task.folder_info = FolderInfoData(
                        task.folder_info.folder_id,
                        task.folder_info.expected_file_count,
                        task.folder_info.uploaded_file_count
                    );
                    flight_task.run_at = task.run_at;
                    flight_task.complete_at = task.complete_at;
                    flight_task.total_waypoints = task.total_waypoints;
                    flight_task.current_waypoint_index = task.current_waypoint_index;
                    flight_task.progress_version = task.progress_version;
                    flight_task.resumable_status = task.resumable_status;
                    flight_task.obstacle_avoidance_notify = task.obstacle_avoidance_notify;
                    flight_task.wayline_uuid = task.wayline_uuid;
                    flight_tasks.flight_tasks.push_back(flight_task);
                }
                onSubscribeMessageCallback->invoke(flight_tasks);
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
        
        return $self->ListenFlightTasks(msg_cb, result_cb, sn, notify_freq);
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