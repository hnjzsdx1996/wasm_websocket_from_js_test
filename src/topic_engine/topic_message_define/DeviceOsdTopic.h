#pragma once

#include "../TopicMessageWrapper.h"

#include "AIGCJson.hpp"

class DeviceOsdMessage {
public:
    int velocity;

    AIGC_JSON_HELPER(velocity);
};

class DeviceOsdTopic: public TopicMessageWrapper {
public:

    DeviceOsdMessage msg;
};