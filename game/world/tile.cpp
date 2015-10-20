#include "tile.h"
#include "../../engine/rng.h"

tile::tile()
{
     display = 0;
     base_tile_type = tile_type::FLAT;
     altitude = 0;
     level_band = 0;
     temperature = 1;
}

void tile::worldgen_tile_calc()
{
     // Determine the climate
     if ( temperature < 10 ) {
          climate = tile_climate::ARCTIC;
     } else if ( temperature < 32 ) {
          climate = tile_climate::COLD;
     } else if ( temperature < 45 ) {
          climate = tile_climate::COOL;
     } else if ( temperature < 75 ) {
          climate = tile_climate::TEMPERATE;
     } else if ( temperature < 90 ) {
          climate = tile_climate::WARM;
     } else if ( temperature < 110 ) {
          climate = tile_climate::HOT;
     }
     else {
          climate = tile_climate::VERY_HOT;
     }

     // Determine the ground material
     if ( base_tile_type == tile_type::WATER ) {
          ground = tile_ground::GRAVEL;
     } else if ( base_tile_type == tile_type::BEACH ) {
          int dice_roll = engine::roll_dice ( 1,8 );
          if ( dice_roll <= 3 ) {
               ground = tile_ground::WHITE_SAND;
          } else if ( dice_roll <= 6 ) {
               ground = tile_ground::YELLOW_SAND;
          } else {
               ground = tile_ground::GRAVEL;
          }
     } else {
          // Ramp or flat: lower ground is more sedimentary, high is more igneous
          int dice_roll = engine::roll_dice ( 1,10 ) + level_band;
          if ( dice_roll < 8 ) {
               // sedimentary!
               int dice2 = engine::roll_dice ( 1,6 );
               if ( dice2 == 1 ) {
                    ground = tile_ground::YELLOW_SAND;
               } else if ( dice2 == 2 or dice2 == 3 ) {
                    ground = tile_ground::GRAVEL;
               } else {
                    ground = tile_ground::SEDIMENTARY;
               }
          } else {
               // igneous
               int dice2 = engine::roll_dice ( 1,6 );
               if ( dice2 < 3 ) {
                    ground = tile_ground::GRAVEL;
               } else {
                    ground = tile_ground::IGNEOUS;
               }
          }
     }
     
     // Base covering
     if (base_tile_type != tile_type::WATER) {
	int chance_of_vegetation = 100;
	switch (climate) {
	  case tile_climate::ARCTIC : chance_of_vegetation=5; break;
	  case tile_climate::SUBARCTIC : chance_of_vegetation=15; break;
	  case tile_climate::COLD : chance_of_vegetation=25; break;
	  case tile_climate::COOL : chance_of_vegetation=75; break;
	  case tile_climate::TEMPERATE : chance_of_vegetation=90; break;
	  case tile_climate::WARM : chance_of_vegetation=75; break;
	  case tile_climate::HOT : chance_of_vegetation=25; break;
	  case tile_climate::VERY_HOT : chance_of_vegetation=5; break;
	}
	switch (ground) {
	  case tile_ground::GRAVEL : chance_of_vegetation += 5; break;
	  case tile_ground::WHITE_SAND : chance_of_vegetation = 0; break;
	  case tile_ground::YELLOW_SAND : chance_of_vegetation = 0; break;
	  case tile_ground::SEDIMENTARY : chance_of_vegetation += 5; break;
	  case tile_ground::IGNEOUS : chance_of_vegetation -= 5; break;
	}
	
	int vegetation_roll = engine::roll_dice(1,100);
	covering = tile_covering::BARE;
	if (vegetation_roll < chance_of_vegetation) {
	    // Something grows here!
	  if (climate == tile_climate::ARCTIC) {
	    int vtype = engine::roll_dice(1,5);
	    switch (vtype) {
	      case 1 : covering = tile_covering::MOSS; break;
	      case 2 : covering = tile_covering::LYCHEN; break;
	      case 3 : covering = tile_covering::HEATHER; break;
	      case 4 : covering = tile_covering::GRASS; break;
	      case 5 : covering = tile_covering::SHRUB; break;
	    }
	  }
	} else if (climate == tile_climate::COLD) {
	    int vtype = engine::roll_dice(1,7);
	    switch (vtype) {
	      case 1 : covering = tile_covering::MOSS; break;
	      case 2 : covering = tile_covering::LYCHEN; break;
	      case 3 : covering = tile_covering::HEATHER; break;
	      case 4 : covering = tile_covering::GRASS; break;
	      case 5 : covering = tile_covering::SHRUB; break;
	      case 6 : covering = tile_covering::THISTLE; break;
	      case 7 : covering = tile_covering::GORSE; break;
	    }
	} else if (climate == tile_climate::COOL) {
	    int vtype = engine::roll_dice(1,7);
	    switch (vtype) {
	      case 1 : covering = tile_covering::GRASS; break;
	      case 2 : covering = tile_covering::REEDS; break;
	      case 3 : covering = tile_covering::HEATHER; break;
	      case 4 : covering = tile_covering::GRASS; break;
	      case 5 : covering = tile_covering::SHRUB; break;
	      case 6 : covering = tile_covering::THISTLE; break;
	      case 7 : covering = tile_covering::GORSE; break;
	    }
	} else if (climate == tile_climate::TEMPERATE or climate == tile_climate::WARM) {
	    int vtype = engine::roll_dice(1,7);
	    switch (vtype) {
	      case 1 : covering = tile_covering::GRASS; break;
	      case 2 : covering = tile_covering::WILDFLOWER; break;
	      case 3 : covering = tile_covering::HEATHER; break;
	      case 4 : covering = tile_covering::GRASS; break;
	      case 5 : covering = tile_covering::SHRUB; break;
	      case 6 : covering = tile_covering::THISTLE; break;
	      case 7 : covering = tile_covering::GRASS; break;
	    }
	} else if (climate == tile_climate::HOT) {
	    int vtype = engine::roll_dice(1,3);
	    switch (vtype) {
	      case 1 : covering = tile_covering::GRASS; break;
	      case 2 : covering = tile_covering::GRASS; break;
	      case 3 : covering = tile_covering::WILDFLOWER; break;
	    }
	} else {
	  // VERY_HOT
	  covering = tile_covering::CACTUS;
	}
     } // end water exclusion
     
     // Finish up
     calculate_display_properties();
}

