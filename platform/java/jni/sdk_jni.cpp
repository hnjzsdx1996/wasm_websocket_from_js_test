//
// Created by desmond on 2024/7/26.
//

#include <jni.h>
#include "SDKManager.h"
#include "websocket/WebsocketEvent.h" // Include the base class definition
#include "base/logger/logger.h" // For logging

JavaVM* g_vm = nullptr;

// A helper struct to manage the JNI environment and Java listener
struct JniListener {
    jobject g_listener = nullptr;

    void onOpen() {
        if (!g_vm) {
            NC_LOG_ERROR("JNI_OnLoad was not called, or failed.");
            return;
        }
        JNIEnv* env = nullptr;
        bool attached = false;
        if (g_vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
            if (g_vm->AttachCurrentThread((void**)&env, nullptr) == JNI_OK) {
                attached = true;
            } else {
                return; // Failed to attach
            }
        }

        jclass listenerClass = env->GetObjectClass(g_listener);
        jmethodID onOpenMethod = env->GetMethodID(listenerClass, "onOpen", "()V");
        env->CallVoidMethod(g_listener, onOpenMethod);
        env->DeleteLocalRef(listenerClass);

        if (attached) {
            g_vm->DetachCurrentThread();
        }
    }

    void onClose() {
        if (!g_vm) {
            NC_LOG_ERROR("JNI_OnLoad was not called, or failed.");
            return;
        }
        JNIEnv* env = nullptr;
        bool attached = false;
        if (g_vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
            if (g_vm->AttachCurrentThread((void**)&env, nullptr) == JNI_OK) {
                attached = true;
            } else {
                return;
            }
        }
        
        jclass listenerClass = env->GetObjectClass(g_listener);
        jmethodID onCloseMethod = env->GetMethodID(listenerClass, "onClose", "()V");
        env->CallVoidMethod(g_listener, onCloseMethod);
        env->DeleteLocalRef(listenerClass);

        if (attached) {
            g_vm->DetachCurrentThread();
        }
    }

    void onError(const std::string& error) {
        if (!g_vm) {
            NC_LOG_ERROR("JNI_OnLoad was not called, or failed.");
            return;
        }
        JNIEnv* env = nullptr;
        bool attached = false;
        if (g_vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
            if (g_vm->AttachCurrentThread((void**)&env, nullptr) == JNI_OK) {
                attached = true;
            } else {
                return;
            }
        }

        jclass listenerClass = env->GetObjectClass(g_listener);
        jmethodID onErrorMethod = env->GetMethodID(listenerClass, "onError", "(Ljava/lang/String;)V");
        jstring j_error = env->NewStringUTF(error.c_str());
        env->CallVoidMethod(g_listener, onErrorMethod, j_error);
        env->DeleteLocalRef(listenerClass);
        env->DeleteLocalRef(j_error);

        if (attached) {
            g_vm->DetachCurrentThread();
        }
    }

    void onMessage(const std::string& message) {
        if (!g_vm) {
            NC_LOG_ERROR("JNI_OnLoad was not called, or failed.");
            return;
        }
        JNIEnv* env = nullptr;
        bool attached = false;
        if (g_vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
            if (g_vm->AttachCurrentThread((void**)&env, nullptr) == JNI_OK) {
                attached = true;
            } else {
                return;
            }
        }

        jclass listenerClass = env->GetObjectClass(g_listener);
        jmethodID onMessageMethod = env->GetMethodID(listenerClass, "onMessage", "(Ljava/lang/String;)V");
        jstring j_message = env->NewStringUTF(message.c_str());
        env->CallVoidMethod(g_listener, onMessageMethod, j_message);
        env->DeleteLocalRef(listenerClass);
        env->DeleteLocalRef(j_message);

        if (attached) {
            g_vm->DetachCurrentThread();
        }
    }

    ~JniListener() {
        if (g_listener) {
            JNIEnv* env = nullptr;
            if (g_vm->GetEnv((void**)&env, JNI_VERSION_1_6) == JNI_OK) {
                env->DeleteGlobalRef(g_listener);
            }
        }
    }
};

