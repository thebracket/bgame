#include "region.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/plants.hpp"
//#include "../../systems/physics/fluid_system.hpp"
//#include "../../main/game_camera.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../raws/defs/plant_t.hpp"
#include "../../bengine/serialization_utils.hpp"
#include "../../render_engine/chunks/chunks.hpp"

namespace region {

	struct region_t {
		region_t() {
			revealed.resize(REGION_TILES_COUNT);
			visible.resize(REGION_TILES_COUNT);
			solid.resize(REGION_TILES_COUNT);
			opaque.resize(REGION_TILES_COUNT);
			tile_type.resize(REGION_TILES_COUNT);
			tile_material.resize(REGION_TILES_COUNT);
			hit_points.resize(REGION_TILES_COUNT);
			veg_hit_points.resize(REGION_TILES_COUNT);
			building_id.resize(REGION_TILES_COUNT);
			tree_id.resize(REGION_TILES_COUNT);
			tile_vegetation_type.resize(REGION_TILES_COUNT);
			tile_vegetation_ticker.resize(REGION_TILES_COUNT);
			tile_vegetation_lifecycle.resize(REGION_TILES_COUNT);
			tile_flags.resize(REGION_TILES_COUNT);
			water_level.resize(REGION_TILES_COUNT);
			above_ground.resize(REGION_TILES_COUNT);
			blood_stains.resize(REGION_TILES_COUNT);
			stockpile_id.resize(REGION_TILES_COUNT);
			bridge_id.resize(REGION_TILES_COUNT);
		}

		int region_x, region_y, biome_idx;
		std::vector<bool> revealed;
		std::vector<bool> visible;
		std::vector<bool> solid;
		std::vector<bool> opaque;

		// New tile format data
		std::vector<uint8_t> tile_type;
		std::vector<std::size_t> tile_material;
		std::vector<uint16_t> hit_points;
		std::vector<uint8_t> veg_hit_points;
		std::vector<std::size_t> building_id;
		std::vector<std::size_t> tree_id;
		std::vector<std::size_t> bridge_id;
		std::vector<std::size_t> tile_vegetation_type;
		std::vector<uint16_t> tile_vegetation_ticker;
		std::vector<uint8_t> tile_vegetation_lifecycle;
		std::vector<std::size_t> stockpile_id;
		std::vector<bitset8> tile_flags;
		std::vector<uint8_t> water_level;
		std::vector<bool> above_ground;
		std::vector<bool> blood_stains;

		void tile_recalc_all();

		void tile_calculate(const int &x, const int &y, const int &z);

		void tile_pathing(const int &x, const int &y, const int &z);

		void calc_render(const int &idx);

		int next_tree_id = 1;
	};

	std::unique_ptr<region_t> current_region;

	int ground_z(const int x, const int y) {
		int z = REGION_DEPTH - 1;
		bool hit_ground = false;
		while (!hit_ground) {
			const auto idx = mapidx(x, y, z);
			if (current_region->tile_type[idx] == tile_type::SOLID) {
				hit_ground = true;
				++z;
			} else {
				--z;
			}
			if (z == 1) hit_ground = true;
		}
		return z;
	}

	uint8_t water_level(const int idx) {
		return current_region->water_level[idx];
	}

	bool flag(const int idx, uint8_t flag) {
		return current_region->tile_flags[idx].test(flag);
	}

    uint8_t tile_type(const int idx) {
        return current_region->tile_type[idx];
    }

    std::size_t veg_type(const int idx) {
        return current_region->tile_vegetation_type[idx];
    }

    std::size_t material(const int idx) {
        return current_region->tile_material[idx];
    }

    bool solid(const int idx) {
        return current_region->solid[idx];
    }

    bool opaque(const int idx) {
        return current_region->opaque[idx];
    }

    uint8_t veg_hp(const int idx) {
        return current_region->veg_hit_points[idx];
    }

