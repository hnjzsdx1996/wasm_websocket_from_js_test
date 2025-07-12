#pragma once

class NonCopyable {
public:
    NonCopyable() = default;

    ~NonCopyable() = default;

    // 删除拷贝构造函数和赋值运算符
    NonCopyable(const NonCopyable &) = delete;

    NonCopyable(const NonCopyable &&) = delete;

    NonCopyable &operator=(const NonCopyable &) = delete;

    NonCopyable &operator=(const NonCopyable &&) = delete;

protected:
    // 允许派生类构造和析构
    NonCopyable(NonCopyable &&) = default; // 可选：删除或默认移动构造函数
    NonCopyable &operator=(NonCopyable &&) = default; // 可选：删除或默认移动赋值运算符
};
