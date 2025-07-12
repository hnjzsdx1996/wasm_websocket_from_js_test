#pragma once

#include <plog/Record.h>
#include <sstream>
#include <ctime>
#include <thread>
#include <cstdio>
#include "thread_info.h"

class CustomFormatter
{
public:
    static plog::util::nstring header()
    {
        return PLOG_NSTR("L/T/");
    }

    static plog::util::nstring format(const plog::Record& record)
    {
        plog::util::nostringstream ss;
        
        // 格式化时间
        time_t time = record.getTime().time;
        struct tm* timeinfo = localtime(&time);
        char timeStr[32];
        strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        
        // 添加毫秒（固定3位）
        int milliseconds = record.getTime().millitm;
        char msStr[8];
        snprintf(msStr, sizeof(msStr), "%03d", milliseconds);
        
        // 格式化线程名（固定10位）
        std::string threadName = nc_logger::get_thread_name();
        char threadStr[16];
        snprintf(threadStr, sizeof(threadStr), "%-10s", threadName.c_str());
        
        ss << PLOG_NSTR("[") << timeStr << PLOG_NSTR(".") << msStr << PLOG_NSTR("] ")
           << plog::severityToString(record.getSeverity()) << PLOG_NSTR(" ")
           << PLOG_NSTR("[") << std::this_thread::get_id() << PLOG_NSTR(":") << threadStr << PLOG_NSTR("] ")
           << PLOG_NSTR("[") << record.getFunc() << PLOG_NSTR("@") << record.getLine() << PLOG_NSTR("] ")
           << record.getMessage() << PLOG_NSTR("\n");
        return ss.str();
    }
}; 