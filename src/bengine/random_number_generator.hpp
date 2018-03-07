#pragma once

#include "pcg_basic.h"
#include <string>

namespace bengine {
    class random_number_generator
    {
    public:
        random_number_generator();
        explicit random_number_generator(const int seed);
        explicit random_number_generator(const std::string seed);

        int roll_dice(const int &n, const int &d);
		int roll_dice(const int &n, const size_t &d)
		{
			return roll_dice(n, static_cast<int>(d));
		}
        int initial_seed;
    private:
        pcg32_random_t rng;
    };
}