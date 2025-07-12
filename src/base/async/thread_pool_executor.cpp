#include "thread_pool_executor.h"

std::shared_ptr<ThreadPool> ThreadPoolExecutor::worker_ = nullptr;
std::shared_ptr<ThreadPool> ThreadPoolExecutor::io_ = nullptr;
std::shared_ptr<ThreadPool> ThreadPoolExecutor::compute_ = nullptr;