    bool revealed(const int idx) { return current_region->revealed[idx]; }
    uint8_t tile_hit_points(const int idx) { return current_region->hit_points[idx]; }
    std::size_t stockpile_id(const int idx) { return current_region->stockpile_id[idx]; }
    std::size_t bridge_id(const int idx) { return current_region->bridge_id[idx]; }

    uint16_t veg_ticker(const int idx) { return current_region->tile_vegetation_ticker[idx]; }
    uint8_t  veg_lifecycle(const int idx) { return current_region->tile_vegetation_lifecycle[idx]; }

    int region_x() { return current_region->region_x; }
    int region_y() { return current_region->region_y; }
    std::size_t next_tree_id() { return current_region->next_tree_id; }
    std::size_t tree_id(const int idx) { return current_region->tree_id[idx]; }

    std::size_t get_biome_idx() { return current_region->biome_idx; }

    bool above_ground(const int idx) { return current_region->above_ground[idx]; }
    bool blood_stain(const int idx) { return current_region->blood_stains[idx]; }

    void set_tile_type(const int idx, const uint8_t type) {
        current_region->tile_type[idx] = type;
    }

    void set_tile_material(const int idx, const std::size_t material) {
        current_region->tile_material[idx] = material;
        auto mat = get_material(material);
        if (mat) current_region->hit_points[idx] = mat->hit_points;
    }

    void set_veg_type(const int idx, const uint8_t type) {
        current_region->tile_vegetation_type[idx] = type;
    }

    void set_veg_hp(const int idx, const uint8_t hp) {
        current_region->veg_hit_points[idx] = hp;
    }

    void set_veg_ticker(const int idx, const uint16_t ticker) {
        current_region->tile_vegetation_ticker[idx] = ticker;
    }

    void set_veg_lifecycle(const int idx, const uint8_t lifecycle) {
        current_region->tile_vegetation_lifecycle[idx] = lifecycle;
    }

    void set_water_level(const int idx, const uint8_t level) {
        current_region->water_level[idx] = level;
    }

    void set_solid(const int idx, bool val) {
        current_region->solid[idx] = val;
    }

    void set_opaque(const int idx, bool val) {
        current_region->opaque[idx] = val;
    }

    void set_tree_id(const int idx, const std::size_t tree_id) {
        current_region->tree_id[idx] = tree_id;
    }

    void inc_next_tree() {
        ++current_region->next_tree_id;
    }

    void reveal(const int idx) {
        current_region->revealed[idx] = true;
    }

    void make_visible(const int idx) {
        current_region->visible[idx] = true;
    }

    void add_water(const int idx) {
        if (current_region->water_level[idx] < 10) ++current_region->water_level[idx];
    }

    void remove_water(const int idx) {
        if (current_region->water_level[idx]>0) --current_region->water_level[idx];
    }

    void set_flag(const int idx, const uint8_t flag) {
        current_region->tile_flags[idx].set(flag);
    }

    void reset_flag(const int idx, const uint8_t flag) {
        current_region->tile_flags[idx].reset(flag);
    }

    void set_bloodstain(const int idx, const bool val) {
        current_region->blood_stains[idx] = val;
    }

    void set_bridge_id(const int idx, const std::size_t id) {
        current_region->bridge_id[idx] = id;
    }

    void set_stockpile_id(const int idx, const std::size_t id) {
        current_region->stockpile_id[idx] = id;
    }

    void delete_bridge(const std::size_t bridge_id) {
        for (auto &bid : current_region->bridge_id) {
            if (bid == bridge_id) bid = 0;
        }
    }

    void delete_stockpile(const std::size_t stockpile_id) {
        for (auto &sid : current_region->stockpile_id) {
            if (sid == stockpile_id) sid = 0;
        }
    }

    void delete_tree(const std::size_t tree_id) {
        for (auto &sid : current_region->tree_id) {
            if (sid == tree_id) sid = 0;
        }
    }

