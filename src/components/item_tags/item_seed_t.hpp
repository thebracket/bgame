#pragma once

#include <string>

struct item_seed_t {

	item_seed_t() = default;
	explicit item_seed_t(const std::string &target) noexcept : grows_into(target) {}
	std::string grows_into = "";
};

