#pragma once

struct bitset8 {
	uint8_t bits = 0;
	constexpr void set(const uint8_t n) { bits |= n; }
	constexpr void reset(const uint8_t n) { bits &= ~n; }
	constexpr bool test(const uint8_t n) { return bits & n; }
};
