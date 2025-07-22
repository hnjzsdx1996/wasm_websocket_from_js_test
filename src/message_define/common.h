#pragma once

// same as plog::Severity
enum class SdkLogLevel {
    NONE = 0,
    FATAL = 1,
    ERROR = 2,
    WARNING = 3,
    INFO = 4,
    DEBUG = 5,
    VERBOSE = 6
};

class SdkInitializeInfo {
public:
    // 日志配置
    std::string log_path;
    SdkLogLevel log_level = SdkLogLevel::NONE;
};
