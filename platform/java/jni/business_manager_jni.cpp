//
// Created by desmond on 2024/7/26.
//

#include <jni.h>
#include "business_manager/BusinessManager.h"
#include "base/logger/logger.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jlong JNICALL
Java_com_example_notificationcentersdk_BusinessManager_nativeListenAircraftLocation(JNIEnv *env, jobject thiz, jlong native_ptr, jstring device_sn, jint freq) {
    if (native_ptr) {
        auto *businessManager = reinterpret_cast<BusinessManager *>(native_ptr);
        const char *c_device_sn = env->GetStringUTFChars(device_sn, nullptr);

        auto listen_id = businessManager->ListenAircraftLocation(
                [=](const std::string& msg) {
                    // todo: 怎么把这个回调给到 java 层
                    NC_LOG_INFO("aircraft location callback, msg: %s", msg.c_str());
                },
                [=](const NotificationCenterErrorCode& error_code) {
                    // todo: 怎么把这个回调给到 java 层
                    NC_LOG_INFO("aircraft location result callback, err: %d", error_code);
                },
                c_device_sn,
                (NotifactionFrequency)freq
        );

        env->ReleaseStringUTFChars(device_sn, c_device_sn);
        return listen_id;
    }
    return 0;
}

JNIEXPORT void JNICALL
Java_com_example_notificationcentersdk_BusinessManager_nativeCancelObserve(JNIEnv *env, jobject thiz, jlong native_ptr, jlong listen_id) {
    if (native_ptr) {
        auto *businessManager = reinterpret_cast<BusinessManager *>(native_ptr);
        businessManager->CancelObserve(listen_id);
    }
}

#ifdef __cplusplus
}
#endif 