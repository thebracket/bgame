#include "tile.h"

tile::tile()
{
     revealed = false;
     display = 0;
     visible = false;
     base_tile_type = flat;
}

void tile::calculate_display_properties()
{
     // Simple copy at this point
     foreground = base_tile_material.foreground;
     background = base_tile_material.background;
     display = base_tile_material.glyph;
}

void tile::save ( fstream& lbfile )
{
     base_tile_material.save ( lbfile );
     char buffer[3];
     if ( revealed ) {
          buffer[0] = 0;
     } else {
          buffer[0] = 1;
     }
     if ( visible ) {
          buffer[1] = 0;
     } else {
          buffer[1] = 1;
     }
     buffer[2] = (char)display;
     
     lbfile.write(buffer, 3);
     lbfile.write(reinterpret_cast<const char *>(&base_tile_type), sizeof(base_tile_type));
}