// This class will implement the C++ WebsocketEvent interface and forward calls to the JniListener
class JniWebsocketEventAdapter : public WebsocketEvent {
public:
    explicit JniWebsocketEventAdapter(std::shared_ptr<JniListener> listener) : jni_listener_(std::move(listener)) {}

    void OnOpen() override { if (jni_listener_) jni_listener_->onOpen(); }
    void OnClose() override { if (jni_listener_) jni_listener_->onClose(); }
    void OnError(const std::string& error) override { if (jni_listener_) jni_listener_->onError(error); }
    void OnMessage(const std::string& message) override { if (jni_listener_) jni_listener_->onMessage(message); }

private:
    std::shared_ptr<JniListener> jni_listener_;
};

#ifdef __cplusplus
extern "C" {
#endif

// Make sure JNI_OnLoad is correctly defined and exported
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    if (vm == nullptr) {
        // Log this critical failure if you have a logger that works at this stage
        return JNI_ERR;
    }
    g_vm = vm;
    return JNI_VERSION_1_6;
}

JNIEXPORT jlong JNICALL
Java_com_example_notificationcentersdk_SDKManager_nativeCreate(JNIEnv *env, jobject thiz) {
    auto *sdkManager = new SDKManager();
    return reinterpret_cast<jlong>(sdkManager);
}

JNIEXPORT void JNICALL
Java_com_example_notificationcentersdk_SDKManager_nativeDestroy(JNIEnv *env, jobject thiz, jlong native_ptr) {
    if (native_ptr) {
        auto *sdkManager = reinterpret_cast<SDKManager *>(native_ptr);
        delete sdkManager;
    }
}

JNIEXPORT void JNICALL
Java_com_example_notificationcentersdk_SDKManager_nativeConfigure(JNIEnv *env, jobject thiz, jlong native_ptr, jstring config) {
    if (native_ptr) {
        auto *sdkManager = reinterpret_cast<SDKManager *>(native_ptr);
        const char *c_config = env->GetStringUTFChars(config, nullptr);
        sdkManager->configure(c_config);
        env->ReleaseStringUTFChars(config, c_config);
    }
}

JNIEXPORT void JNICALL
Java_com_example_notificationcentersdk_SDKManager_nativeSetConnectionListener(JNIEnv *env, jobject thiz, jlong native_ptr, jobject listener) {
    if (native_ptr && listener) {
        auto *sdkManager = reinterpret_cast<SDKManager *>(native_ptr);
        
        auto jniListener = std::make_shared<JniListener>();
        jniListener->g_listener = env->NewGlobalRef(listener);

        auto adapter = std::make_shared<JniWebsocketEventAdapter>(jniListener);
        sdkManager->setWebsocketEventListener(adapter);
    }
}

JNIEXPORT void JNICALL
Java_com_example_notificationcentersdk_SDKManager_nativeConnect(JNIEnv *env, jobject thiz, jlong native_ptr, jstring url) {
    if (native_ptr) {
        auto *sdkManager = reinterpret_cast<SDKManager *>(native_ptr);
        const char *c_url = env->GetStringUTFChars(url, nullptr);
        sdkManager->connect(c_url);
        env->ReleaseStringUTFChars(url, c_url);
    }
}

JNIEXPORT jlong JNICALL
Java_com_example_notificationcentersdk_SDKManager_nativeGetBusinessManager(JNIEnv *env, jobject thiz, jlong native_ptr) {
    if (native_ptr) {
        auto *sdkManager = reinterpret_cast<SDKManager *>(native_ptr);
        return reinterpret_cast<jlong>(sdkManager->getBusinessManager().get());
    }
    return 0;
}

JNIEXPORT jint JNICALL
Java_com_example_notificationcentersdk_SDKManager_nativePoll(JNIEnv *env, jobject thiz, jlong native_ptr) {
    if (native_ptr) {
        auto *sdkManager = reinterpret_cast<SDKManager *>(native_ptr);
        return sdkManager->poll();
    }
    return 0;
}

#ifdef __cplusplus
}
#endif 