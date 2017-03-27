#include "region.hpp"
#include <rltk.hpp>
#include <string>
#include "../raws/raws.hpp"
#include "../raws/materials.hpp"
#include "../raws/plants.hpp"
#include "../systems/physics/fluid_system.hpp"
#include "../main/game_globals.hpp"

using namespace rltk;

void save_region(const region_t &region) {
	std::string region_filename = "world/region_" + std::to_string(region.region_x) + "_" + std::to_string(region.region_y) + ".dat";
	gzip_file deflate(region_filename, "wb");

	deflate.serialize(region.region_x);
	deflate.serialize(region.region_y);
	deflate.serialize(region.biome_idx);
	deflate.serialize(region.next_tree_id);

	deflate.serialize_vector_bool(region.revealed);
	deflate.serialize_vector_bool(region.visible);
	deflate.serialize_vector_bool(region.solid);
	deflate.serialize_vector_bool(region.opaque);
	deflate.serialize(region.tile_type);
	deflate.serialize(region.tile_material);
	deflate.serialize(region.tile_hit_points);
	deflate.serialize(region.building_id);
	deflate.serialize(region.tree_id);
	deflate.serialize(region.tile_vegetation_type);
	deflate.serialize(region.tile_vegetation_ticker);
    deflate.serialize(region.tile_vegetation_lifecycle);
	deflate.serialize(region.tile_flags);
	deflate.serialize(region.water_level);
	deflate.serialize_vector_bool(region.blood_stains);
	deflate.serialize(region.stockpile_id);
	deflate.serialize(region.bridge_id);
}

region_t load_region(const int region_x, const int region_y) {
	std::string region_filename = "world/region_" + std::to_string(region_x) + "_" + std::to_string(region_y) + ".dat";
	gzip_file inflate(region_filename, "rb");
	region_t region;

	inflate.deserialize(region.region_x);
	inflate.deserialize(region.region_y);
	inflate.deserialize(region.biome_idx);
	inflate.deserialize(region.next_tree_id);

	inflate.deserialize(region.revealed);
	inflate.deserialize(region.visible);
	inflate.deserialize(region.solid);
	inflate.deserialize(region.opaque);
	inflate.deserialize(region.tile_type);
	inflate.deserialize(region.tile_material);
	inflate.deserialize(region.tile_hit_points);
	inflate.deserialize(region.building_id);
	inflate.deserialize(region.tree_id);
	inflate.deserialize(region.tile_vegetation_type);
    inflate.deserialize(region.tile_vegetation_ticker);
    inflate.deserialize(region.tile_vegetation_lifecycle);
	inflate.deserialize(region.tile_flags);
	inflate.deserialize(region.water_level);
	inflate.deserialize(region.blood_stains);
	inflate.deserialize(region.stockpile_id);
    inflate.deserialize(region.bridge_id);

	std::cout << "Recalculating region paths\n";
	region.tile_recalc_all();
	return region;
}

void region_t::tile_recalc_all() {
	for (int z=0; z<REGION_DEPTH; ++z) {
        for (int y=0; y<REGION_HEIGHT; ++y) {
            for (int x=0; x<REGION_WIDTH; ++x) {
                tile_calculate(x,y,z);
            }
        }
    }
	for (int z=0; z<REGION_DEPTH; ++z) {
        for (int y=0; y<REGION_HEIGHT; ++y) {
            for (int x=0; x<REGION_WIDTH; ++x) {
                tile_pathing(x,y,z);
            }
        }
    }
}

void region_t::tile_calculate(const int &x, const int &y, const int &z) {
	const auto idx = mapidx(x,y,z);

	// Calculate render characteristics
	calc_render(idx);
	
	// Solidity and first-pass standability
	if (tile_type[idx] == tile_type::SEMI_MOLTEN_ROCK || tile_type[idx] == tile_type::SOLID 
			|| tile_type[idx] == tile_type::WALL || tile_type[idx] == tile_type::TREE_TRUNK || tile_type[idx] == tile_type::TREE_LEAF
			|| tile_type[idx] == tile_type::WINDOW || tile_type[idx] == tile_type::CLOSED_DOOR) {
		solid[idx] = true;
		if (tile_type[idx] == tile_type::WINDOW) {
			opaque[idx] = false;
		} else {
			opaque[idx] = true;
		}
		tile_flags[idx].reset(CAN_STAND_HERE);
	} else {
		solid[idx] = false;

		// Locations on which one can stand
		tile_flags[idx].set(CAN_STAND_HERE);
		if (tile_type[idx] == tile_type::OPEN_SPACE) tile_flags[idx].reset(CAN_STAND_HERE);

		if (z>0) {
			const auto idx_below = mapidx(x,y,z-1);

			// Can stand on the tile above walls, ramps and up stairs
			if (tile_type[idx] == tile_type::FLOOR) tile_flags[idx].set(CAN_STAND_HERE);
			if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::WALL) tile_flags[idx].set(CAN_STAND_HERE);
			if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::RAMP) tile_flags[idx].set(CAN_STAND_HERE);
			if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::STAIRS_UP) tile_flags[idx].set(CAN_STAND_HERE);
			if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::STAIRS_UPDOWN) tile_flags[idx].set(CAN_STAND_HERE);
			if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::SOLID) tile_flags[idx].set(CAN_STAND_HERE);
		}
	}

	// Is it underground?
	if (z == REGION_DEPTH-1) {
		above_ground[idx] = true;
	} else {
		bool underground = false;
		for (int z=REGION_DEPTH-1; z>0; --z) {
			if (tile_type[idx] == tile_type::SOLID) underground = true;
		}
		above_ground[idx] = !underground;
	}

	tile_pathing(x,y,z);
	water_stable[mapidx(x,y,z)] = false;
}

