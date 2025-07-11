mergeInto(LibraryManager.library, {
  js_websocket_connect: function(wsPtr, url) {
    const urlStr = UTF8ToString(url);
    if (!Module.wsMap) Module.wsMap = new Map();
    const wsMap = Module.wsMap;
    try {
      const ws = new WebSocket(urlStr);
      wsMap.set(wsPtr, ws);
      ws.onopen = function() {
        if (Module._js_websocket_on_open) Module._js_websocket_on_open(wsPtr);
      };
      ws.onmessage = function(event) {
        const len = Module.lengthBytesUTF8(event.data) + 1;
        const msgPtr = Module._malloc(len);
        Module.stringToUTF8(event.data, msgPtr, len);
        if (Module._js_websocket_on_message) Module._js_websocket_on_message(wsPtr, msgPtr);
        Module._free(msgPtr);
      };
      ws.onclose = function() {
        wsMap.delete(wsPtr);
        if (Module._js_websocket_on_close) Module._js_websocket_on_close(wsPtr);
      };
      ws.onerror = function(event) {
        const errMsg = 'WebSocket error';
        const errLen = Module.lengthBytesUTF8(errMsg) + 1;
        const errPtr = Module._malloc(errLen);
        Module.stringToUTF8(errMsg, errPtr, errLen);
        if (Module._js_websocket_on_error) Module._js_websocket_on_error(wsPtr, errPtr);
        Module._free(errPtr);
      };
    } catch (error) {
      const errMsg = error.message;
      const errLen = Module.lengthBytesUTF8(errMsg) + 1;
      const errPtr = Module._malloc(errLen);
      Module.stringToUTF8(errMsg, errPtr, errLen);
      if (Module._js_websocket_on_error) Module._js_websocket_on_error(wsPtr, errPtr);
      Module._free(errPtr);
    }
  },
  js_websocket_send: function(wsPtr, message) {
    const wsMap = Module.wsMap;
    if (!wsMap) return;
    const ws = wsMap.get(wsPtr);
    if (!ws) return;
    if (ws.readyState !== WebSocket.OPEN) return;
    const messageStr = UTF8ToString(message);
    ws.send(messageStr);
  },
  js_websocket_close: function(wsPtr) {
    const wsMap = Module.wsMap;
    if (!wsMap) return;
    const ws = wsMap.get(wsPtr);
    if (ws) {
      ws.close();
      wsMap.delete(wsPtr);
    }
  },
  js_websocket_is_open: function(wsPtr) {
    const wsMap = Module.wsMap;
    if (!wsMap) return 0;
    const ws = wsMap.get(wsPtr);
    return ws && ws.readyState === WebSocket.OPEN;
  }
});