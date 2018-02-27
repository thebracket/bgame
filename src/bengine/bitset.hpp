#pragma once
#include <cstdint>

namespace bengine 
{
	template <typename T>
	struct bitset
	{
		T bits = 0;
		void set(const T n) noexcept { bits |= n; }
		void reset(const T n) noexcept { bits &= ~n; }
		bool test(const T n) const noexcept { return (bits & n) != 0; }
	};
}
