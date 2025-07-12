#include "../../src/base/async/thread_pool_executor.h"
#include "../../src/base/logger/logger.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>

// 全局计数器，用于验证任务执行
std::atomic<int> task_counter{0};
std::atomic<int> delayed_task_counter{0};

// 测试任务函数
void test_task(int task_id) {
    auto thread_id = std::this_thread::get_id();
    NC_LOG_INFO("[任务 %d] 在线程 %zu 执行", task_id, thread_id);
    task_counter.fetch_add(1, std::memory_order_relaxed);
    
    // 模拟一些工作
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

// 延迟任务函数
void test_delayed_task(int task_id) {
    auto thread_id = std::this_thread::get_id();
    NC_LOG_INFO("[延迟任务 %d] 在线程 %zu 执行", task_id, thread_id);
    delayed_task_counter.fetch_add(1, std::memory_order_relaxed);
}

// 测试 Worker 线程池
void test_worker_pool() {
    NC_LOG_INFO("\n=== 测试 Worker 线程池 ===");
    
    auto worker_pool = ThreadPoolExecutor::Worker();
    
    // 提交多个任务
    for (int i = 1; i <= 5; i++) {
        worker_pool->Post([i]() { test_task(i); });
        NC_LOG_INFO("提交任务 %d 到 Worker 池", i);
    }
    
    // 等待任务完成
    std::this_thread::sleep_for(std::chrono::seconds(1));
    NC_LOG_INFO("Worker 池任务完成数量: %d", task_counter.load());
}

// 测试 IO 线程池
void test_io_pool() {
    NC_LOG_INFO("\n=== 测试 IO 线程池 ===");
    
    auto io_pool = ThreadPoolExecutor::IO();
    
    // 提交多个任务
    for (int i = 1; i <= 3; i++) {
        io_pool->Post([i]() { test_task(i + 10); });
        NC_LOG_INFO("提交任务 %d 到 IO 池", i + 10);
    }
    
    // 等待任务完成
    std::this_thread::sleep_for(std::chrono::seconds(1));
    NC_LOG_INFO("IO 池任务完成数量: %d", task_counter.load());
}

// 测试 Compute 线程池
void test_compute_pool() {
    NC_LOG_INFO("\n=== 测试 Compute 线程池 ===");
    
    auto compute_pool = ThreadPoolExecutor::Compute();
    
    // 提交多个任务
    for (int i = 1; i <= 4; i++) {
        compute_pool->Post([i]() { test_task(i + 20); });
        NC_LOG_INFO("提交任务 %d 到 Compute 池", i + 20);
    }
    
    // 等待任务完成
    std::this_thread::sleep_for(std::chrono::seconds(1));
    NC_LOG_INFO("Compute 池任务完成数量: %d", task_counter.load());
}

// 测试延迟任务
void test_delayed_tasks() {
    NC_LOG_INFO("\n=== 测试延迟任务 ===");
    
    auto worker_pool = ThreadPoolExecutor::Worker();
    
    // 提交延迟任务
    for (int i = 1; i <= 3; i++) {
        auto delay = std::chrono::milliseconds(i * 500); // 500ms, 1000ms, 1500ms
        worker_pool->PostDelayed([i]() { test_delayed_task(i); }, delay);
        NC_LOG_INFO("提交延迟任务 %d，延迟 %dms", i, i * 500);
    }
    
    // 等待所有延迟任务完成
    std::this_thread::sleep_for(std::chrono::seconds(3));
    NC_LOG_INFO("延迟任务完成数量: %d", delayed_task_counter.load());
}

// 测试并发任务
void test_concurrent_tasks() {
    NC_LOG_INFO("\n=== 测试并发任务 ===");
    
    auto worker_pool = ThreadPoolExecutor::Worker();
    std::vector<std::thread> threads;
    
    // 创建多个线程同时提交任务
    for (int t = 0; t < 3; t++) {
        threads.emplace_back([worker_pool, t]() {
            for (int i = 1; i <= 3; i++) {
                int task_id = t * 100 + i;
                worker_pool->Post([task_id]() { test_task(task_id); });
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        });
    }
    
    // 等待所有线程完成提交
    for (auto& thread : threads) {
        thread.join();
    }
    
    // 等待任务完成
    std::this_thread::sleep_for(std::chrono::seconds(2));
    NC_LOG_INFO("并发任务完成数量: %d", task_counter.load());
}

int main() {
    NC_LOG_INFO("=== ThreadPoolExecutor 测试程序 ===");
    
    // 初始化日志
    nc_logger::init(plog::debug, "thread_pool_test.log");
    nc_logger::set_level(plog::debug);
    
    NC_LOG_INFO("开始测试 ThreadPoolExecutor...");
    
    // 测试各种线程池
    test_worker_pool();
    test_io_pool();
    test_compute_pool();
    
    // 测试延迟任务
    test_delayed_tasks();
    
    // 测试并发任务
    test_concurrent_tasks();
    
    NC_LOG_INFO("\n=== 测试总结 ===");
    NC_LOG_INFO("总任务完成数量: %d", task_counter.load());
    NC_LOG_INFO("延迟任务完成数量: %d", delayed_task_counter.load());
    NC_LOG_INFO("所有测试完成！");
    
    return 0;
} 