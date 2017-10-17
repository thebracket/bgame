#include "random_number_generator.hpp"
#include "pcg_basic.h"
#include <ctime>

namespace bengine {

    random_number_generator::random_number_generator() {
        pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
    }

    random_number_generator::random_number_generator(const int seed) {
        pcg32_srandom_r(&rng, seed, (intptr_t)&rng);
    }

    random_number_generator::random_number_generator(const std::string seed) {
        std::hash<std::string> hash_func;
        initial_seed = static_cast<int>(hash_func(seed));
        pcg32_srandom_r(&rng, initial_seed, (intptr_t)&rng);
    }

    int random_number_generator::roll_dice(const int &n, const int &d) {
        int total = 0;
        for (int i = 0; i < n; ++i) {
            total += pcg32_boundedrand_r(&rng, d)+1;
        }
        return total;
    }
}