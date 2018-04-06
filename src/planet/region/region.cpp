#include "region.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/plants.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../raws/defs/plant_t.hpp"
#include "../../bengine/serialization_utils.hpp"
#include "../../render_engine/chunks/chunks.hpp"
#include "../../bengine/bitset.hpp"
#include "../../systems/physics/fluid_system.hpp"

using namespace tile_flags;

namespace region {

	struct region_t {
		region_t() {
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
			stockpile_id.resize(REGION_TILES_COUNT);
			bridge_id.resize(REGION_TILES_COUNT);
			veg_render_cache_ascii.resize(REGION_TILES_COUNT);
			building_id.resize(REGION_TILES_COUNT);
		}

		int region_x=0, region_y=0, biome_idx=0;

		// New tile format data
		std::vector<uint8_t> tile_type;
		std::vector<uint16_t> tile_material;
		std::vector<uint16_t> hit_points;
		std::vector<uint8_t> veg_hit_points;
		std::vector<uint32_t> building_id;
		std::vector<uint32_t> tree_id;
		std::vector<uint32_t> bridge_id;
		std::vector<uint16_t> tile_vegetation_type;
		std::vector<uint16_t> tile_vegetation_ticker;
		std::vector<uint8_t> tile_vegetation_lifecycle;
		std::vector<uint32_t> stockpile_id;
		std::vector<bengine::bitset<tile_flag_type>> tile_flags;
		std::vector<uint32_t> water_level;
		std::vector<render::ascii::glyph_t> veg_render_cache_ascii;

		void tile_recalc_all();

		void tile_calculate(const int &x, const int &y, const int &z);

		void tile_pathing(const int &x, const int &y, const int &z);

		void calc_render(const int &idx);

		int next_tree_id = 1;

		void above_ground_calculation();
	};

	std::unique_ptr<region_t> current_region;

	std::vector<bengine::bitset<tile_flag_type>> * get_tile_flags()
	{
		return &current_region->tile_flags;
	}

	std::vector<uint32_t> * get_water_level()
	{
		return &current_region->water_level;
	}

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

	uint32_t water_level(const int idx) {
		return current_region->water_level[idx];
	}

	bool flag(const int idx, const tile_flags::tile_flag_type flag) {
		return current_region->tile_flags[idx].test(flag);
	}

	bengine::bitset<tile_flags::tile_flag_type> get_flag_reference(const int idx)
	{
		return current_region->tile_flags[idx];
	}

    uint8_t tile_type(const int idx) {
        return current_region->tile_type[idx];
    }

	std::vector<uint8_t> * get_tile_types_array()
	{
		return &current_region->tile_type;
	}

    std::size_t veg_type(const int idx) {
        return current_region->tile_vegetation_type[idx];
    }

    std::size_t material(const int idx) {
        return current_region->tile_material[idx];
    }

    uint8_t veg_hp(const int idx) {
        return current_region->veg_hit_points[idx];
    }

    uint16_t tile_hit_points(const int idx) { return current_region->hit_points[idx]; }
    std::size_t stockpile_id(const int idx) { return current_region->stockpile_id[idx]; }
    std::size_t bridge_id(const int idx) { return current_region->bridge_id[idx]; }

	std::size_t get_building_id(const int idx) {
		return current_region->building_id[idx];
	}

	void set_building_id(const int idx, const int id) {
		current_region->building_id[idx] = id;
	}

	void delete_building(const int building_id) {
		for (auto &bid : current_region->building_id) {
			if (bid == building_id) bid = 0;
		}
	}

    uint16_t veg_ticker(const int idx) { return current_region->tile_vegetation_ticker[idx]; }
    uint8_t  veg_lifecycle(const int idx) { return current_region->tile_vegetation_lifecycle[idx]; }
	render::ascii::glyph_t veg_ascii_cache(const int idx) { return current_region->veg_render_cache_ascii[idx]; }
	
    int region_x() { return current_region->region_x; }
    int region_y() { return current_region->region_y; }
    int next_tree_id() { return current_region->next_tree_id; }
    int tree_id(const int idx) { return current_region->tree_id[idx]; }

    std::size_t get_biome_idx() { return current_region->biome_idx; }

    void set_tile_type(const int idx, const uint8_t type) {
		if (idx < 0 || idx > REGION_TILES_COUNT) return;
        current_region->tile_type[idx] = type;
    }

