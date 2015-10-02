#include "rng.h"
#include <random>
#include <iostream>

using std::mt19937;
using std::uniform_int_distribution;

namespace engine {

mt19937 random_number_generator;

void init_rng() {
    random_number_generator.seed(std::random_device()());
}

int roll_dice(int n, int d) {
    int total = 0;
    uniform_int_distribution<mt19937::result_type> dist_dice(1,d);
    for (int i=0; i<n; ++i) {
        total += dist_dice(random_number_generator);
    }
    return total;
}

}
