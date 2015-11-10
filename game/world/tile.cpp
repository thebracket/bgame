#include "tile.h"
#include <sstream>
#include "../game.h"

using std::stringstream;

tile::tile()
{
     display = 0;
     base_tile_type = tile_type::FLAT;
     altitude = 0;
     level_band = 0;
     temperature = 1;
}

void tile::worldgen_determine_climate()
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
	  if (base_tile_type != tile_type::RAMP and base_tile_type != tile_type::WATER) {
	    int roll = game_engine->rng.roll_dice(1,10);
	    if (roll > 5) base_tile_type = tile_type::BEACH;
	  }
     }
     else {
          climate = tile_climate::VERY_HOT;
	  if (base_tile_type != tile_type::RAMP and base_tile_type != tile_type::WATER) {
	    int roll = game_engine->rng.roll_dice(1,10);
	    if (roll > 2) base_tile_type = tile_type::BEACH;
	  }
     }
}

void tile::worldgen_determine_base_ground()
{
      // Determine the ground material
     if ( base_tile_type == tile_type::WATER ) {
          ground = tile_ground::GRAVEL;
     } else if ( base_tile_type == tile_type::BEACH ) {
          int dice_roll = game_engine->rng.roll_dice ( 1,10 );
          if ( dice_roll <= 9 ) {
               ground = tile_ground::WHITE_SAND;
          } else if ( dice_roll <= 6 ) {
               ground = tile_ground::YELLOW_SAND;
	  } else if (dice_roll <=3) {
	      ground = tile_ground::RED_SAND;
          } else {
               ground = tile_ground::GRAVEL;
          }
     } else {
          // Ramp or flat: lower ground is more sedimentary, high is more igneous
          int dice_roll = game_engine->rng.roll_dice ( 1,10 ) + level_band;
          if ( dice_roll < 8 ) {
               // sedimentary!
               int dice2 = game_engine->rng.roll_dice ( 1,6 );
               if ( dice2 == 1 ) {
                    ground = tile_ground::YELLOW_SAND;
               } else if ( dice2 == 2 or dice2 == 3 ) {
                    ground = tile_ground::GRAVEL;
               } else {
                    ground = tile_ground::SEDIMENTARY;
               }
          } else {
               // igneous
               int dice2 = game_engine->rng.roll_dice ( 1,6 );
               if ( dice2 < 3 ) {
                    ground = tile_ground::GRAVEL;
               } else {
                    ground = tile_ground::IGNEOUS;
               }
          }
     }
     covering = tile_covering::BARE;
}

void tile::worldgen_arctic_covering()
{
    constexpr int chance_of_vegetation = 5;
    const int vegetation_roll = game_engine->rng.roll_dice(1,100);
    if (vegetation_roll < chance_of_vegetation) {
	int table_roll = game_engine->rng.roll_dice(1,3);
	switch (table_roll) {
	  case 1 : covering = tile_covering::MOSS; break;
	  case 2 : covering = tile_covering::LYCHEN; break;
	  case 3 : covering = tile_covering::REEDS; break;
	}
    }
}

void tile::worldgen_subarctic_covering()
{
    constexpr int chance_of_vegetation = 10;
    const int vegetation_roll = game_engine->rng.roll_dice(1,100);
    if (vegetation_roll < chance_of_vegetation) {
	int table_roll = game_engine->rng.roll_dice(1,5);
	switch (table_roll) {
	  case 1 : covering = tile_covering::MOSS; break;
	  case 2 : covering = tile_covering::LYCHEN; break;
	  case 3 : covering = tile_covering::REEDS; break;
	  case 4 : covering = tile_covering::GRASS; break;
	  case 5 : covering = tile_covering::SHRUB; break;
	}
    }
}

void tile::worldgen_cold_covering()
{
    constexpr int chance_of_vegetation = 60;
    const int vegetation_roll = game_engine->rng.roll_dice(1,100);
    if (vegetation_roll < chance_of_vegetation) {
	int table_roll = game_engine->rng.roll_dice(1,8);
	switch (table_roll) {
	  case 1 : covering = tile_covering::MOSS; break;
	  case 2 : covering = tile_covering::LYCHEN; break;
	  case 3 : covering = tile_covering::REEDS; break;
	  case 4 : covering = tile_covering::GRASS; break;
	  case 5 : covering = tile_covering::GRASS; break;
	  case 6 : covering = tile_covering::HEATHER; break;
	  case 7 : covering = tile_covering::GRASS; break;
	  case 8 : covering = tile_covering::GORSE; break;
	}
    }
}

