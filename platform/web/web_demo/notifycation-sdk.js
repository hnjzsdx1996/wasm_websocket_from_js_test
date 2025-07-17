// NotifycationSDKManager 封装 NotificationCenterSDK 的加载与操作
// 依赖 NotificationCenterSDK.js 已经通过 <script> 引入

const NotifycationSDKManager = (function() {
    let sdk = null;
    let sdkHandle = null;
    let pollingInterval = null;
    let isPolling = false;
    let log = function(msg) { console.log(msg); };

    // 允许外部设置日志函数
    function setLogger(logger) {
        log = logger || log;
    }

    // 加载并初始化SDK，返回Promise
    function loadAndInit(configObj, callbacks = {}) {
        return new Promise((resolve, reject) => {
            if (typeof NotificationCenterSDK !== 'function') {
                log('NotificationCenterSDK 工厂函数未加载');
                reject('NotificationCenterSDK 工厂函数未加载');
                return;
            }
            NotificationCenterSDK().then(function(ModuleInstance) {
                sdk = ModuleInstance;
                sdkHandle = sdk._js_sdk_create();
                // 配置参数
                const configStr = JSON.stringify(configObj || { sn: "1234567890", ping_pong_interval: 100 });
                const configPtr = sdk._malloc(sdk.lengthBytesUTF8(configStr) + 1);
                sdk.stringToUTF8(configStr, configPtr, sdk.lengthBytesUTF8(configStr) + 1);
                sdk._js_sdk_configure(sdkHandle, configPtr);
                sdk._free(configPtr);
                // 创建 JSWebSocket 并注入
                const wsHandle = sdk._js_sdk_create_js_websocket();
                sdk._js_sdk_set_websocket(sdkHandle, wsHandle);
                // 注册回调
                if (callbacks.onMessage) {
                    sdk._js_sdk_set_message_callback(sdkHandle, sdk.addFunction(function(msgPtr, userData) {
                        const msg = sdk.UTF8ToString(msgPtr);
                        callbacks.onMessage(msg);
                        sdk._free(msgPtr);
                    }, 'vii'), 0);
                }
                if (callbacks.onOpen) {
                    sdk._js_sdk_set_open_callback(sdkHandle, sdk.addFunction(function(userData) {
                        callbacks.onOpen();
                    }, 'vi'), 0);
                }
                if (callbacks.onClose) {
                    sdk._js_sdk_set_close_callback(sdkHandle, sdk.addFunction(function(userData) {
                        callbacks.onClose();
                    }, 'vi'), 0);
                }
                if (callbacks.onError) {
                    sdk._js_sdk_set_error_callback(sdkHandle, sdk.addFunction(function(errPtr, userData) {
                        const err = sdk.UTF8ToString(errPtr);
                        callbacks.onError(err);
                        sdk._free(errPtr);
                    }, 'vii'), 0);
                }
                log('WASM加载完成, 可操作WebSocket');
                resolve();
            }).catch(reject);
        });
    }

    function connect(url) {
        if (!sdk || !sdkHandle) {
            log('WASM未加载完成');
            return;
        }
        const urlPtr = sdk._malloc(sdk.lengthBytesUTF8(url) + 1);
        sdk.stringToUTF8(url, urlPtr, sdk.lengthBytesUTF8(url) + 1);
        sdk._js_sdk_connect(sdkHandle, urlPtr);
        sdk._free(urlPtr);
        log('已请求连接: ' + url);
    }

    function sendMsg(msg) {
        if (!sdk || !sdkHandle) {
            log('WASM未加载完成');
            return;
        }
        const msgPtr = sdk._malloc(sdk.lengthBytesUTF8(msg) + 1);
        sdk.stringToUTF8(msg, msgPtr, sdk.lengthBytesUTF8(msg) + 1);
        sdk._js_sdk_send(sdkHandle, msgPtr);
        sdk._free(msgPtr);
        log('发送: ' + msg);
    }

    function close() {
        if (sdk && sdkHandle) sdk._js_sdk_close(sdkHandle);
        log('已请求断开');
    }

    function startPolling() {
        if (!sdk || !sdkHandle) {
            log('WASM未加载完成');
            return;
        }
        if (isPolling) {
            log('轮询已在运行中');
            return;
        }
        isPolling = true;
        pollingInterval = setInterval(() => {
            const executedTasks = sdk._js_sdk_poll(sdkHandle);
            // if (executedTasks > 0) { log(`主线程执行器: 执行了 ${executedTasks} 个任务`); }
        }, 16);
        log('开始轮询主线程任务 (60FPS)');
    }

    function listenAircraftLocation(deviceSn, freq, msgCb, resultCb) {
        if (!sdk || !sdkHandle) {
            log('WASM未加载完成');
            return;
        }
        const msgCbPtr = sdk.addFunction(function(msgPtr, userData) {
            const msg = sdk.UTF8ToString(msgPtr);
            msgCb && msgCb(msg);
        }, 'vii');
        const resultCbPtr = sdk.addFunction(function(result, userData) {
            resultCb && resultCb(result);
        }, 'vii');
        const deviceSnLen = sdk.lengthBytesUTF8(deviceSn) + 1;
        const deviceSnPtr = sdk._malloc(deviceSnLen);
        sdk.stringToUTF8(deviceSn, deviceSnPtr, deviceSnLen);
        sdk._js_sdk_listen_aircraft_location(sdkHandle, msgCbPtr, 0, resultCbPtr, 0, deviceSnPtr, freq);
        sdk._free(deviceSnPtr);
    }

    // 文件系统相关
    function getFS() {
        return sdk && sdk.FS ? sdk.FS : null;
    }
    function getSDK() { return sdk; }
    function getSDKHandle() { return sdkHandle; }

    return {
        setLogger,
        loadAndInit,
        connect,
        sendMsg,
        close,
        startPolling,
        listenAircraftLocation,
        getFS,
        getSDK,
        getSDKHandle
    };
})();
