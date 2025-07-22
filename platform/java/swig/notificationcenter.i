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

// Create Java-friendly callback interfaces with different names
%feature("director") AircraftLocationCallback;
%feature("director") ResultCallback;

// 手动定义AircraftLocation类，避免宏问题
%inline %{
class AircraftLocation {
public:
    double height;    // 椭球高度
    double elevation; // 相对起飞点高度
    double longitude; // 经度
    double latitude;  // 纬度
    
    AircraftLocation() : height(0.0), elevation(0.0), longitude(0.0), latitude(0.0) {}
    AircraftLocation(double height, double elevation, double longitude, double latitude) 
        : height(height), elevation(elevation), longitude(longitude), latitude(latitude) {}
};

class AircraftLocationCallback {
public:
    virtual ~AircraftLocationCallback() {}
    virtual void onMessage(const AircraftLocation& msg) = 0;
};

class ResultCallback {
public:
    virtual ~ResultCallback() {}
    virtual void onResult(long result) = 0;
};
%}

// Include the type definitions and enums
%include "message_define/common.h"
%include "business_manager/BusinessManagerDefine.h"

// Process the actual header files
%include "SDKManager.h"
%include "BusinessManager.h"
%include "WebsocketEvent.h"

// Add Java-friendly wrapper methods to BusinessManager
%extend BusinessManager {
    long listenAircraftLocationJava(AircraftLocationCallback* msg_callback, ResultCallback* result_callback, const std::string& device_sn, int freq) {
        auto msg_cb = [msg_callback](const AircraftLocationMsg& msg) {
            if (msg_callback) {
                // 将AircraftLocationMsg转换为AircraftLocation
                AircraftLocation aircraft_location(msg.height, msg.elevation, msg.longitude, msg.latitude);
                msg_callback->onMessage(aircraft_location);
            }
        };
        
        auto result_cb = [result_callback](const NotificationCenterErrorCode& error_code) {
            if (result_callback) {
                result_callback->onResult(static_cast<long>(error_code));
            }
        };
        
        // Convert freq to NotifactionFrequency enum
        NotifactionFrequency notify_freq;
        switch (freq) {
            case 0: notify_freq = NotifactionFrequency_Any; break;
            case 1: notify_freq = NotifactionFrequency_OnChanged; break;
            case 1000: notify_freq = NotifactionFrequency_Push_1s; break;
            case 2000: notify_freq = NotifactionFrequency_Push_2s; break;
            case 3000: notify_freq = NotifactionFrequency_Push_3s; break;
            case 4000: notify_freq = NotifactionFrequency_Push_4s; break;
            case 5000: notify_freq = NotifactionFrequency_Push_5s; break;
            case 10000: notify_freq = NotifactionFrequency_Push_10s; break;
            case 20000: notify_freq = NotifactionFrequency_Push_20s; break;
            case 30000: notify_freq = NotifactionFrequency_Push_30s; break;
            default: notify_freq = NotifactionFrequency_Any; break;
        }
        
        return static_cast<long>($self->ListenAircraftLocation(msg_cb, result_cb, device_sn, notify_freq));
    }
    
    void cancelObserveJava(long listen_id) {
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