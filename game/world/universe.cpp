#include "universe.hpp"
#include "../tasks/help_wanted.h"

std::unique_ptr<universe_t> universe = std::make_unique<universe_t>();

void load_universe_state ( fstream& lbfile )
{
    lbfile.read ( reinterpret_cast<char *> ( &universe->globals.camera_handle ), sizeof ( universe->globals.camera_handle ) );
    lbfile.read ( reinterpret_cast<char *> ( &universe->globals.cordex_handle ), sizeof ( universe->globals.cordex_handle ) );    
    lbfile.read ( reinterpret_cast<char *> ( &universe->globals.stored_power ), sizeof ( universe->globals.stored_power ) );    
    lbfile.read ( reinterpret_cast<char *> ( &universe->globals.render_graphics ), sizeof ( universe->globals.render_graphics ) );  
    
    ai::load_help_wanted( lbfile );
}

void save_universe_state ( fstream& lbfile )
{
    lbfile.write ( reinterpret_cast<const char *> ( &universe->globals.camera_handle ), sizeof ( universe->globals.camera_handle ) );
    lbfile.write ( reinterpret_cast<const char *> ( &universe->globals.cordex_handle ), sizeof ( universe->globals.cordex_handle ) );
    lbfile.write ( reinterpret_cast<const char *> ( &universe->globals.stored_power ), sizeof ( universe->globals.stored_power ) );
    lbfile.write ( reinterpret_cast<const char *> ( &universe->globals.render_graphics ), sizeof ( universe->globals.render_graphics ) );
    
    ai::save_help_wanted( lbfile );
}
