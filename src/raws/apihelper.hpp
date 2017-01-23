#pragma once

#include <boost/optional.hpp>
#include <unordered_map>
#include <string>

template <typename T>
boost::optional<T> api_search(const std::unordered_map<std::string, T> &map, const std::string &key) {
    boost::optional<T> result;
    auto finder = map.find(key);
    if (finder != map.end()) {
        result = finder->second;
    }
    return result;
}