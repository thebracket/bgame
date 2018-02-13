#pragma once

#include <string>

struct name_t {
	std::string first_name;
	std::string last_name;

	name_t() = default;
	name_t(const std::string &fn, const std::string &ln) : first_name(fn), last_name(ln) {}
};