    void set_tile(const int idx, const uint8_t type, const bool solid, const bool opaque,
                  const std::size_t material, const uint8_t water, const bool remove_vegetation,
                  const bool construction)
    {
        set_tile_type(idx, type);
        current_region->solid[idx] = solid;
        current_region->opaque[idx] = opaque;
        set_tile_material(idx, material);
        if (remove_vegetation) current_region->tile_vegetation_type[idx] = 0;
        current_region->water_level[idx] = water;
        if (construction) current_region->tile_flags[idx].set(CONSTRUCTION);
    }

    void for_all_tiles(const std::function<void(int)> &func) {
        for (int i=0; i<REGION_TILES_COUNT; ++i) {
            func(i);
        }
    }

    void each_bridge(const std::function<void(std::size_t)> &func) {
        for (auto &bid : current_region->bridge_id) func(bid);
    }

    void damage_vegetation(const int idx, const uint8_t damage) {
        if (current_region->veg_hit_points[idx] > damage) {
            current_region->veg_hit_points[idx] -= damage;
        } else {
            current_region->veg_hit_points[idx] = 0;
        }
    }

    void damage_tile(const int idx, const uint8_t damage) {
        if (current_region->hit_points[idx] > damage) {
            current_region->hit_points[idx] -= damage;
        } else {
            current_region->hit_points[idx] = 0;
        }
    }
    
    void zero_map() {
        current_region->next_tree_id = 1;
        std::fill(current_region->visible.begin(), current_region->visible.end(), false);
        std::fill(current_region->solid.begin(), current_region->solid.end(), false);
        std::fill(current_region->opaque.begin(), current_region->opaque.end(), false);
        std::fill(current_region->revealed.begin(), current_region->revealed.end(), false);
        std::fill(current_region->tile_type.begin(), current_region->tile_type.end(), tile_type::OPEN_SPACE);
        std::fill(current_region->tile_material.begin(), current_region->tile_material.end(), 0);
        std::fill(current_region->hit_points.begin(), current_region->hit_points.end(), 0);
        std::fill(current_region->veg_hit_points.begin(), current_region->veg_hit_points.end(), 0);
        std::fill(current_region->building_id.begin(), current_region->building_id.end(), 0);
        std::fill(current_region->tree_id.begin(), current_region->tree_id.end(), 0);
        std::fill(current_region->tile_vegetation_type.begin(), current_region->tile_vegetation_type.end(), 0);
        std::fill(current_region->tile_vegetation_ticker.begin(), current_region->tile_vegetation_ticker.end(), 0);
        std::fill(current_region->tile_vegetation_lifecycle.begin(), current_region->tile_vegetation_lifecycle.end(), 0);
        std::fill(current_region->water_level.begin(), current_region->water_level.end(), 0);
        std::fill(current_region->blood_stains.begin(), current_region->blood_stains.end(), false);
        std::fill(current_region->stockpile_id.begin(), current_region->stockpile_id.end(), 0);
        std::fill(current_region->bridge_id.begin(), current_region->bridge_id.end(), 0);
    }

    void clear_visibility() {
        std::fill(current_region->visible.begin(), current_region->visible.end(), false);
    }

    void new_region(const int x, const int y, const std::size_t biome) {
        current_region = std::make_unique<region_t>();
        current_region ->region_x = x;
        current_region->region_y = y;
        current_region->biome_idx = static_cast<int>(biome);
        zero_map();
    }

    void tile_recalc_all() {
        current_region->tile_recalc_all();
    }

    void calc_render(const int idx) {
        current_region->calc_render(idx);
    }

    void tile_calculate(const int x, const int y, const int z) {
        current_region->tile_calculate(x, y, z);
    }

    void make_open_space(const int idx) {
        current_region->tile_type[idx] = tile_type::OPEN_SPACE;
        current_region->solid[idx] = false;
        current_region->opaque[idx] = false;
        current_region->tile_flags[idx].reset(CAN_STAND_HERE);
        current_region->tile_flags[idx].reset(CONSTRUCTION);
        current_region->tile_vegetation_type[idx] = 0;
    }

