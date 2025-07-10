#include "JSWebSocket.h"
#include <emscripten/bind.h>
#include <emscripten.h>
#include <unordered_map>
#include <string>

namespace {
// 管理C++ wsId与JS端WebSocket对象的映射
static int g_next_ws_id = 1;
static std::unordered_map<int, JSWebSocket*> g_ws_map;
}

// JS桥接函数声明（不能放在extern "C"内）
EM_JS(void, js_ws_connect, (int wsId, const char* url), {
  if (!window.cppWsMap) window.cppWsMap = {};
  let ws = new WebSocket(UTF8ToString(url));
  window.cppWsMap[wsId] = ws;
  ws.onopen = function() { Module._js_ws_on_open(wsId); };
  ws.onmessage = function(e) {
    var str = e.data;
    var arr = intArrayFromString(str);
    var ptr = stackAlloc(arr.length);
    HEAP8.set(arr, ptr);
    Module._js_ws_on_message(wsId, ptr);
  };
  ws.onclose = function() { Module._js_ws_on_close(wsId); };
  ws.onerror = function(e) {
    var arr = intArrayFromString('error');
    var ptr = stackAlloc(arr.length);
    HEAP8.set(arr, ptr);
    Module._js_ws_on_error(wsId, ptr);
  };
});

EM_JS(void, js_ws_send, (int wsId, const char* msg), {
  let ws = window.cppWsMap && window.cppWsMap[wsId];
  if (ws && ws.readyState === 1) ws.send(UTF8ToString(msg));
});

EM_JS(void, js_ws_close, (int wsId), {
  let ws = window.cppWsMap && window.cppWsMap[wsId];
  if (ws) ws.close();
});

JSWebSocket::JSWebSocket() {
    wsId = g_next_ws_id++;
    g_ws_map[wsId] = this;
}

JSWebSocket::~JSWebSocket() {
    g_ws_map.erase(wsId);
    js_ws_close(wsId);
}

void JSWebSocket::connect(const std::string& url) {
    js_ws_connect(wsId, url.c_str());
}

void JSWebSocket::send(const std::string& message) {
    js_ws_send(wsId, message.c_str());
}

void JSWebSocket::close() {
    js_ws_close(wsId);
}

// 供JS回调
void JSWebSocket::onOpenFromJS() {
    if (onOpen) onOpen();
}
void JSWebSocket::setOnMessageCallback(std::function<void(std::string)> cb) {
    cppOnMessageCallback = std::move(cb);
}
void JSWebSocket::onMessageFromJS(const std::string& msg) {
    if (onMessage) onMessage(msg);
    if (cppOnMessageCallback) cppOnMessageCallback(msg);
}
void JSWebSocket::onCloseFromJS() {
    if (onClose) onClose();
}
void JSWebSocket::onErrorFromJS(const std::string& err) {
    if (onError) onError(err);
}

// C++导出给JS的回调
extern "C" {
EMSCRIPTEN_KEEPALIVE void js_ws_on_open(int wsId) {
    auto it = g_ws_map.find(wsId);
    if (it != g_ws_map.end()) it->second->onOpenFromJS();
}
EMSCRIPTEN_KEEPALIVE void js_ws_on_message(int wsId, const char* msg) {
    auto it = g_ws_map.find(wsId);
    if (it != g_ws_map.end()) it->second->onMessageFromJS(std::string(msg));
}
EMSCRIPTEN_KEEPALIVE void js_ws_on_close(int wsId) {
    auto it = g_ws_map.find(wsId);
    if (it != g_ws_map.end()) it->second->onCloseFromJS();
}
EMSCRIPTEN_KEEPALIVE void js_ws_on_error(int wsId, const char* err) {
    auto it = g_ws_map.find(wsId);
    if (it != g_ws_map.end()) it->second->onErrorFromJS(std::string(err));
}
}

// 绑定到JS
EMSCRIPTEN_BINDINGS(js_ws_bindings) {
    emscripten::class_<JSWebSocket>("JSWebSocket")
        .constructor<>()
        .function("connect", &JSWebSocket::connect)
        .function("send", &JSWebSocket::send)
        .function("close", &JSWebSocket::close);
} 