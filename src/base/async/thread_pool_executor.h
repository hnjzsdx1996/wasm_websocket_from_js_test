#pragma once

#include <memory>
#include "thread_pool.h"
#include "main_thread_executor.h"

#include "../utils/non_copyable.h"

class ThreadPoolExecutor : public NonCopyable {
public:
    static ThreadPoolExecutor &getInstance() {
        static ThreadPoolExecutor instance;
        return instance;
    }

    static std::shared_ptr<ThreadPool> Worker() {
        if (worker_ == nullptr) {
            worker_ = std::make_shared<ThreadPool>("Worker");
        }
        return worker_;
    }

    static std::shared_ptr<ThreadPool> IO() {
        if (io_ == nullptr) {
            io_ = std::make_shared<ThreadPool>("IO");
        }
        return io_;
    }

    static std::shared_ptr<ThreadPool> Compute() {
        if (compute_ == nullptr) {
            compute_ = std::make_shared<ThreadPool>("Compute");
        }
        return compute_;
    }

    /**
     * 获取主线程执行器
     * 用于将任务从工作线程抛回主线程
     */
    static MainThreadExecutor& Main() {
        return MainThreadExecutor::getInstance();
    }

private:
    static std::shared_ptr<ThreadPool> worker_;
    static std::shared_ptr<ThreadPool> io_;
    static std::shared_ptr<ThreadPool> compute_;
};
