#pragma once

#include "pcg_basic.h"

namespace bengine {
    class random_number_generator
    {
    public:
        random_number_generator();
        random_number_generator(const int seed);
        random_number_generator(const std::string seed);

        int roll_dice(const int &n, const int &d);
        int initial_seed;
    private:
        pcg32_random_t rng;
    };
}