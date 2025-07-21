#pragma once

#include <functional>
#include <queue>
#include <mutex>
#include <atomic>
#include <memory>
#include <chrono>

/**
 * 主线程执行器
 * 用于将任务从工作线程抛回主线程执行
 * 用户需要定期调用 poll() 方法来处理主线程任务
 */
class MainThreadExecutor {
public:
    using Task = std::function<void()>;
    
    static MainThreadExecutor& getInstance() {
        static MainThreadExecutor instance;
        return instance;
    }
    
    /**
     * 提交任务到主线程执行
     * 线程安全，可以从任何线程调用
     */
    void post(Task task);
    
    /**
     * 延迟提交任务到主线程执行
     * 线程安全，可以从任何线程调用
     */
    void postDelayed(Task task, std::chrono::milliseconds delay);
    
    /**
     * 轮询并执行主线程任务
     * 用户需要定期调用此方法（比如在游戏循环中）
     * @return 执行的任务数量
     */
    size_t poll();
    
    /**
     * 获取待执行的任务数量
     */
    size_t pendingTaskCount() const;
    
    /**
     * 清空所有待执行的任务
     */
    void clear();

private:
    MainThreadExecutor() = default;
    ~MainThreadExecutor() = default;
    
    struct DelayedTask {
        Task task;
        std::chrono::steady_clock::time_point exec_time;
        
        bool operator<(const DelayedTask& other) const {
            return exec_time > other.exec_time;
        }
    };
    
    mutable std::mutex mutex_{};
    std::queue<Task> immediate_tasks_{};
    std::priority_queue<DelayedTask> delayed_tasks_{};
    std::atomic<size_t> task_count_{0};
}; 