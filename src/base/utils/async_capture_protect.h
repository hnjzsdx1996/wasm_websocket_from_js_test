#pragma once

#include <memory>

#define WeakDummy(__TARGET__) \
    std::weak_ptr<Dummy> __TARGET__(this->dummy_ptr_)

#define WeakDummyReturn(__TARGET__) \
    std::shared_ptr<Dummy> strong_dummy_prt = __TARGET__.lock(); \
    if (!strong_dummy_prt) return;


class Dummy {
public:
    Dummy() = default;
    ~Dummy() = default;
};

class AsyncCaptureProtect {
public:
    AsyncCaptureProtect() {
        dummy_ptr_ = std::make_shared<Dummy>();
    }
    virtual ~AsyncCaptureProtect() {
        dummy_ptr_ = nullptr;
    }
    std::shared_ptr<Dummy> dummy_ptr_;
};