void tile::calculate_display_properties()
{
     // Temporary
     if ( base_tile_type == tile_type::WATER ) {
	  if (climate < tile_climate::COLD) {
	    foreground = color_t {255,255,255};
	    background = color_t {0,0,0};
	    display = 10;
	  } else {
	    foreground = color_t {0,0,255};
	    background = color_t {0,0,0};
	    display = '~';
	  }
     } else {
	  if (covering == tile_covering::BARE) {
	      switch (ground) {
		case tile_ground::GRAVEL : { foreground = color_t{128,128,128}; background=color_t{0,0,0}; display=177; } break;
		case tile_ground::IGNEOUS : { foreground = color_t{128,128,128}; background=color_t{0,0,0}; display=219; } break;
		case tile_ground::SEDIMENTARY : { foreground = color_t{128,128,128}; background=color_t{0,0,0}; display=178; } break;
		case tile_ground::WHITE_SAND : { foreground = color_t{200,200,200}; background=color_t{0,0,0}; display=176; } break;
		case tile_ground::YELLOW_SAND : { foreground = color_t{200,200,0}; background=color_t{0,0,0}; display=176; } break;
	      }
	  } else {
	      switch (covering) {
		case tile_covering::CACTUS : { foreground = color_t{0,200,0}; background=color_t{0,0,0}; display=5; } break;
		case tile_covering::GORSE : { foreground = color_t{240,255,0}; background=color_t{0,0,0}; display=21; } break;
		case tile_covering::GRASS : { foreground = color_t{0,200,0}; background=color_t{0,0,0}; display=176; } break;
		case tile_covering::HEATHER : { foreground = color_t{128,0,128}; background=color_t{0,0,0}; display=157; } break;
		case tile_covering::LYCHEN : { foreground = color_t{200,128,0}; background=color_t{0,0,0}; display=15; } break;
		case tile_covering::MOSS : { foreground = color_t{0,255,0}; background=color_t{0,0,0}; display=178; } break;
		case tile_covering::REEDS : { foreground = color_t{0,200,0}; background=color_t{0,0,0}; display=157; } break;
		case tile_covering::SHRUB : { foreground = color_t{0,255,0}; background=color_t{0,0,0}; display=37; } break;
		case tile_covering::THISTLE : { foreground = color_t{0,190,0}; background=color_t{0,0,0}; display=247; } break;
		case tile_covering::WILDFLOWER : { foreground = color_t{255,255,0}; background=color_t{0,0,0}; display='`'; } break;
	      }
	  }
     }
     
     if (base_tile_type == tile_type::RAMP) {
	display = 30;
     }
}

void tile::save ( fstream& lbfile )
{
     calculate_display_properties();
     lbfile.write ( reinterpret_cast<const char *> ( &base_tile_type ), sizeof ( base_tile_type ) );
     lbfile.write ( reinterpret_cast<const char *> ( &level_band ), sizeof ( level_band ) );
     lbfile.write ( reinterpret_cast<const char *> ( &climate ), sizeof ( climate ) );
     lbfile.write ( reinterpret_cast<const char *> ( &climate ), sizeof ( ground ) );
     lbfile.write ( reinterpret_cast<const char *> ( &covering ), sizeof ( covering ) );
}

void tile::load ( fstream &lbfile )
{
     lbfile.read ( reinterpret_cast<char *> ( &base_tile_type ), sizeof ( base_tile_type ) );
     lbfile.read ( reinterpret_cast<char *> ( &level_band ), sizeof ( level_band ) );
     lbfile.read ( reinterpret_cast<char *> ( &climate ), sizeof ( climate ) );
     lbfile.read ( reinterpret_cast<char *> ( &ground ), sizeof ( ground ) );
     lbfile.read ( reinterpret_cast<char *> ( &covering ), sizeof ( covering ) );
     calculate_display_properties();
}
