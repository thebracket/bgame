#include "tile3d.hpp"
#include "../components/component_types.h"
#include <iostream>
#include "tile_types.hpp"

using namespace serialization_generic;
using engine::vterm::color_t;
using engine::vterm::screen_character;

void tile_render_calculation(tile_t * tile)
{
	// Water is simple
	if (tile->water_level > 0)
	{
		tile->render_as.character = 126;
		if (tile->climate == tile_climate::ARCTIC
				or tile->climate == tile_climate::SUBARCTIC)
		{
			tile->render_as.foreground_color = color_t{ 255, 255, 255 };
			tile->render_as.background_color = color_t{ 128, 128, 128 };
		}
		else
		{
			tile->render_as.foreground_color = color_t{ 0, 255, 255 };
			tile->render_as.background_color = color_t{ 0, 0, 64 };
		}
		return;
	}

	// Set the ground type
	switch (tile->ground)
	{
	case tile_ground::GRAVEL:
		tile->render_as = screen_character{ 176, color_t{ 128, 128, 128 }, color_t{ 64, 64, 64 } };
		break;
	case tile_ground::IGNEOUS:
		tile->render_as = screen_character{ 178, color_t{ 128, 128, 128 }, color_t{ 64, 64, 64 } };
		break;
	case tile_ground::MUD:
		tile->render_as = screen_character{ 177, color_t{ 121, 76, 19 }, color_t{ 60, 35, 8 } };
		break;
	case tile_ground::PERMAFROST_BLUE:
		tile->render_as = screen_character{ 177, color_t{ 255, 255, 255 }, color_t{ 128, 128, 255 } };
		break;
	case tile_ground::PERMAFROST_CRACKED:
		tile->render_as = screen_character{ 35, color_t{ 255, 255, 255 }, color_t{ 128, 128, 255 } };
		break;
	case tile_ground::PERMAFROST_DIRTY:
		tile->render_as = screen_character{ 177, color_t{ 192, 192, 192 }, color_t{ 64, 64, 46 } };
		break;
	case tile_ground::PERMAFROST_WHITE:
		tile->render_as = screen_character{ 177, color_t{ 255, 255, 255 }, color_t{ 192, 192, 192 } };
		break;
	case tile_ground::RED_SAND:
		tile->render_as = screen_character{ 177, color_t{ 236, 197, 168 }, color_t{ 120, 95, 85 } };
		break;
	case tile_ground::RED_SAND_RIPPLE:
		tile->render_as = screen_character{ 247, color_t{ 236, 197, 168 }, color_t{ 120, 95, 85 } };
		break;
	case tile_ground::RED_SAND_ROUGH:
		tile->render_as = screen_character{ 178, color_t{ 236, 197, 168 }, color_t{ 120, 95, 85 } };
		break;
	case tile_ground::SEDIMENTARY:
		tile->render_as = screen_character{ 176, color_t{ 128, 128, 128 }, color_t{ 64, 64, 64 } };
		break;
	case tile_ground::WHITE_SAND:
		tile->render_as = screen_character{ 247, color_t{ 128, 128, 128 }, color_t{ 64, 64, 64 } };
		break;
	case tile_ground::YELLOW_SAND:
		tile->render_as = screen_character{ 176, color_t{ 239, 221, 111 }, color_t{ 116, 110, 56 } };
		break;
	case tile_ground::YELLOW_SAND_DIRTY:
		tile->render_as = screen_character{ 177, color_t{ 239, 221, 111 }, color_t{ 116, 110, 56 } };
		break;
	case tile_ground::YELLOW_SAND_RIPPLE:
		tile->render_as = screen_character{ 247, color_t{ 239, 221, 111 }, color_t{ 116, 110, 56 } };
		break;
	default:
		std::cout << "Unknown ground type: " << tile->ground << "\n";
	}

	// Set the covering
	switch (tile->covering)
	{
	case tile_covering::BARE:
	{ /* Do Nothing */
	}
		break;
	case tile_covering::CACTUS:
	{
		tile->render_as.character = 15;
		tile->render_as.foreground_color = color_t{ 1, 166, 17 };
	}
		break;
	case tile_covering::GORSE:
	{
		tile->render_as.character = 37;
		tile->render_as.foreground_color = color_t{ 1, 166, 17 };
	}
		break;
	case tile_covering::GRASS:
	{
		tile->render_as.character = 177;
		tile->render_as.foreground_color = color_t{ 1, 142, 14 };
	}
		break;
	case tile_covering::GRASS_SPARSE:
	{
		tile->render_as.character = 176;
		tile->render_as.foreground_color = color_t{ 1, 166, 17 };
	}
		break;
	case tile_covering::HEATHER:
	{
		tile->render_as.character = 126;
		tile->render_as.foreground_color = color_t{ 158, 162, 162 };
	}
		break;
	case tile_covering::LYCHEN:
	{
		tile->render_as.character = 246;
		tile->render_as.foreground_color = color_t{ 17, 150, 113 };
	}
		break;
	case tile_covering::MOSS:
	{
		tile->render_as.character = 178;
		tile->render_as.foreground_color = color_t{ 5, 92, 9 };
	}
		break;
	case tile_covering::PEBBLES:
	{
		tile->render_as.character = 176;
		tile->render_as.foreground_color = color_t{ 128, 128, 128 };
	}
		break;
	case tile_covering::REEDS:
	{
		tile->render_as.character = 159;
		tile->render_as.foreground_color = color_t{ 5, 92, 9 };
	}
		break;
	case tile_covering::SHRUB:
	{
		tile->render_as.character = 15;
		tile->render_as.foreground_color = color_t{ 58, 192, 58 };
	}
		break;
	case tile_covering::THISTLE:
	{
		tile->render_as.character = 37;
		tile->render_as.foreground_color = color_t{ 5, 92, 9 };
	}
		break;
	case tile_covering::WILDFLOWER:
	{
		tile->render_as.character = 96;
		tile->render_as.foreground_color = color_t{ 255, 255, 64 };
	}
		break;
	default:
		std::cout << "Unknown covering: " << tile->covering << "\n";
	}

	// Ramps: always render as an up ramp; added afterwards
	if (tile->base_tile_type == tile_type::RAMP)
	{
		tile->render_as.character = 30;
	}
}

void tile_t::save(Poco::DeflatingOutputStream& lbfile)
{
	save_primitive<uint8_t>(lbfile, water_level);
	save_primitive<unsigned char>(lbfile, base_tile_type);
	save_primitive<unsigned char>(lbfile, ground);
	save_primitive<unsigned char>(lbfile, climate);
	save_primitive<unsigned char>(lbfile, covering);
	string flags_export = flags.to_string();
	save_primitive<string>(lbfile, flags_export);
}

void tile_t::load(Poco::InflatingInputStream& lbfile)
{
	load_primitive<uint8_t>(lbfile, water_level);
	load_primitive<unsigned char>(lbfile, base_tile_type);
	load_primitive<unsigned char>(lbfile, ground);
	load_primitive<unsigned char>(lbfile, climate);
	load_primitive<unsigned char>(lbfile, covering);
	string flags_import;
	load_primitive<string>(lbfile, flags_import);
	flags = std::bitset<TILE_OPTIONS_COUNT>(flags_import);

	tile_render_calculation(this);
}
