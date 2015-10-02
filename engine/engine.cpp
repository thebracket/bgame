#include "engine.h"
#include "output_backend.h"
#include "ncurses_backend.h"
#include <memory>
#include <thread>
#include <chrono>

using std::unique_ptr;
using std::make_unique;

namespace engine {

namespace detail {

unique_ptr<output_backend> backend_driver;

}

using namespace detail;

void init() {
    init_rng();
    init_virtual_terminal();

    // TODO: This should be determined by configuration at some point
    backend_driver = make_unique<ncurses_backend>();
    backend_driver->init();
    pair<int,int> size = backend_driver->get_console_size();
    vterm::resize(size.first, size.second);
}

void main_loop()
{
    vterm::clear_screen();
    vterm::print(0,0,"Hello World!");
    backend_driver->draw(vterm::get_virtual_screen());
    std::this_thread::sleep_for (std::chrono::seconds(10));
    backend_driver->stop();
}


}