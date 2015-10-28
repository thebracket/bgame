#include "output_backend.h"
#include "ansi_backend.h"
#include "ncurses_backend.h"
#include "sdl2_backend.h"

using std::make_unique;

namespace engine {
  
unique_ptr< output_backend > init_backend ( const backend_mode& mode )
{
    unique_ptr<output_backend> backend_driver;
    switch (mode) {
       case NCURSES : backend_driver = make_unique<ncurses_backend>(); break;
       case SDL2 : backend_driver = make_unique<sdl2_backend>(); break;
       case ANSI : backend_driver = make_unique<ansi_backend>(); break;
     }
     backend_driver->init();
     return backend_driver;
}
  
}