    void make_floor(const int idx) {
        current_region->tile_type[idx] = tile_type::FLOOR;
        current_region->solid[idx] = false;
        current_region->opaque[idx] = false;
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
    }

    void make_ramp(const int idx) {
        current_region->tile_type[idx] = tile_type::RAMP;
        current_region->solid[idx] = false;
        current_region->opaque[idx] = false;
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
    }

    void make_stairs_up(const int idx) {
        current_region->tile_type[idx] = tile_type::STAIRS_UP;
        current_region->solid[idx] = false;
        current_region->opaque[idx] = false;
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
    }

    void make_stairs_down(const int idx) {
        current_region->tile_type[idx] = tile_type::STAIRS_DOWN;
        current_region->solid[idx] = false;
        current_region->opaque[idx] = false;
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
    }

    void make_stairs_updown(const int idx) {
        current_region->tile_type[idx] = tile_type::STAIRS_UPDOWN;
        current_region->solid[idx] = false;
        current_region->opaque[idx] = false;
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
    }

    void make_wall(const int idx, const std::size_t mat) {
        current_region->tile_type[idx] = tile_type::WALL;
        current_region->solid[idx] = true;
        current_region->opaque[idx] = true;
        current_region->tile_flags[idx].reset(CAN_STAND_HERE);
        current_region->tile_flags[idx].set(CONSTRUCTION);
        current_region->tile_vegetation_type[idx] = 0;
        set_tile_material(idx, mat);
    }

	void save_current_region() {
		std::string region_filename =
				get_save_path() + std::string("/region_") + std::to_string(current_region->region_x) + "_" +
				std::to_string(current_region->region_y) + ".dat";
		serial::gzip_file deflate(region_filename, "wb");

		deflate.serialize(current_region->region_x);
		deflate.serialize(current_region->region_y);
		deflate.serialize(current_region->biome_idx);
		deflate.serialize(current_region->next_tree_id);

		deflate.serialize_vector_bool(current_region->revealed);
		deflate.serialize_vector_bool(current_region->visible);
		deflate.serialize_vector_bool(current_region->solid);
		deflate.serialize_vector_bool(current_region->opaque);
		deflate.serialize(current_region->tile_type);
		deflate.serialize(current_region->tile_material);
		deflate.serialize(current_region->hit_points);
		deflate.serialize(current_region->veg_hit_points);
		deflate.serialize(current_region->building_id);
		deflate.serialize(current_region->tree_id);
		deflate.serialize(current_region->tile_vegetation_type);
		deflate.serialize(current_region->tile_vegetation_ticker);
		deflate.serialize(current_region->tile_vegetation_lifecycle);
		deflate.serialize(current_region->tile_flags);
		deflate.serialize(current_region->water_level);
		deflate.serialize_vector_bool(current_region->blood_stains);
		deflate.serialize(current_region->stockpile_id);
		deflate.serialize(current_region->bridge_id);
	}

	void load_current_region(const int region_x, const int region_y) {
		std::string region_filename =
				get_save_path() + std::string("/region_") + std::to_string(region_x) + "_" + std::to_string(region_y) +
				".dat";
		serial::gzip_file inflate(region_filename, "rb");
		current_region = std::make_unique<region_t>();

		inflate.deserialize(current_region->region_x);
		inflate.deserialize(current_region->region_y);
		inflate.deserialize(current_region->biome_idx);
		inflate.deserialize(current_region->next_tree_id);

		inflate.deserialize(current_region->revealed);
		inflate.deserialize(current_region->visible);
		inflate.deserialize(current_region->solid);
		inflate.deserialize(current_region->opaque);
		inflate.deserialize(current_region->tile_type);
		inflate.deserialize(current_region->tile_material);
		inflate.deserialize(current_region->hit_points);
		inflate.deserialize(current_region->veg_hit_points);
		inflate.deserialize(current_region->building_id);
		inflate.deserialize(current_region->tree_id);
		inflate.deserialize(current_region->tile_vegetation_type);
		inflate.deserialize(current_region->tile_vegetation_ticker);
		inflate.deserialize(current_region->tile_vegetation_lifecycle);
		inflate.deserialize(current_region->tile_flags);
		inflate.deserialize(current_region->water_level);
		inflate.deserialize(current_region->blood_stains);
		inflate.deserialize(current_region->stockpile_id);
		inflate.deserialize(current_region->bridge_id);

		std::cout << "Recalculating region paths\n";
		current_region->tile_recalc_all();
	}

