#pragma once

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <functional>
#include <atomic>
#include <string>
#include <utility>
#include "../logger/thread_info.h"

class ThreadPool {
public:
    explicit ThreadPool(std::string  name = "") : name_(std::move(name)), stop_(false), running_task_(false) {
        worker_ = std::thread(&ThreadPool::workerThread, this);
    }

    ~ThreadPool() { {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_ = true;
        }
        condition_.notify_all();
        if (worker_.joinable()) {
            worker_.join();
        }
    }

    // 立即执行任务
    void Post(std::function<void()> task) {
        addTask(Task{std::move(task), std::chrono::steady_clock::now()});
    }

    // 延迟执行任务
    void PostDelayed(std::function<void()> task, std::chrono::milliseconds delay) {
        auto exec_time = std::chrono::steady_clock::now() + delay;
        addTask(Task{std::move(task), exec_time});
    }

private:
    struct Task {
        std::function<void()> func;
        std::chrono::steady_clock::time_point exec_time;
        bool operator<(const Task &other) const {
            return exec_time > other.exec_time;
        }
    };

    std::priority_queue<Task> tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread worker_;
    std::atomic<bool> stop_;
    bool running_task_;
    std::string name_;

    void addTask(Task task) { {
            std::unique_lock<std::mutex> lock(mutex_);
            tasks_.emplace(std::move(task));
        }
        condition_.notify_one();
    }

    void workerThread() {
        // 设置当前线程名
        nc_logger::set_thread_name(name_);
        while (true) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (stop_) {
                break;
            }
            if (!tasks_.empty()) {
                auto now = std::chrono::steady_clock::now();
                auto &next_task = tasks_.top();
                if (next_task.exec_time <= now) {
                    auto task = std::move(next_task.func);
                    tasks_.pop();
                    running_task_ = true;
                    lock.unlock();
                    task();
                    running_task_ = false;
                } else {
                    condition_.wait_until(lock, next_task.exec_time);
                }
            } else {
                if (stop_) {
                    break;
                }
                condition_.wait(lock);
            }
        }
    }
};
