#pragma once

#include <unordered_map>
#include <string>

template <typename T>
const T * api_search(const std::unordered_map<std::string, T> &map, const std::string &key) noexcept {
    auto finder = map.find(key);
    if (finder != map.end()) {
        return &finder->second;
    }
    return nullptr;
}