void tile::worldgen_cool_covering()
{
    constexpr int chance_of_vegetation = 70;
    const int vegetation_roll = game_engine->rng.roll_dice(1,100);
    if (vegetation_roll < chance_of_vegetation) {
	int table_roll = game_engine->rng.roll_dice(1,9);
	switch (table_roll) {
	  case 1 : covering = tile_covering::MOSS; break;
	  case 2 : covering = tile_covering::GORSE; break;
	  case 3 : covering = tile_covering::REEDS; break;
	  case 4 : covering = tile_covering::GRASS; break;
	  case 5 : covering = tile_covering::GRASS; break;
	  case 6 : covering = tile_covering::HEATHER; break;
	  case 7 : covering = tile_covering::GRASS; break;
	  case 8 : covering = tile_covering::SHRUB; break;
	  case 9 : covering = tile_covering::WILDFLOWER; break;
	}
    }
}

void tile::worldgen_temperate_covering()
{
    constexpr int chance_of_vegetation = 95;
    const int vegetation_roll = game_engine->rng.roll_dice(1,100);
    if (vegetation_roll < chance_of_vegetation) {
	int table_roll = game_engine->rng.roll_dice(1,9);
	switch (table_roll) {
	  case 1 : covering = tile_covering::MOSS; break;
	  case 2 : covering = tile_covering::GRASS; break;
	  case 3 : covering = tile_covering::REEDS; break;
	  case 4 : covering = tile_covering::GRASS; break;
	  case 5 : covering = tile_covering::GRASS; break;
	  case 6 : covering = tile_covering::GRASS; break;
	  case 7 : covering = tile_covering::GRASS; break;
	  case 8 : covering = tile_covering::SHRUB; break;
	  case 9 : covering = tile_covering::WILDFLOWER; break;
	}
    }
}

void tile::worldgen_warm_covering()
{
    constexpr int chance_of_vegetation = 98;
    const int vegetation_roll = game_engine->rng.roll_dice(1,100);
    if (vegetation_roll < chance_of_vegetation) {
	int table_roll = game_engine->rng.roll_dice(1,9);
	switch (table_roll) {
	  case 1 : covering = tile_covering::GRASS; break;
	  case 2 : covering = tile_covering::GRASS; break;
	  case 3 : covering = tile_covering::GRASS; break;
	  case 4 : covering = tile_covering::GRASS; break;
	  case 5 : covering = tile_covering::GRASS; break;
	  case 6 : covering = tile_covering::GRASS; break;
	  case 7 : covering = tile_covering::GRASS; break;
	  case 8 : covering = tile_covering::SHRUB; break;
	  case 9 : covering = tile_covering::CACTUS; break;
	}
    }
}

void tile::worldgen_hot_covering()
{
    constexpr int chance_of_vegetation = 30;
    const int vegetation_roll = game_engine->rng.roll_dice(1,100);
    if (vegetation_roll < chance_of_vegetation) {
	int table_roll = game_engine->rng.roll_dice(1,3);
	switch (table_roll) {
	  case 1 : covering = tile_covering::GRASS; break;
	  case 2 : covering = tile_covering::GRASS; break;
	  case 3 : covering = tile_covering::CACTUS; break;
	}
    }
}

void tile::worldgen_very_hot_covering()
{
    constexpr int chance_of_vegetation = 10;
    const int vegetation_roll = game_engine->rng.roll_dice(1,100);
    if (vegetation_roll < chance_of_vegetation) {
	int table_roll = game_engine->rng.roll_dice(1,3);
	switch (table_roll) {
	  case 1 : covering = tile_covering::GRASS; break;
	  case 2 : covering = tile_covering::CACTUS; break;
	  case 3 : covering = tile_covering::CACTUS; break;
	}
    }
}

void tile::worldgen_tile_calc()
{
     worldgen_determine_climate();
     worldgen_determine_base_ground();
     if (base_tile_type != tile_type::BEACH and base_tile_type != tile_type::WATER and base_tile_type != tile_type::WATER) {
	switch (climate) {
	  case tile_climate::ARCTIC : worldgen_arctic_covering(); break;
	  case tile_climate::SUBARCTIC : worldgen_subarctic_covering(); break;
	  case tile_climate::COLD : worldgen_cold_covering(); break;
	  case tile_climate::COOL : worldgen_cool_covering(); break;
	  case tile_climate::TEMPERATE : worldgen_temperate_covering(); break;
	  case tile_climate::WARM : worldgen_warm_covering(); break;
	  case tile_climate::HOT : worldgen_hot_covering(); break;
	  case tile_climate::VERY_HOT : worldgen_very_hot_covering(); break;
	}    
     }
     
     // Finish up
     calculate_display_properties();
}

