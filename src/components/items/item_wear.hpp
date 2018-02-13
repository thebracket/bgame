#pragma once

#include <string>

struct item_wear_t {

	item_wear_t() = default;
	explicit item_wear_t(const uint8_t w) noexcept : wear(w) {}
	uint8_t wear = 100;

	std::string get_wear_text() const {
		if (wear < 10) return std::string("falling apart");
		if (wear < 30) return std::string("tattered");
		if (wear < 50) return std::string("worn");
		if (wear < 75) return std::string("lightly used");
		return std::string("pristine");
	}
};

