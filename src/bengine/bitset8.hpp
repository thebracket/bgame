#pragma once

namespace bengine 
{
	struct bitset8 {
		uint8_t bits = 0;
		void set(const uint8_t n) noexcept { bits|= n; }
		void reset(const uint8_t n) noexcept { bits &= ~n; }
		bool test(const uint8_t n) const noexcept { return (bits & n)!=0; }
	};
}