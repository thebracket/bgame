#include <iostream>
#include "engine/rng.h"

using std::cout;

int main() {
    engine::init_rng();
    for (int i=0; i<6; ++i) {
        cout << "3d6 roll: " << engine::roll_dice(3,6) << "\n";
    }
}