    void set_tile_material(const int idx, const std::size_t material) {
		if (idx < 0 || idx > REGION_TILES_COUNT) return;
        current_region->tile_material[idx] = material;
        const auto mat = get_material(material);
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

    void set_water_level(const int idx, const uint32_t level) {
		systems::fluids::water_dirty = true;
        current_region->water_level[idx] = level;
    }

    void set_tree_id(const int idx, const int tree_id) {
        current_region->tree_id[idx] = tree_id;
    }

    void inc_next_tree() {
        ++current_region->next_tree_id;
    }

    void reveal(const int idx) {
		if (idx < 0 || idx > REGION_TILES_COUNT) return;
		current_region->tile_flags[idx].set(REVEALED);
    }

    void make_visible(const int idx) {
		current_region->tile_flags[idx].set(VISIBLE);
    }

    void add_water(const int idx) {
		systems::fluids::water_dirty = true;
        if (current_region->water_level[idx] < 10) ++current_region->water_level[idx];
    }

    void remove_water(const int idx) {
		systems::fluids::water_dirty = true;
        if (current_region->water_level[idx]>0) --current_region->water_level[idx];
    }

    void set_flag(const int idx, const tile_flags::tile_flag_type flag) {
        current_region->tile_flags[idx].set(flag);
    }

    void reset_flag(const int idx, const tile_flags::tile_flag_type flag) {
        current_region->tile_flags[idx].reset(flag);
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

    void delete_tree(const int tree_id) {
        for (auto &sid : current_region->tree_id) {
            if (sid == tree_id) sid = 0;
        }
    }

    void set_tile(const int idx, const uint8_t type, const bool solid, const bool opaque,
                  const std::size_t material, const uint8_t water, const bool remove_vegetation,
                  const bool construction)
    {
        set_tile_type(idx, type);
		if (solid) {
			current_region->tile_flags[idx].set(SOLID);
		} else
		{
			current_region->tile_flags[idx].reset(SOLID);
		}
		if (opaque)
		{
			current_region->tile_flags[idx].set(OPAQUE_TILE);
		} else
		{
			current_region->tile_flags[idx].reset(OPAQUE_TILE);
		}
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
        std::fill(current_region->stockpile_id.begin(), current_region->stockpile_id.end(), 0);
        std::fill(current_region->bridge_id.begin(), current_region->bridge_id.end(), 0);
    }

    void clear_visibility() {
		for (auto &f : current_region->tile_flags)
		{
			f.reset(VISIBLE);
		}
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
		current_region->tile_flags[idx].reset(SOLID);
		current_region->tile_flags[idx].reset(OPAQUE_TILE);
        current_region->tile_flags[idx].reset(CAN_STAND_HERE);
        current_region->tile_flags[idx].reset(CONSTRUCTION);
        current_region->tile_vegetation_type[idx] = 0;
		chunks::mark_chunk_dirty_by_tileidx(idx);
    }

    void make_floor(const int idx, const std::size_t mat) {
        current_region->tile_type[idx] = tile_type::FLOOR;
		current_region->tile_flags[idx].reset(SOLID);
		current_region->tile_flags[idx].reset(OPAQUE_TILE);
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
		if (mat > 0) set_tile_material(idx, mat);
		chunks::mark_chunk_dirty_by_tileidx(idx);
    }

    void make_ramp(const int idx, const std::size_t mat) {
        current_region->tile_type[idx] = tile_type::RAMP;
		current_region->tile_flags[idx].reset(SOLID);
		current_region->tile_flags[idx].reset(OPAQUE_TILE);
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
		if (mat > 0) set_tile_material(idx, mat);
		chunks::mark_chunk_dirty_by_tileidx(idx);
    }

    void make_stairs_up(const int idx, const std::size_t mat) {
        current_region->tile_type[idx] = tile_type::STAIRS_UP;
		current_region->tile_flags[idx].reset(SOLID);
		current_region->tile_flags[idx].reset(OPAQUE_TILE);
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
		if (mat > 0) set_tile_material(idx, mat);
		chunks::mark_chunk_dirty_by_tileidx(idx);
    }

    void make_stairs_down(const int idx, const std::size_t mat) {
        current_region->tile_type[idx] = tile_type::STAIRS_DOWN;
		current_region->tile_flags[idx].reset(SOLID);
		current_region->tile_flags[idx].reset(OPAQUE_TILE);
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
		if (mat > 0) set_tile_material(idx, mat);
		chunks::mark_chunk_dirty_by_tileidx(idx);
    }

    void make_stairs_updown(const int idx, const std::size_t mat) {
        current_region->tile_type[idx] = tile_type::STAIRS_UPDOWN;
		current_region->tile_flags[idx].reset(SOLID);
		current_region->tile_flags[idx].reset(OPAQUE_TILE);
        current_region->tile_flags[idx].set(CAN_STAND_HERE);
        current_region->tile_vegetation_type[idx] = 0;
		if (mat > 0) set_tile_material(idx, mat);
		chunks::mark_chunk_dirty_by_tileidx(idx);
    }

    void make_wall(const int idx, const std::size_t mat) {
        current_region->tile_type[idx] = tile_type::WALL;
		current_region->tile_flags[idx].reset(SOLID);
		current_region->tile_flags[idx].reset(OPAQUE_TILE);
        current_region->tile_flags[idx].reset(CAN_STAND_HERE);
        current_region->tile_flags[idx].set(CONSTRUCTION);
        current_region->tile_vegetation_type[idx] = 0;
        set_tile_material(idx, mat);
		chunks::mark_chunk_dirty_by_tileidx(idx);
    }

	void update_outdoor_calculation() {
		current_region->above_ground_calculation();
	}

	void save_current_region() {
		const auto region_filename =
				get_save_path() + std::string("/region_") + std::to_string(current_region->region_x) + "_" +
				std::to_string(current_region->region_y) + ".dat";
		serial::gzip_file deflate(region_filename, "wb");

		deflate.serialize(current_region->region_x);
		deflate.serialize(current_region->region_y);
		deflate.serialize(current_region->biome_idx);
		deflate.serialize(current_region->next_tree_id);

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
		deflate.serialize(current_region->stockpile_id);
		deflate.serialize(current_region->bridge_id);
	}

	void load_current_region(const int region_x, const int region_y) {
		const auto region_filename =
				get_save_path() + std::string("/region_") + std::to_string(region_x) + "_" + std::to_string(region_y) +
				".dat";
		serial::gzip_file inflate(region_filename, "rb");
		current_region = std::make_unique<region_t>();

		inflate.deserialize(current_region->region_x);
		inflate.deserialize(current_region->region_y);
		inflate.deserialize(current_region->biome_idx);
		inflate.deserialize(current_region->next_tree_id);

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
		inflate.deserialize(current_region->stockpile_id);
		inflate.deserialize(current_region->bridge_id);

		//std::cout << "Recalculating region paths\n";
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
			tile_flags[idx].set(SOLID);
			if (tile_type[idx] == tile_type::WINDOW) {
				tile_flags[idx].reset(OPAQUE_TILE);
			} else {
				tile_flags[idx].set(OPAQUE_TILE);
			}
			tile_flags[idx].reset(CAN_STAND_HERE);
		} else {
			tile_flags[idx].reset(SOLID);

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
		tile_flags[idx].reset(CAN_GO_NORTH_EAST);
		tile_flags[idx].reset(CAN_GO_NORTH_WEST);
		tile_flags[idx].reset(CAN_GO_SOUTH_EAST);
		tile_flags[idx].reset(CAN_GO_SOUTH_WEST);

		if (tile_flags[idx].test(SOLID) || !tile_flags[idx].test(CAN_STAND_HERE)) {
			// If you can't go there, it doesn't have any exits.
		} else {
			if (x > 0 && tile_flags[mapidx(x - 1, y, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_WEST);
			if (x < REGION_WIDTH - 1 && tile_flags[mapidx(x + 1, y, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_EAST);
			if (y > 0 && tile_flags[mapidx(x, y - 1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_NORTH);
			if (y < REGION_HEIGHT - 1 && tile_flags[mapidx(x, y + 1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_SOUTH);
			if (x > 0 && y > 0 && tile_flags[mapidx(x - 1, y - 1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_NORTH_WEST);
			if (x < REGION_WIDTH - 1 && y > 0 && tile_flags[mapidx(x + 1, y - 1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_NORTH_EAST);
			if (x > 0 && y < REGION_HEIGHT - 1 && tile_flags[mapidx(x - 1, y + 1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_SOUTH_WEST);
			if (x < REGION_WIDTH - 1 && y < REGION_HEIGHT - 1 && tile_flags[mapidx(x + 1, y + 1, z)].test(CAN_STAND_HERE)) tile_flags[idx].set(CAN_GO_SOUTH_EAST);

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
		// Vegetation cache
		using namespace render::ascii;
		glyph_t ascii_vegetation{ '"', 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };

		const auto vegtype = tile_vegetation_type[idx];
		if (vegtype > 0) {
			//std::cout << vegtype << "\n";
			const auto lifecycle = tile_vegetation_lifecycle[idx];
			if (lifecycle < 4) {
				auto plant = get_plant_def(vegtype);
				if (plant) {
					if (!plant->glyphs_ascii.empty()) {
						ascii_vegetation = glyph_t{ static_cast<uint8_t>(plant->glyphs_ascii[lifecycle].glyph), plant->glyphs_ascii[lifecycle].foreground.r, plant->glyphs_ascii[lifecycle].foreground.g, plant->glyphs_ascii[lifecycle].foreground.b,
							plant->glyphs_ascii[lifecycle].background.r, plant->glyphs_ascii[lifecycle].background.g, plant->glyphs_ascii[lifecycle].background.b };
					}
				}
			}
		}

		veg_render_cache_ascii[idx] = ascii_vegetation;
	}

	void region_t::above_ground_calculation() {
		for (auto &f : tile_flags)
		{
			f.reset(ABOVE_GROUND);
		}
		for (auto y = 0; y < REGION_HEIGHT; ++y) {
			for (auto x = 0; x < REGION_WIDTH; ++x) {
				for (auto z = REGION_DEPTH - 1; z > 0; --z) {
					const auto idx = mapidx(x, y, z);
					tile_flags[idx].set(ABOVE_GROUND);
					const auto tt = tile_type[idx];
					const auto hit_ground = (tt == tile_type::SOLID || tt == tile_type::FLOOR || tt == tile_type::WALL);
					if (hit_ground) goto escape_from_dive;
				}

			escape_from_dive: {

				}
			}
		}
	}
}