#include "BusinessManager.h"

#include <utility>

#include "logger.h"


BusinessManager::BusinessManager(std::weak_ptr<TopicManager> topic_mgr) : topic_mgr_(std::move(topic_mgr)) {
    NC_LOG_INFO("[BusinessManager] ctor");
}

void BusinessManager::CancelObserve(int64_t listen_id) {
    auto strong_topic_mgr = topic_mgr_.lock();
    if (strong_topic_mgr == nullptr) {
        NC_LOG_INFO("[BusinessManager] CancelObserve: %d, failed", listen_id);
        return;
    }
    NC_LOG_INFO("[BusinessManager] CancelObserve: %d", listen_id);
    strong_topic_mgr->CancelObserve(listen_id);
}
