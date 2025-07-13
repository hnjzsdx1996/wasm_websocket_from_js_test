#pragma once

#include <memory>
#include <functional>
#include <chrono>
#include <atomic>
#include <mutex>
#include <unordered_map>
#include <thread>
#include <condition_variable>
#include <queue>
#include "thread_pool.h"
#include "../utils/non_copyable.h"

class Timer : public NonCopyable {
public:
    using TaskId = uint64_t;
    using TaskCallback = std::function<void()>;
    using Duration = std::chrono::milliseconds;

    struct TimerTask {
        TaskId id;
        TaskCallback callback;
        std::chrono::steady_clock::time_point next_exec_time;
        Duration interval;
        bool repeat;
        std::shared_ptr<ThreadPool> executor;
        
        bool operator<(const TimerTask& other) const {
            return next_exec_time > other.next_exec_time;
        }
    };

    explicit Timer(std::shared_ptr<ThreadPool> default_executor = nullptr);
    ~Timer();

    /**
     * 延迟执行一次任务
     * @param delay 延迟时间
     * @param task 要执行的任务
     * @param executor 执行任务的线程池，如果为nullptr则使用默认执行器
     * @return 任务ID，可用于取消任务
     */
    TaskId PostDelayed(Duration delay, TaskCallback task, 
                      std::shared_ptr<ThreadPool> executor = nullptr);

    /**
     * 延迟执行重复任务
     * @param initial_delay 初始延迟时间
     * @param interval 重复间隔时间
     * @param task 要执行的任务
     * @param executor 执行任务的线程池，如果为nullptr则使用默认执行器
     * @return 任务ID，可用于取消任务
     */
    TaskId PostRepeating(Duration initial_delay, Duration interval, 
                        TaskCallback task, 
                        std::shared_ptr<ThreadPool> executor = nullptr);

    /**
     * 取消任务
     * @param task_id 任务ID
     * @return 是否成功取消
     */
    bool Cancel(TaskId task_id);

    /**
     * 取消所有任务
     */
    void CancelAll();

    /**
     * 设置默认执行器
     * @param executor 默认线程池执行器
     */
    void SetDefaultExecutor(std::shared_ptr<ThreadPool> executor);

    /**
     * 获取当前活跃任务数量
     * @return 活跃任务数量
     */
    size_t GetActiveTaskCount() const;

private:
    void workerThread();
    void addTask(TimerTask task);
    void removeTask(TaskId task_id);
    TaskId generateTaskId();

    std::priority_queue<TimerTask> tasks_;
    std::unordered_map<TaskId, TimerTask> active_tasks_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread worker_;
    std::atomic<bool> stop_;
    std::atomic<TaskId> next_task_id_;
    std::shared_ptr<ThreadPool> default_executor_;
    std::atomic<size_t> active_task_count_;
}; 