	void region_t::tile_recalc_all() {
		for (int z = 0; z < REGION_DEPTH; ++z) {
			for (int y = 0; y < REGION_HEIGHT; ++y) {
				for (int x = 0; x < REGION_WIDTH; ++x) {
					tile_calculate(x, y, z);
				}
			}
		}
		for (int z = 0; z < REGION_DEPTH; ++z) {
			for (int y = 0; y < REGION_HEIGHT; ++y) {
				for (int x = 0; x < REGION_WIDTH; ++x) {
					tile_pathing(x, y, z);
				}
			}
		}
	}

	void region_t::tile_calculate(const int &x, const int &y, const int &z) {
		const auto idx = mapidx(x, y, z);

		// Calculate render characteristics
		calc_render(idx);

		// Solidity and first-pass standability
		if (tile_type[idx] == tile_type::SEMI_MOLTEN_ROCK || tile_type[idx] == tile_type::SOLID
			|| tile_type[idx] == tile_type::WALL || tile_type[idx] == tile_type::TREE_TRUNK ||
			tile_type[idx] == tile_type::TREE_LEAF
			|| tile_type[idx] == tile_type::WINDOW || tile_type[idx] == tile_type::CLOSED_DOOR)
        {
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

			if (z > 0) {
				const auto idx_below = mapidx(x, y, z - 1);

				// Can stand on the tile above walls, ramps and up stairs
				if (tile_type[idx] == tile_type::FLOOR) tile_flags[idx].set(CAN_STAND_HERE);
				if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::WALL)
					tile_flags[idx].set(CAN_STAND_HERE);
				if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::RAMP)
					tile_flags[idx].set(CAN_STAND_HERE);
				if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::STAIRS_UP)
					tile_flags[idx].set(CAN_STAND_HERE);
				if (tile_type[idx] == tile_type::OPEN_SPACE &&
					tile_type[idx_below] == tile_type::STAIRS_UPDOWN)
					tile_flags[idx].set(CAN_STAND_HERE);
				if (tile_type[idx] == tile_type::OPEN_SPACE && tile_type[idx_below] == tile_type::SOLID)
					tile_flags[idx].set(CAN_STAND_HERE);
			}
		}

		// Is it underground?
		int test_z = z+1;
        bool ag = true;
        while (test_z < REGION_DEPTH-1) {
            const int testidx = mapidx(x,y,test_z);
            if (solid[testidx] || tile_type[testidx] == tile_type::FLOOR || tile_type[testidx] == tile_type::WALL || tile_type[testidx] == tile_type::SOLID || flag(testidx, CAN_STAND_HERE)) ag = false;
            ++test_z;
        }
        above_ground[idx] = ag;

		tile_pathing(x, y, z);
	}

	void region_t::tile_pathing(const int &x, const int &y, const int &z) {
		const auto idx = mapidx(x, y, z);

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
			if (x > 0 && tile_flags[mapidx(x - 1, y, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_WEST);
			if (x < REGION_WIDTH - 1 && tile_flags[mapidx(x + 1, y, z)].test(CAN_STAND_HERE))
				tile_flags[idx].set(CAN_GO_EAST);
			if (y > 0 && tile_flags[mapidx(x, y - 1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_NORTH);
			if (y < REGION_HEIGHT - 1 && tile_flags[mapidx(x, y + 1, z)].test(CAN_STAND_HERE))
				tile_flags[idx].set(CAN_GO_SOUTH);

			if (z < REGION_DEPTH - 1 &&
				(tile_type[idx] == tile_type::RAMP || tile_type[idx] == tile_type::STAIRS_UP ||
				 tile_type[idx] == tile_type::STAIRS_UPDOWN) &&
				tile_flags[mapidx(x, y, z + 1)].test(CAN_STAND_HERE)) {
				tile_flags[idx].set(CAN_GO_UP);
			}

			if (z > 0 && (tile_type[idx] == tile_type::STAIRS_DOWN || tile_type[idx] == tile_type::STAIRS_UPDOWN) &&
				tile_flags[mapidx(x, y, z - 1)].test(CAN_STAND_HERE)) {
				tile_flags[idx].set(CAN_GO_DOWN);
			}

			if (z > 0 && tile_type[idx] == tile_type::OPEN_SPACE && tile_type[mapidx(x, y, z - 1)] == tile_type::RAMP) {
				tile_flags[idx].set(CAN_GO_DOWN);
			}
		}
	}

	void region_t::calc_render(const int &idx) {
		/*
		if (camera == nullptr) {
			each<camera_options_t>([](entity_t &e, camera_options_t &cam) {
				camera = &cam;
			});
		}

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
			}
				break;
			case tile_type::SOLID : {
				auto mat = get_material(tile_material[idx]);
				glyph = camera->ascii_mode ? 219 : 257;
				if (idx < tile_material.size()) {
					fg = mat->fg;
					bg = mat->bg;
				} else {
					//std::cout << "Warning - material not found (" << idx << ")!\n";
					tile_material[idx] = 1;
					fg = mat->fg;
					bg = mat->bg;
				}
			}
				break;
			case tile_type::OPEN_SPACE : {
				glyph = ' ';
				fg = rltk::colors::BLACK;
			}
				break;
			case tile_type::WALL : {
				if (camera->ascii_mode) {
					uint8_t wall_mask = 0;
					if (tile_type[idx - 1] == tile_type::WALL) wall_mask += 1;
					if (tile_type[idx + 1] == tile_type::WALL) wall_mask += 2;
					if (tile_type[idx - REGION_WIDTH] == tile_type::WALL) wall_mask += 4;
					if (tile_type[idx + REGION_WIDTH] == tile_type::WALL) wall_mask += 8;

					switch (wall_mask) {
						case 0 :
							glyph = 79;
							break; // Isolated
						case 1 :
							glyph = 181;
							break; // West only
						case 2 :
							glyph = 198;
							break; // East only
						case 3 :
							glyph = 205;
							break; // East and West
						case 4 :
							glyph = 208;
							break; // North only
						case 5 :
							glyph = 188;
							break; // North and west
						case 6 :
							glyph = 200;
							break; // North and east
						case 7 :
							glyph = 202;
							break; // North and east/west
						case 8 :
							glyph = 210;
							break; // South only
						case 9 :
							glyph = 187;
							break; // South and west
						case 10 :
							glyph = 201;
							break; // South and east
						case 11 :
							glyph = 203;
							break; // South east/west
						case 12 :
							glyph = 186;
							break; // North and South
						case 13 :
							glyph = 185;
							break; // North/South/West
						case 14 :
							glyph = 204;
							break; // North/South/East
						case 15 :
							glyph = 206;
							break; // All
						default : {
							std::cout << "WARNING: Wall calculator hit a case of " << +wall_mask << "\n";
							glyph = 79;
						}
					}
					fg = get_material(tile_material[idx])->fg;
				} else {
					glyph = 257;
					fg = get_material(tile_material[idx])->fg;
					//bg = material_defs[tile_material[idx]].bg;
				}
			}
				break;
			case tile_type::WINDOW : {
				glyph = camera->ascii_mode ? 176 : 257;
				fg = rltk::colors::CYAN;
			}
				break;
			case tile_type::RAMP : {
				glyph = camera->ascii_mode ? 30 : 257;
				if (tile_material[idx] > 0) {
					fg = get_material(tile_material[idx])->fg;
				} else {
					fg = rltk::colors::GREY;
				}

			}
				break;
			case tile_type::STAIRS_UP : {
				glyph = '<';
				fg = get_material(tile_material[idx])->fg;
			}
				break;
			case tile_type::STAIRS_DOWN : {
				glyph = '>';
				fg = get_material(tile_material[idx])->fg;
			}
				break;
			case tile_type::STAIRS_UPDOWN : {
				glyph = 'X';
				fg = get_material(tile_material[idx])->fg;
			}
				break;
			case tile_type::FLOOR: {

				if (get_material(tile_material[idx])->spawn_type == sand) {
					glyph = camera->ascii_mode ? 126 : 258;
				} else if (get_material(tile_material[idx])->spawn_type == blight) {
					glyph = camera->ascii_mode ? 247 : 265;
				} else if (tile_flags[idx].test(CONSTRUCTION)) {
					glyph = camera->ascii_mode ? '.' : 256;
				} else {
					glyph = camera->ascii_mode ? '`' : 257;
				}

				fg = get_material(tile_material[idx])->fg;

				if (tile_vegetation_type[idx] > 0) {
					//std::cout << plant_defs[tile_vegetation_type[idx]].name << "\n";
					const auto plant = get_plant_def(tile_vegetation_type[idx]);
					const uint8_t lifecycle = tile_vegetation_lifecycle[idx];
					rltk::vchar plant_render;
					plant_render.glyph = camera->ascii_mode ? plant->glyphs_ascii[lifecycle].glyph
															: plant->glyphs[lifecycle];
					if (camera->ascii_mode) {
						plant_render.foreground = plant->glyphs_ascii[lifecycle].foreground;
					} else {
						plant_render.foreground = colors::WHITE;
					}
					veg_cache[idx] = plant_render;
				}
			}
				break;
			case tile_type::CLOSED_DOOR : {
				glyph = camera->ascii_mode ? 197 : 329;
				fg = get_material(tile_material[idx])->fg;

			}
				break;
			case tile_type::TREE_TRUNK : {
				glyph = camera->ascii_mode ? 180 : 259;
				fg = get_material(tile_material[idx])->fg;
				bg = rltk::colors::Black;
				render_cache[idx] = camera->ascii_mode ? vchar{180, rltk::colors::Brown, rltk::colors::BLACK} : vchar{
						glyph, rltk::colors::Brown, bg};
			}
				break;
			case tile_type::TREE_LEAF : {
				glyph = camera->ascii_mode ? 177 : 278;
				fg = camera->ascii_mode ? rltk::colors::GREEN : rltk::colors::White;
				bg = rltk::colors::Black;
			}
				break;
		}

		if (water_level[idx] > 0) {
			switch (water_level[idx]) {
				case 1 :
					glyph = '1';
					break;
				case 2 :
					glyph = '2';
					break;
				case 3 :
					glyph = '3';
					break;
				case 4 :
					glyph = '4';
					break;
				case 5 :
					glyph = '5';
					break;
				case 6 :
					glyph = '6';
					break;
				case 7 :
					glyph = '7';
					break;
				case 8 :
					glyph = '8';
					break;
				case 9 :
					glyph = '9';
					break;
				case 10 :
					glyph = '0';
					break;
				default :
					glyph = '~';
			}
			fg = rltk::colors::Blue;
			bg = rltk::colors::DarkBlue;
		}

		// Apply it
		render_cache[idx] = vchar{glyph, fg, bg};
		 */

	}
}