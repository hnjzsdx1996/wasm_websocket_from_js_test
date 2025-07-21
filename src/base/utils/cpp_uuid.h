#pragma once

#include <random>
#include <sstream>
#include <iomanip>
#include <string>

inline std::string generate_uuid_v4() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

    uint32_t data[4];
    for (int i = 0; i < 4; ++i) {
        data[i] = dis(gen);
    }

    // 设置 version 和 variant
    uint8_t* bytes = reinterpret_cast<uint8_t*>(data);
    bytes[6] = (bytes[6] & 0x0F) | 0x40; // version 4
    bytes[8] = (bytes[8] & 0x3F) | 0x80; // variant

    std::ostringstream oss;
    oss << std::hex << std::setfill('0')
        << std::setw(2) << (int)bytes[0]
        << std::setw(2) << (int)bytes[1]
        << std::setw(2) << (int)bytes[2]
        << std::setw(2) << (int)bytes[3] << "-"
        << std::setw(2) << (int)bytes[4]
        << std::setw(2) << (int)bytes[5] << "-"
        << std::setw(2) << (int)bytes[6]
        << std::setw(2) << (int)bytes[7] << "-"
        << std::setw(2) << (int)bytes[8]
        << std::setw(2) << (int)bytes[9] << "-"
        << std::setw(2) << (int)bytes[10]
        << std::setw(2) << (int)bytes[11]
        << std::setw(2) << (int)bytes[12]
        << std::setw(2) << (int)bytes[13]
        << std::setw(2) << (int)bytes[14]
        << std::setw(2) << (int)bytes[15];

    return oss.str();
}