#pragma once
#include <string>
#include <thread>

namespace nc_logger {
    // thread_local 变量保存当前线程名
    extern thread_local std::string current_thread_name;
    
    // 设置当前线程名
    inline void set_thread_name(const std::string& name) {
        current_thread_name = name;
    }
    
    // 获取当前线程名
    inline std::string get_thread_name() {
        return current_thread_name.empty() ? "Main" : current_thread_name;
    }
} 