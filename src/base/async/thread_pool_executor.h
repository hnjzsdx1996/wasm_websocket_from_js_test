#pragma once

#include <memory>
#include "thread_pool.h"

#include "../utils/non_copyable.h"

class ThreadPoolExecutor : public NonCopyable {
public:
    static ThreadPoolExecutor &getInstance() {
        static ThreadPoolExecutor instance;
        return instance;
    }

    static std::shared_ptr<ThreadPool> Worker() {
        if (worker_ == nullptr) {
            worker_ = std::make_shared<ThreadPool>();
        }
        return worker_;
    }

    static std::shared_ptr<ThreadPool> IO() {
        if (io_ == nullptr) {
            io_ = std::make_shared<ThreadPool>();
        }
        return io_;
    }

    static std::shared_ptr<ThreadPool> Compute() {
        if (compute_ == nullptr) {
            compute_ = std::make_shared<ThreadPool>();
        }
        return compute_;
    }

private:
    static std::shared_ptr<ThreadPool> worker_;
    static std::shared_ptr<ThreadPool> io_;
    static std::shared_ptr<ThreadPool> compute_;
};
