#include "CppWebSocket.h"
#include "base/logger/logger.h"

#ifdef ENABLE_LIBWEBSOCKETS

#include <libwebsockets.h>
#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>

#include "base/async/thread_pool_executor.h"
#include "base/utils/async_capture_protect.h"


class CppWebSocket::Impl : public AsyncCaptureProtect {
public:
    Impl(CppWebSocket* parent)
        : parent_(parent), context(nullptr), wsi(nullptr), connected(false), exitFlag(false) {}
    ~Impl() { close(); }

    // 连接 WebSocket
    void connect(const std::string& url) {
        this->url = url;
        exitFlag = false;
        // 启动事件循环线程
        WeakDummy(weak_ptr);
        ThreadPoolExecutor::IO()->Post([this, weak_ptr]()->void {
            WeakDummyReturn(weak_ptr);
            this->serviceLoop();
        });
    }

    // 发送消息
    void send(const std::string& message) {
        std::lock_guard<std::mutex> lock(sendMutex);
        sendQueue.push(message);
        if (wsi) {
            lws_callback_on_writable(wsi);
        }
    }

    // 关闭连接
    void close() {
        exitFlag = true;
        if (context) {
            lws_cancel_service(context);
        }
        // 资源释放在析构
    }

    // 是否已连接
    bool isOpen() const {
        return connected.load();
    }

private:
    // 事件循环线程, IO线程
    void serviceLoop() {
        // 协议定义
        static struct lws_protocols protocols[] = {
            { "cpp-websocket-protocol", &CppWebSocket::Impl::lwsCallback, sizeof(void*), 4096, 0, nullptr, 0 },
            { nullptr, nullptr, 0, 0, 0, nullptr, 0 }
        };
        struct lws_context_creation_info info;
        memset(&info, 0, sizeof(info));
        info.port = -1; // CONTEXT_PORT_NO_LISTEN 通常为 -1
        info.protocols = protocols;
        info.gid = -1;
        info.uid = -1;
        // 为客户端 SSL 连接启用 SSL 全局初始化
        info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        context = lws_create_context(&info);
        if (!context) {
            NC_LOG_INFO("CppWebSocket: 创建 lws_context 失败");
            WeakDummy(weak_ptr);
            ThreadPoolExecutor::Main().post([this, weak_ptr]()->void {
                WeakDummyReturn(weak_ptr);
                parent_->callOnError("lws_context create failed");
            });
            return;
        }
        // 解析 ws://host:port/path
        std::string address, path;
        int port = 80;
        bool ssl = false;
        if (!parseUrl(url, address, port, path, ssl)) {
            NC_LOG_INFO("CppWebSocket: URL 解析失败: %s", url.c_str());
            WeakDummy(weak_ptr);
            ThreadPoolExecutor::Main().post([this, weak_ptr]()->void {
                WeakDummyReturn(weak_ptr);
                parent_->callOnError("url parse failed");
            });
            return;
        }
        struct lws_client_connect_info ccinfo = {0};
        ccinfo.context = context;
        ccinfo.address = address.c_str();
        ccinfo.port = port;
        ccinfo.path = path.c_str();
        ccinfo.host = address.c_str();
        ccinfo.origin = address.c_str();
        ccinfo.protocol = protocols[0].name;
        ccinfo.ssl_connection = ssl ? LCCSCF_USE_SSL : 0;
        ccinfo.pwsi = &wsi;
        ccinfo.userdata = this;
        wsi = lws_client_connect_via_info(&ccinfo);
        if (!wsi) {
            NC_LOG_INFO("CppWebSocket: 连接失败");
            WeakDummy(weak_ptr);
            ThreadPoolExecutor::Main().post([this, weak_ptr]()->void {
                WeakDummyReturn(weak_ptr);
                parent_->callOnError("connect failed");
            });
            lws_context_destroy(context);
            context = nullptr;
            return;
        }
        // 事件循环
        while (!exitFlag) {
            lws_service(context, 50);
        }
        lws_context_destroy(context);
        context = nullptr;
        wsi = nullptr;
        connected = false;
    }

