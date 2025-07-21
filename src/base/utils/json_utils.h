#pragma once

#include <string>
#include <rapidjson/document.h>

inline bool isValidJson(const std::string &str) {
    rapidjson::Document doc;
    if (doc.Parse(str.c_str()).HasParseError()) {
        return false;
    }
    return true;
}
