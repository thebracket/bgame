#include "tile.h"

tile::tile()
{
    display = 0;
    base_tile_type = flat;
    altitude = 0;
    flags = 0;
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
    lbfile.write(reinterpret_cast<const char *>(&flags), 1);
    lbfile.write(reinterpret_cast<const char *>(&base_tile_type), sizeof(base_tile_type));
    lbfile.write(reinterpret_cast<const char *>(&altitude), sizeof(altitude));
}

void tile::load(fstream &lbfile)
{
    base_tile_material.load(lbfile);
    lbfile.read(reinterpret_cast<char *>(&flags),1);
    lbfile.read(reinterpret_cast<char *>(&base_tile_type), sizeof(base_tile_type));
    lbfile.read(reinterpret_cast<char *>(&altitude), sizeof(altitude));
    calculate_display_properties();
}