void region_t::tile_pathing(const int &x, const int &y, const int &z) {
	const auto idx = mapidx(x,y,z);

	// Start with a clean slate
	tile_flags[idx].reset(CAN_GO_NORTH);
	tile_flags[idx].reset(CAN_GO_SOUTH);
	tile_flags[idx].reset(CAN_GO_EAST);
	tile_flags[idx].reset(CAN_GO_WEST);
	tile_flags[idx].reset(CAN_GO_UP);
	tile_flags[idx].reset(CAN_GO_DOWN);

	if (solid[idx] || !tile_flags[idx].test(CAN_STAND_HERE)) {
		// If you can't go there, it doesn't have any exits.
	} else {
		if (x>0 && tile_flags[mapidx(x-1, y, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_WEST);
		if (x<REGION_WIDTH-1 && tile_flags[mapidx(x+1, y, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_EAST);
		if (y>0 && tile_flags[mapidx(x, y-1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_NORTH);
		if (y<REGION_HEIGHT-1 && tile_flags[mapidx(x, y+1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_SOUTH);

		if (z<REGION_DEPTH-1 && 
			(tile_type[idx] == tile_type::RAMP || tile_type[idx] == tile_type::STAIRS_UP || tile_type[idx] == tile_type::STAIRS_UPDOWN) && 
			tile_flags[mapidx(x,y,z+1)].test(CAN_STAND_HERE)) 
		{
			tile_flags[idx].set(CAN_GO_UP);
		}

		if (z>0 && (tile_type[idx] == tile_type::STAIRS_DOWN || tile_type[idx] == tile_type::STAIRS_UPDOWN ) && tile_flags[mapidx(x,y,z-1)].test(CAN_STAND_HERE)) {
			tile_flags[idx].set(CAN_GO_DOWN);
		}

		if (z>0 && tile_type[idx] == tile_type::OPEN_SPACE && tile_type[mapidx(x,y,z-1)]==tile_type::RAMP) {
			tile_flags[idx].set(CAN_GO_DOWN);
		}
	}
}

void region_t::calc_render(const int &idx) {
	uint16_t glyph;
	color_t fg;
	color_t bg = rltk::colors::BLACK;
    veg_cache[idx] = rltk::vchar{0, rltk::colors::BLACK, rltk::colors::BLACK};

    if (stockpile_id[idx] > 0) {
        // Stockpiles are always grey floors
        glyph = 256;
        render_cache[idx] = vchar{glyph, rltk::colors::DARK_GREY, rltk::colors::BLACK};
        return;
    }

	// Start with the basic tile_type; this hard-sets some glyphs.
	switch (tile_type[idx]) {
		case tile_type::SEMI_MOLTEN_ROCK : {
			glyph = 178;
			fg = rltk::colors::RED;
			bg = rltk::colors::YELLOW;
		} break;
		case tile_type::SOLID : {
			auto mat = get_material(tile_material[idx]);
            glyph = ascii_mode ? 219 : 257;
			if (idx < tile_material.size()) {
				fg = mat->fg;
				bg = mat->bg;
			} else {
				//std::cout << "Warning - material not found (" << idx << ")!\n";
				tile_material[idx] = 1;
				fg = mat->fg;
				bg = mat->bg;
			}
		} break;
		case tile_type::OPEN_SPACE : {
			glyph = ' ';
			fg = rltk::colors::BLACK;
		} break;
		case tile_type::WALL : {
            if (ascii_mode) {
                uint8_t wall_mask = 0;
                if (tile_type[idx-1] == tile_type::WALL) wall_mask += 1;
                if (tile_type[idx+1] == tile_type::WALL) wall_mask += 2;
                if (tile_type[idx-REGION_WIDTH] == tile_type::WALL) wall_mask += 4;
                if (tile_type[idx+REGION_WIDTH] == tile_type::WALL) wall_mask += 8;

                switch (wall_mask) {
                    case 0 : glyph = 79; break; // Isolated
                    case 1 : glyph = 181; break; // West only
                    case 2 : glyph = 198; break; // East only
                    case 3 : glyph = 205; break; // East and West
                    case 4 : glyph = 208; break; // North only
                    case 5 : glyph = 188; break; // North and west
                    case 6 : glyph = 200; break; // North and east
                    case 7 : glyph = 202; break; // North and east/west
                    case 8 : glyph = 210; break; // South only
                    case 9 : glyph = 187; break; // South and west
                    case 10 : glyph = 201; break; // South and east
                    case 11 : glyph = 203; break; // South east/west
                    case 12 : glyph = 186; break; // North and South
                    case 13 : glyph = 185; break; // North/South/West
                    case 14 : glyph = 204; break; // North/South/East
                    case 15 : glyph = 206; break; // All
                    default : {
                        std::cout << "WARNING: Wall calculator hit a case of " << +wall_mask << "\n";
                        glyph = 79;
                    }
                }
            } else {
                glyph = 257;
                fg = get_material(tile_material[idx])->fg;
                //bg = material_defs[tile_material[idx]].bg;
            }
		} break;
		case tile_type::WINDOW : {
			glyph = ascii_mode ? 176 : 257;
			fg = rltk::colors::CYAN;
		} break;
		case tile_type::RAMP : {
			glyph = ascii_mode ? 30 : 257;
			if (tile_material[idx] > 0) {
				fg = get_material(tile_material[idx])->fg;
			} else {
				fg = rltk::colors::GREY;
			} 

		} break;
		case tile_type::STAIRS_UP : {
			glyph = '<';
			fg = get_material(tile_material[idx])->fg;
		} break;
		case tile_type::STAIRS_DOWN : {
			glyph = '>';
			fg = get_material(tile_material[idx])->fg;
		} break;
		case tile_type::STAIRS_UPDOWN : {
			glyph = 'X';
			fg = get_material(tile_material[idx])->fg;
		} break;
		case tile_type::FLOOR: {

			if (get_material(tile_material[idx])->spawn_type == sand) {
				glyph = ascii_mode ? 126 : 258;
			} else if (get_material(tile_material[idx])->spawn_type == blight) {
				glyph = ascii_mode ? 247 : 265;
			} else if (tile_flags[idx].test(CONSTRUCTION)) {
				glyph = ascii_mode ? '.' : 256;
			} else {
				glyph = ascii_mode ? '`' : 257;
			}

			fg = get_material(tile_material[idx])->fg;

			if (tile_vegetation_type[idx]>0) {
				//std::cout << plant_defs[tile_vegetation_type[idx]].name << "\n";
				const plant_t plant = get_plant_def(tile_vegetation_type[idx]);
                const uint8_t lifecycle = tile_vegetation_lifecycle[idx];
                rltk::vchar plant_render;
                plant_render.glyph = ascii_mode ? '"' : plant.glyphs[lifecycle];
                if (ascii_mode) {
                    plant_render.foreground = colors::GREEN;
                } else {
                    plant_render.foreground = colors::WHITE;
                }
                veg_cache[idx] = plant_render;
			}
		} break;
		case tile_type::CLOSED_DOOR : {
            glyph = ascii_mode ? 197 : 329;
			fg = get_material(tile_material[idx])->fg;

		} break;
		case tile_type::TREE_TRUNK : {
			glyph = ascii_mode ? 180 : 257;
			fg = get_material(tile_material[idx])->fg;
			bg = rltk::colors::Black;
            veg_cache[idx] = ascii_mode ? vchar{180, rltk::colors::Brown, rltk::colors::BLACK} : vchar{259, rltk::colors::WHITE, rltk::colors::BLACK};
		} break;
		case tile_type::TREE_LEAF : {
			glyph = ascii_mode ? 177 : 278;
			fg = ascii_mode ? rltk::colors::GREEN : rltk::colors::White;
			bg = rltk::colors::Black;
		} break;
	}

	if (water_level[idx]>0) {
		switch (water_level[idx]) {
			case 1 : glyph = '1'; break;
			case 2 : glyph = '2'; break;
			case 3 : glyph = '3'; break;
			case 4 : glyph = '4'; break;
			case 5 : glyph = '5'; break;
			case 6 : glyph = '6'; break;
			case 7 : glyph = '7'; break;
			case 8 : glyph = '8'; break;
			case 9 : glyph = '9'; break;
			case 10 : glyph = '0'; break;
			default : glyph = '~';
		}
        fg = rltk::colors::Blue;
        bg = rltk::colors::DarkBlue;
	}

	// Apply it
	render_cache[idx] = vchar{glyph, fg, bg}; 
}