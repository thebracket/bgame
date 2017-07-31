#include <iostream>
#include "bengine/main_window.hpp"
#include "main_loops/splash_screen.hpp"

using namespace bengine;

int main() {
    init();
    splash_screen::init();
    main_func = splash_screen::tick;
    run();

    return 0;
}