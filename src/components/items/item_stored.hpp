#pragma once

struct item_stored_t {
	std::size_t stored_in;
	item_stored_t() = default;
	explicit item_stored_t(const std::size_t carrier) noexcept : stored_in(carrier) {}
};