    // lws 回调函数（静态）
    static int lwsCallback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
        Impl* self = reinterpret_cast<Impl*>(lws_wsi_user(wsi));
        if (!self) {
            if (reason == LWS_CALLBACK_CLIENT_ESTABLISHED || reason == LWS_CALLBACK_CLIENT_RECEIVE || reason == LWS_CALLBACK_CLIENT_WRITEABLE || reason == LWS_CALLBACK_CLIENT_CLOSED || reason == LWS_CALLBACK_CLIENT_CONNECTION_ERROR) {
                self = reinterpret_cast<Impl*>(user);
            }
        }
        if (!self) return 0;
        switch (reason) {
            case LWS_CALLBACK_CLIENT_ESTABLISHED:
                self->connected = true;
                NC_LOG_INFO("CppWebSocket: 连接已建立");
                ThreadPoolExecutor::Main().post([self]()->void {
                    self->parent_->callOnOpen();
                });
                break;
            case LWS_CALLBACK_CLIENT_RECEIVE:
                if (in && len > 0) {
                    std::string msg((const char*)in, len);
                    NC_LOG_INFO("CppWebSocket: 收到消息: %s", msg.c_str());
                    ThreadPoolExecutor::Main().post([self, msg]()->void {
                        self->parent_->callOnMessage(msg);
                    });
                }
                break;
            case LWS_CALLBACK_CLIENT_WRITEABLE: {
                std::string msg;
                {
                    std::lock_guard<std::mutex> lock(self->sendMutex);
                    if (!self->sendQueue.empty()) {
                        msg = self->sendQueue.front();
                        self->sendQueue.pop();
                    }
                }
                if (!msg.empty()) {
                    std::vector<unsigned char> buf(LWS_PRE + msg.size());
                    memcpy(buf.data() + LWS_PRE, msg.data(), msg.size());
                    int n = lws_write(wsi, buf.data() + LWS_PRE, msg.size(), LWS_WRITE_TEXT);
                    if (n < 0) {
                        NC_LOG_INFO("CppWebSocket: 发送失败");
                        ThreadPoolExecutor::Main().post([self]()->void {
                            self->parent_->callOnError("send failed");
                        });
                    } else {
                        NC_LOG_INFO("CppWebSocket: 已发送: %s", msg.c_str());
                    }
                }
                // 继续触发写事件（如果还有数据）
                {
                    std::lock_guard<std::mutex> lock(self->sendMutex);
                    if (!self->sendQueue.empty()) {
                        lws_callback_on_writable(wsi);
                    }
                }
                break;
            }
            case LWS_CALLBACK_CLIENT_CLOSED:
                self->connected = false;
                NC_LOG_INFO("CppWebSocket: 连接已关闭");
                ThreadPoolExecutor::Main().post([self]()->void {
                    self->parent_->callOnClose();
                });
                break;
            case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
                self->connected = false;
                NC_LOG_INFO("CppWebSocket: 连接出错");
                ThreadPoolExecutor::Main().post([self]()->void {
                    self->parent_->callOnError("connection error");
                });
                break;
            default:
                break;
        }
        return 0;
    }

    // URL 解析工具
    static bool parseUrl(const std::string& url, std::string& address, int& port, std::string& path, bool& ssl) {
        // 支持 ws://host:port/path 或 wss://host:port/path
        ssl = false;
        size_t pos = 0;
        if (url.find("ws://") == 0) {
            pos = 5;
            ssl = false;
        } else if (url.find("wss://") == 0) {
            pos = 6;
            ssl = true;
        } else {
            return false;
        }
        size_t slash = url.find('/', pos);
        size_t colon = url.find(':', pos);
        if (colon != std::string::npos && (slash == std::string::npos || colon < slash)) {
            address = url.substr(pos, colon - pos);
            size_t port_start = colon + 1;
            size_t port_end = (slash == std::string::npos) ? url.size() : slash;
            port = std::stoi(url.substr(port_start, port_end - port_start));
        } else {
            address = url.substr(pos, (slash == std::string::npos) ? url.size() - pos : slash - pos);
            port = ssl ? 443 : 80;
        }
        if (slash != std::string::npos) {
            path = url.substr(slash);
        } else {
            path = "/";
        }
        return true;
    }

    CppWebSocket* parent_;
    struct lws_context* context;
    struct lws* wsi;
    std::atomic<bool> connected;
    std::atomic<bool> exitFlag;
    std::string url;
    std::queue<std::string> sendQueue;
    std::mutex sendMutex;
};

CppWebSocket::CppWebSocket() : pImpl(new Impl(this)) {}

CppWebSocket::~CppWebSocket() {
    if (pImpl) {
        pImpl->close();
        pImpl.reset();
    }
}

void CppWebSocket::connect(const std::string& url) {
    NC_LOG_INFO("CppWebSocket: 连接 %s", url.c_str());
    pImpl->connect(url);
}

void CppWebSocket::send(const std::string& message) {
    NC_LOG_INFO("CppWebSocket: 发送 %s", message.c_str());
    pImpl->send(message);
}

void CppWebSocket::close() {
    NC_LOG_INFO("CppWebSocket: 关闭连接");
    pImpl->close();
}

bool CppWebSocket::isOpen() const {
    return pImpl->isOpen();
}

#else // !ENABLE_LIBWEBSOCKETS

CppWebSocket::CppWebSocket() : pImpl(nullptr) {
    NC_LOG_ERROR("CppWebSocket: ENABLE_LIBWEBSOCKETS == false");
}

CppWebSocket::~CppWebSocket() {
    NC_LOG_ERROR("CppWebSocket: ENABLE_LIBWEBSOCKETS == false");
    pImpl = nullptr;
}

void CppWebSocket::connect(const std::string&) {
    NC_LOG_ERROR("CppWebSocket: ENABLE_LIBWEBSOCKETS == false");
}

void CppWebSocket::send(const std::string&) {
    NC_LOG_ERROR("CppWebSocket: ENABLE_LIBWEBSOCKETS == false");
}

void CppWebSocket::close() {
    NC_LOG_ERROR("CppWebSocket: ENABLE_LIBWEBSOCKETS == false");
}

bool CppWebSocket::isOpen() const {
    NC_LOG_ERROR("CppWebSocket: ENABLE_LIBWEBSOCKETS == false");
    return false;
}

#endif // ENABLE_LIBWEBSOCKETS