#pragma once

struct bitset8 {
	uint8_t bits = 0;
	void set(const uint8_t n) { bits|= n; }
	void reset(const uint8_t n) { bits &= ~n; }
	bool test(const uint8_t n) { return (bits & n)!=0; }
};
