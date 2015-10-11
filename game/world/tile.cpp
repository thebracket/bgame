#include "tile.h"

tile::tile()
{
    revealed = false;
    display = 0;
    visible = false;
    base_tile_type = flat;
    altitude = 0;
}

void tile::calculate_display_properties()
{
    // Temporary
    if (base_tile_type == water) {
        foreground = color_t {0,0,255};
        background = color_t {0,0,0};
        display = '~';
    } else if (base_tile_type == flat) {
        foreground = color_t {0,255,0};
        background = color_t {0,0,0};
        display = '.';
    } else if (base_tile_type == hill) {
        foreground = color_t {0,255,0};
        background = color_t {0,0,0};
        display = '^';
    } else {
        foreground = color_t {128,128,128};
        background = color_t {0,0,0};
        display = '^';
    }
}

void tile::save ( fstream& lbfile )
{
    calculate_display_properties();
    base_tile_material.save ( lbfile );
    char buffer[3];
    if ( revealed ) {
        buffer[0] = 1;
    } else {
        buffer[0] = 0;
    }
    if ( visible ) {
        buffer[1] = 0;
    } else {
        buffer[1] = 1;
    }
    buffer[2] = (char)display;

    lbfile.write(buffer, 3);
    lbfile.write(reinterpret_cast<const char *>(&base_tile_type), sizeof(base_tile_type));
    lbfile.write(reinterpret_cast<const char *>(&altitude), sizeof(altitude));
}

void tile::load(fstream &lbfile)
{
    base_tile_material.load(lbfile);
    char buffer[3];
    lbfile.read(buffer, 3);
    if (buffer[0] != 0) {
      revealed = true;
    } else {
      revealed = false;
    }
    if (buffer[1] != 0) {
      visible = true;
    } else {
      visible = false;
    }
    
    lbfile.read(reinterpret_cast<char *>(&base_tile_type), sizeof(base_tile_type));
    lbfile.read(reinterpret_cast<char *>(&altitude), sizeof(altitude));
    calculate_display_properties();
}
