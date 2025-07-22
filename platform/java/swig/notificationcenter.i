/* notificationcenter.i */

%module(directors="1") notificationcenter
%javaconst(1);

// Enable C++11 features and directors for cross-language polymorphism
%feature("c++11");
%feature("director");

// Ignore problematic macros and specific lines
%ignore AIGC_JSON_HELPER;
%ignore AIGC_JSON_HELPER_BASE;
%ignore PublishAircraftLocationTopic::AIGC_JSON_HELPER;
%ignore PublishAircraftLocationTopic::AIGC_JSON_HELPER_BASE;

%{
#include "SDKManager.h"
#include "BusinessManager.h"
#include "WebsocketEvent.h"
#include "business_manager/BusinessManagerDefine.h"
#include "business_manager/topic_message_define/PublishAircraftLocationTopic.h"
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

// 定义回调接口
template<typename T>
class JavaMessageCallback {
public:
    virtual ~JavaMessageCallback() {}
    virtual void invoke(const T& message) = 0;
};

class JavaResultCallback {
public:
    virtual ~JavaResultCallback() {}
    virtual void invoke(const NotificationCenterErrorCode& result) = 0;
};
%}

// Create Java-friendly callback interfaces and data structures
%feature("director") JavaMessageCallback;
%feature("director") JavaResultCallback;

// 为特定的模板实例化创建具体的Java类
%template(JavaMessageCallbackAircraftLocation) JavaMessageCallback<AircraftLocation>;

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
        JavaMessageCallback<AircraftLocation>* onSubscribeMessageCallback,
        JavaResultCallback* onSubscribeResultCallback,
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