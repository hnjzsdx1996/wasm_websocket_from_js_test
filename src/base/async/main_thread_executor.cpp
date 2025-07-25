#include "main_thread_executor.h"
#include "../logger/logger.h"

void MainThreadExecutor::post(Task task) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        immediate_tasks_.emplace(std::move(task));
        task_count_.fetch_add(1);
    }
    NC_LOG_DEBUG("MainThreadExecutor: 提交立即任务，当前任务数: %zu", task_count_.load());
}

void MainThreadExecutor::postDelayed(Task task, std::chrono::milliseconds delay) {
    auto exec_time = std::chrono::steady_clock::now() + delay;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        delayed_tasks_.emplace(DelayedTask{std::move(task), exec_time});
        task_count_.fetch_add(1);
    }
    NC_LOG_DEBUG("MainThreadExecutor: 提交延迟任务，延迟: %lldms，当前任务数: %zu", delay.count(), task_count_.load());
}

size_t MainThreadExecutor::poll() {
    size_t executed_count = 0;
    std::vector<Task> tasks_to_execute;
    
    {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // 处理立即任务
        while (!immediate_tasks_.empty()) {
            tasks_to_execute.emplace_back(std::move(immediate_tasks_.front()));
            immediate_tasks_.pop();
            task_count_.fetch_sub(1);
        }
        
        // 处理延迟任务
        auto now = std::chrono::steady_clock::now();
        while (!delayed_tasks_.empty()) {
            const auto& next_task = delayed_tasks_.top();
            if (next_task.exec_time <= now) {
                tasks_to_execute.emplace_back(next_task.task);
                delayed_tasks_.pop();
                task_count_.fetch_sub(1);
            } else {
                break;
            }
        }
    }
    
    // 执行任务（在锁外执行，避免死锁）
    for (auto& task : tasks_to_execute) {
        try {
            task();
            executed_count++;
        } catch (const std::exception& e) {
            NC_LOG_ERROR("MainThreadExecutor: 任务执行异常: %s", e.what());
        } catch (...) {
            NC_LOG_ERROR("MainThreadExecutor: 任务执行未知异常");
        }
    }
    
    if (executed_count > 0) {
        NC_LOG_DEBUG("MainThreadExecutor: 执行了 %zu 个任务，剩余任务数: %zu", 
                     executed_count, task_count_.load());
    }
    
    return executed_count;
}

size_t MainThreadExecutor::pendingTaskCount() const {
    return task_count_.load();
}

void MainThreadExecutor::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::queue<Task>().swap(immediate_tasks_);
    std::priority_queue<DelayedTask>().swap(delayed_tasks_);
    task_count_.store(0);
    NC_LOG_INFO("MainThreadExecutor: 清空所有任务");
} 