void tile::calculate_display_properties()
{
     // Temporary
     if ( base_tile_type == tile_type::WATER ) {
	  if (climate < tile_climate::COOL) {
	    foreground = color_t {255,255,255};
	    background = color_t {0,0,0};
	    display = '~';
	  } else {
	    foreground = color_t {64,64,255};
	    background = color_t {0,0,0};
	    display = '~';
	  }
     } else {
	  if (covering == tile_covering::BARE) {
	      switch (ground) {
		case tile_ground::GRAVEL : { foreground = color_t{128,128,128}; background=color_t{0,0,0}; display=176; } break;
		case tile_ground::IGNEOUS : { foreground = color_t{96,96,96}; background=color_t{0,0,0}; display=178; } break;
		case tile_ground::SEDIMENTARY : { foreground = color_t{128,128,128}; background=color_t{0,0,0}; display=178; } break;
		case tile_ground::WHITE_SAND : { foreground = color_t{200,200,200}; background=color_t{0,0,0}; display=177; } break;
		case tile_ground::YELLOW_SAND : { foreground = color_t{200,200,0}; background=color_t{0,0,0}; display=177; } break;
		case tile_ground::RED_SAND : { foreground = color_t{200,128,64}; background=color_t{0,0,0}; display=177; } break;
	      }
	  } else {
	      switch (covering) {
		case tile_covering::CACTUS : { foreground = color_t{0,200,0}; background=color_t{0,0,0}; display=5; } break;
		case tile_covering::GORSE : { foreground = color_t{240,255,0}; background=color_t{0,0,0}; display=178; } break;
		case tile_covering::GRASS : { foreground = color_t{0,200,0}; background=color_t{0,0,0}; display=176; } break;
		case tile_covering::HEATHER : { foreground = color_t{128,0,128}; background=color_t{0,0,0}; display='%'; } break;
		case tile_covering::LYCHEN : { foreground = color_t{0,128,0}; background=color_t{0,0,0}; display='%'; } break;
		case tile_covering::MOSS : { foreground = color_t{0,255,0}; background=color_t{0,0,0}; display='.'; } break;
		case tile_covering::REEDS : { foreground = color_t{0,200,0}; background=color_t{0,0,0}; display=';'; } break;
		case tile_covering::SHRUB : { foreground = color_t{0,255,0}; background=color_t{0,0,0}; display=15; } break;
		case tile_covering::THISTLE : { foreground = color_t{0,190,0}; background=color_t{0,0,0}; display='"'; } break;
		case tile_covering::WILDFLOWER : { foreground = color_t{255,255,0}; background=color_t{0,0,0}; display='`'; } break;
	      }
	  }
     }
     
     if (base_tile_type == tile_type::RAMP) {
	display = 30;
     }
}

string tile::get_climate()
{
    switch (climate) {
      case tile_climate::ARCTIC : return "Arctic";
      case tile_climate::SUBARCTIC : return "Sub-Arctic";
      case tile_climate::COLD : return "Cold";
      case tile_climate::COOL : return "Cool";
      case tile_climate::TEMPERATE : return "Temperate";
      case tile_climate::WARM : return "Warm";
      case tile_climate::HOT : return "Hot";
      case tile_climate::VERY_HOT : return "Scorching";
      default : return "Unknown";
    }
}

string tile::get_description()
{
    stringstream ss;
    
    switch (base_tile_type) {
      case tile_type::WATER : ss << "Water"; break;
      case tile_type::FLAT : ss << "Flat"; break;
      case tile_type::RAMP : ss << "Upwards Slope"; break;
      case tile_type::BEACH : ss << "Clear"; break;
    }
    
    if (base_tile_type != tile_type::WATER) {
      ss << " ";    
      switch (ground) {
	case tile_ground::GRAVEL : ss << "Gravel"; break;
	case tile_ground::IGNEOUS : ss << "Granite"; break;
	case tile_ground::SEDIMENTARY : ss << "Limestone"; break;
	case tile_ground::WHITE_SAND : ss << "White Sand"; break;
	case tile_ground::YELLOW_SAND : ss << "Yellow Sand"; break;
      }
      
      ss << " ";
      switch (covering) {
	case tile_covering::BARE : ss << "Bare"; break;
	case tile_covering::CACTUS : ss << "Cactus"; break;
	case tile_covering::GORSE : ss << "Gorse"; break;
	case tile_covering::GRASS : ss << "Grass"; break;
	case tile_covering::HEATHER : ss << "Heather"; break;
	case tile_covering::LYCHEN : ss << "Lychen"; break;
	case tile_covering::MOSS : ss << "Moss"; break;
	case tile_covering::REEDS : ss << "Reeds"; break;
	case tile_covering::SHRUB : ss << "Shrub"; break;
	case tile_covering::THISTLE : ss << "Thistle"; break;
	case tile_covering::WILDFLOWER : ss << "Wild flower"; break;
      }
    }
    
    return ss.str();
}

void tile::save ( fstream& lbfile )
{
     calculate_display_properties();
     lbfile.write ( reinterpret_cast<const char *> ( &base_tile_type ), sizeof ( base_tile_type ) );
     lbfile.write ( reinterpret_cast<const char *> ( &level_band ), sizeof ( level_band ) );
     lbfile.write ( reinterpret_cast<const char *> ( &climate ), sizeof ( climate ) );
     lbfile.write ( reinterpret_cast<const char *> ( &ground ), sizeof ( ground ) );
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
