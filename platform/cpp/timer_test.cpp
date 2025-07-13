#include "../../src/base/async/timer.h"
#include "../../src/base/async/thread_pool_executor.h"
#include "../../src/base/logger/logger.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>

std::atomic<int> counter{0};

void testBasicFunctionality() {
    NC_LOG_INFO("\n=== 测试基本功能 ===");
    
    auto timer = std::make_unique<Timer>(ThreadPoolExecutor::Worker());
    
    // 测试延迟执行
    auto task1_id = timer->PostDelayed(
        std::chrono::milliseconds(100),
        []() { 
            NC_LOG_INFO("延迟任务执行成功！");
            counter++;
        }
    );
    
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    
    // 测试重复执行
    auto task2_id = timer->PostRepeating(
        std::chrono::milliseconds(50),
        std::chrono::milliseconds(100),
        []() { 
            NC_LOG_INFO("重复任务执行，计数: %d", ++counter);
            if (counter >= 5) {
                NC_LOG_INFO("重复任务达到5次，测试完成");
            }
        }
    );
    
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
    
    // 取消重复任务
    timer->Cancel(task2_id);
    NC_LOG_INFO("重复任务已取消");
}

void testThreadPoolIntegration() {
    NC_LOG_INFO("\n=== 测试线程池集成 ===");
    
    auto timer = std::make_unique<Timer>();
    
    // 在Worker线程池执行
    timer->PostDelayed(
        std::chrono::milliseconds(50),
        []() { 
            NC_LOG_INFO("任务在Worker线程池执行");
        },
        ThreadPoolExecutor::Worker()
    );
    
    // 在IO线程池执行
    timer->PostDelayed(
        std::chrono::milliseconds(100),
        []() { 
            NC_LOG_INFO("任务在IO线程池执行");
        },
        ThreadPoolExecutor::IO()
    );
    
    // 在Compute线程池执行
    timer->PostDelayed(
        std::chrono::milliseconds(150),
        []() { 
            NC_LOG_INFO("任务在Compute线程池执行");
        },
        ThreadPoolExecutor::Compute()
    );
    
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void testTaskCancellation() {
    NC_LOG_INFO("\n=== 测试任务取消 ===");
    
    auto timer = std::make_unique<Timer>();
    
    // 添加一个延迟任务
    auto task_id = timer->PostDelayed(
        std::chrono::milliseconds(500),
        []() { 
            NC_LOG_INFO("这个任务不应该执行！");
        }
    );
    
    NC_LOG_INFO("任务已添加，ID: %lu", task_id);
    NC_LOG_INFO("当前活跃任务数量: %zu", timer->GetActiveTaskCount());
    
    // 立即取消任务
    if (timer->Cancel(task_id)) {
        NC_LOG_INFO("任务已成功取消");
    } else {
        NC_LOG_INFO("任务取消失败");
    }
    
    NC_LOG_INFO("取消后活跃任务数量: %zu", timer->GetActiveTaskCount());
    
    std::this_thread::sleep_for(std::chrono::milliseconds(600));
}

void testCancelAll() {
    NC_LOG_INFO("\n=== 测试取消所有任务 ===");
    
    auto timer = std::make_unique<Timer>();
    
    // 添加多个任务
    timer->PostDelayed(std::chrono::milliseconds(200), []() { NC_LOG_INFO("任务1"); });
    timer->PostDelayed(std::chrono::milliseconds(300), []() { NC_LOG_INFO("任务2"); });
    timer->PostDelayed(std::chrono::milliseconds(400), []() { NC_LOG_INFO("任务3"); });
    
    NC_LOG_INFO("添加了3个任务，当前活跃任务数量: %zu", timer->GetActiveTaskCount());
    
    // 取消所有任务
    timer->CancelAll();
    NC_LOG_INFO("取消所有任务后，活跃任务数量: %zu", timer->GetActiveTaskCount());
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

int main() {
    nc_logger::init(plog::debug, "timer_test.log");
    nc_logger::set_level(plog::debug);
    
    NC_LOG_INFO("Timer类测试开始...");
    
    testBasicFunctionality();
    testThreadPoolIntegration();
    testTaskCancellation();
    testCancelAll();
    
    NC_LOG_INFO("\nTimer类测试完成！");
    return 0;
} 