#include <iostream>
#include "bengine/main_window.hpp"

using namespace bengine;

void tick(const double &duration_ms) {

}

int main() {
    init();
    main_func = tick;
    run();

    return 0;
}