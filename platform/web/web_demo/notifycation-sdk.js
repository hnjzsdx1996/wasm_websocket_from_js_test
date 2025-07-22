// NotifycationSDKManager 封装 NotificationCenterSDK 的加载与操作
// 依赖 NotificationCenterSDK.js 已经通过 <script> 引入

const NotifycationSDKManager = (function() {
    let sdk = null;
    let sdkHandle = null;
    let wsHandle = null;
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
                
                // 初始化SDK
                const logPath = configObj.log_path || "";
                const logLevel = configObj.log_level || 4; // SDK_LOG_INFO
                const logPathPtr = sdk._malloc(sdk.lengthBytesUTF8(logPath) + 1);
                sdk.stringToUTF8(logPath, logPathPtr, sdk.lengthBytesUTF8(logPath) + 1);
                sdk._js_sdk_init(sdkHandle, logPathPtr, logLevel);
                sdk._free(logPathPtr);
                
                // 创建 JSWebSocket 并注入
                wsHandle = sdk._js_sdk_create_js_websocket();
                sdk._js_sdk_set_websocket(sdkHandle, wsHandle);
                
                // 注册WebSocket事件监听器
                const onMessagePtr = callbacks.onMessage ? sdk.addFunction(function(msgPtr) {
                    const msg = sdk.UTF8ToString(msgPtr);
                    callbacks.onMessage(msg);
                }, 'vi') : 0;
                
                const onOpenPtr = callbacks.onOpen ? sdk.addFunction(function() {
                    callbacks.onOpen();
                }, 'v') : 0;
                
                const onClosePtr = callbacks.onClose ? sdk.addFunction(function() {
                    callbacks.onClose();
                }, 'v') : 0;
                
                const onErrorPtr = callbacks.onError ? sdk.addFunction(function(errPtr) {
                    const err = sdk.UTF8ToString(errPtr);
                    callbacks.onError(err);
                }, 'vi') : 0;
                
                sdk._js_sdk_set_websocket_event_listener(sdkHandle, onMessagePtr, onOpenPtr, onClosePtr, onErrorPtr);
                
                log('WASM加载完成, 可操作WebSocket');
                resolve();
            }).catch(reject);
        });
    }

    function connect(url) {
        if (!sdk || !sdkHandle || !wsHandle) {
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
        if (!sdk || !wsHandle) {
            log('WASM未加载完成');
            return;
        }
        // 通过WebSocket发送消息
        const msgPtr = sdk._malloc(sdk.lengthBytesUTF8(msg) + 1);
        sdk.stringToUTF8(msg, msgPtr, sdk.lengthBytesUTF8(msg) + 1);
        sdk._js_websocket_send(wsHandle, msgPtr);
        sdk._free(msgPtr);
        log('发送: ' + msg);
    }

    function close() {
        if (sdk && wsHandle) {
            sdk._js_websocket_close(wsHandle);
        }
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
        // 注意：新的API中没有listen_aircraft_location接口
        log('监听飞机位置功能在新的API中已被移除');
        return -1;
    }

    function cancelObserve(listenId) {
        if (!sdk || !sdkHandle) {
            log('WASM未加载完成');
            return;
        }
        sdk._js_sdk_cancel_observe(sdkHandle, BigInt(listenId));
        log('已请求取消监听: ' + listenId);
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
        cancelObserve,
        getFS,
        getSDK,
        getSDKHandle
    };
})();
