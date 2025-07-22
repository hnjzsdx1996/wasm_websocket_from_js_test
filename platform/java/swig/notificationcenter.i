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
#include "business_manager/topic_message_define/PublishAircraftLocationTopic.h"
#include "business_manager/topic_message_define/PublishAircraftAttitudeTopic.h"
#include "business_manager/topic_message_define/PublishAircraftSpeedTopic.h"
#include "business_manager/topic_message_define/PublishDeviceOsdTopic.h"
#include "message_define/common.h"
%}

// Standard library support
%include "std_string.i"
%include "std_shared_ptr.i"

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
%ignore SDKManager::getTopicManager;
%ignore SDKManager::getWebSocketHolder;

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

// 定义新的DeviceOsd类
class DeviceOsd {
public:
    int mode_code; // 模式代码
    
    DeviceOsd() : mode_code(0) {}
    DeviceOsd(int mode_code) : mode_code(mode_code) {}
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
                DeviceOsd device_osd(msg.mode_code);
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