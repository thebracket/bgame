#pragma once

struct item_stored_t {
	int stored_in;
	item_stored_t() = default;
	explicit item_stored_t(const int carrier) noexcept : stored_in(carrier) {}
};
