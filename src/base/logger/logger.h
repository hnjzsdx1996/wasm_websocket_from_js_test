#pragma once
#include <plog/Log.h>
#include <plog/Appenders/RollingFileAppender.h>
#include <plog/Appenders/ConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>
#include <memory>
#include <cstdio>
#include <cstdarg>
#include <sstream>

#include "plog/Initializers/RollingFileInitializer.h"
#include "thread_info.h"
#include "custom_formatter.h"

// 文件名截取工具
#ifndef FILE_NAME
#define FILE_NAME(x) (strrchr(x, '/') ? strrchr(x, '/') + 1 : x)
#endif

namespace nc_logger_detail {
    inline std::string format_string(const char* fmt, ...) {
        char buf[1024];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        return std::string(buf);
    }
}

// 日志开关宏
#if DISABLE_ALL_LOG

#define NC_LOG_DEBUG(...) ((void)0)
#define NC_LOG_INFO(...)  ((void)0)
#define NC_LOG_WARN(...)  ((void)0)
#define NC_LOG_ERROR(...) ((void)0)

#else

#define NC_LOG_DEBUG(fmt, ...) PLOGD << nc_logger_detail::format_string(fmt, ##__VA_ARGS__)
#define NC_LOG_INFO(fmt, ...)  PLOGI << nc_logger_detail::format_string(fmt, ##__VA_ARGS__)
#define NC_LOG_WARN(fmt, ...)  PLOGW << nc_logger_detail::format_string(fmt, ##__VA_ARGS__)
#define NC_LOG_ERROR(fmt, ...) PLOGE << nc_logger_detail::format_string(fmt, ##__VA_ARGS__)

#endif

#if LOG_NO_DEBUG
#undef NC_LOG_DEBUG
#define NC_LOG_DEBUG(...) ((void)0)
#endif

namespace nc_logger {

// 日志初始化（只注册一次 appender，避免日志重复）
inline void init(plog::Severity severity = plog::info, const char* file = nullptr, bool alsoConsole = true) {
    static std::shared_ptr<plog::IAppender> fileAppender;
    static std::shared_ptr<plog::IAppender> consoleAppender;
    static bool initialized = false;
    if (!initialized) {
        // 设置日志格式包含线程 ID
        plog::init(severity);

        if (file && file[0] != '\0') {
            fileAppender = std::make_shared<plog::RollingFileAppender<plog::TxtFormatter>>(file);
            plog::get()->addAppender(fileAppender.get());
        }

        if (alsoConsole) {
            consoleAppender = std::make_shared<plog::ConsoleAppender<plog::TxtFormatter>>();
            plog::get()->addAppender(consoleAppender.get());
        }

        initialized = true;
    }
}

// 设置日志级别
inline void set_level(plog::Severity severity) {
    if (plog::get()) plog::get()->setMaxSeverity(severity);
}

} // namespace nc_logger