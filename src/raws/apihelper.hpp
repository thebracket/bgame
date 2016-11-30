#pragma once

#include <boost/optional.hpp>
#include <boost/container/flat_map.hpp>
#include <string>

template <typename T>
boost::optional<T> api_search(const boost::container::flat_map<std::string, T> &map, const std::string &key) {
    boost::optional<T> result;
    auto finder = map.find(key);
    if (finder != map.end()) {
        result = finder->second;
    }
    return result;
}