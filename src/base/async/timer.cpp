#include "timer.h"
#include <iostream>

Timer::Timer(std::shared_ptr<ThreadPool> default_executor)
    : stop_(false)
    , next_task_id_(1)
    , default_executor_(std::move(default_executor))
    , active_task_count_(0) {
    worker_ = std::thread(&Timer::workerThread, this);
}

Timer::~Timer() {
    {
        std::unique_lock<std::mutex> lock(mutex_);
        stop_ = true;
    }
    condition_.notify_all();
    if (worker_.joinable()) {
        worker_.join();
    }
}

Timer::TaskId Timer::PostDelayed(Duration delay, TaskCallback task, 
                                std::shared_ptr<ThreadPool> executor) {
    auto task_id = generateTaskId();
    auto exec_time = std::chrono::steady_clock::now() + delay;
    
    TimerTask timer_task{
        .id = task_id,
        .callback = std::move(task),
        .next_exec_time = exec_time,
        .interval = Duration::zero(),
        .repeat = false,
        .executor = executor ? executor : default_executor_
    };
    
    addTask(std::move(timer_task));
    return task_id;
}

Timer::TaskId Timer::PostRepeating(Duration initial_delay, Duration interval, 
                                  TaskCallback task, 
                                  std::shared_ptr<ThreadPool> executor) {
    auto task_id = generateTaskId();
    auto exec_time = std::chrono::steady_clock::now() + initial_delay;
    
    TimerTask timer_task{
        .id = task_id,
        .callback = std::move(task),
        .next_exec_time = exec_time,
        .interval = interval,
        .repeat = true,
        .executor = executor ? executor : default_executor_
    };
    
    addTask(std::move(timer_task));
    return task_id;
}

bool Timer::Cancel(TaskId task_id) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    auto it = active_tasks_.find(task_id);
    if (it == active_tasks_.end()) {
        return false;
    }
    
    removeTask(task_id);
    return true;
}

void Timer::CancelAll() {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // 清空优先级队列
    while (!tasks_.empty()) {
        tasks_.pop();
    }
    
    // 清空活跃任务映射
    active_tasks_.clear();
    active_task_count_.store(0);
}

void Timer::SetDefaultExecutor(std::shared_ptr<ThreadPool> executor) {
    default_executor_ = std::move(executor);
}

size_t Timer::GetActiveTaskCount() const {
    return active_task_count_.load();
}

void Timer::workerThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        if (stop_) {
            break;
        }
        
        if (!tasks_.empty()) {
            auto now = std::chrono::steady_clock::now();
            auto& next_task = tasks_.top();
            
            if (next_task.next_exec_time <= now) {
                // 执行任务
                auto task = std::move(next_task);
                tasks_.pop();
                
                // 从活跃任务映射中移除
                active_tasks_.erase(task.id);
                active_task_count_.fetch_sub(1);
                
                // 保存回调函数的副本用于执行
                auto callback = task.callback;
                
                lock.unlock();
                
                // 在指定的执行器中执行任务
                if (task.executor) {
                    task.executor->Post(std::move(callback));
                } else {
                    // 如果没有指定执行器，直接在当前线程执行
                    callback();
                }
                
                // 如果是重复任务，重新调度
                if (task.repeat) {
                    lock.lock();
                    task.next_exec_time = std::chrono::steady_clock::now() + task.interval;
                    active_tasks_[task.id] = task;
                    active_task_count_.fetch_add(1);
                    tasks_.emplace(std::move(task));
                    lock.unlock();
                }
            } else {
                // 等待到下一个任务的执行时间
                condition_.wait_until(lock, next_task.next_exec_time);
            }
        } else {
            // 没有任务时等待
            condition_.wait(lock);
        }
    }
}

void Timer::addTask(TimerTask task) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // 添加到活跃任务映射
    active_tasks_[task.id] = task;
    active_task_count_.fetch_add(1);
    
    // 添加到优先级队列
    tasks_.emplace(std::move(task));
    
    lock.unlock();
    condition_.notify_one();
}

void Timer::removeTask(TaskId task_id) {
    // 从活跃任务映射中移除
    active_tasks_.erase(task_id);
    active_task_count_.fetch_sub(1);
    
    // 重建优先级队列，移除指定的任务
    std::priority_queue<TimerTask> new_tasks;
    while (!tasks_.empty()) {
        auto task = std::move(tasks_.top());
        tasks_.pop();
        
        if (task.id != task_id) {
            new_tasks.emplace(std::move(task));
        }
    }
    
    tasks_ = std::move(new_tasks);
}

Timer::TaskId Timer::generateTaskId() {
    return next_task_id_.fetch_add(1);
} 