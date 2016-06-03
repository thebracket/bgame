#include "region.hpp"
#include <rltk.hpp>
#include <string>
#include "../raws/raws.hpp"
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/zlib.hpp>

using namespace rltk;

void save_region(const region_t &region) {
	std::string region_filename = "world/region_" + std::to_string(region.region_x) + "_" + std::to_string(region.region_y) + ".dat";
	std::fstream lbfile(region_filename, std::ios::out | std::ios::binary);
	boost::iostreams::filtering_stream<boost::iostreams::output> deflate;
	deflate.push(boost::iostreams::zlib_compressor());
    deflate.push(lbfile);
	serialize(deflate, region.region_x);
	serialize(deflate, region.region_y);
	serialize(deflate, region.biome_idx);
	serialize(deflate, region.tiles.size());
	for (const tile_t &tile : region.tiles) {
		serialize(deflate, tile.base_type);
		serialize(deflate, tile.contents);
		serialize(deflate, tile.flags);
	}
}

region_t load_region(const int region_x, const int region_y) {
	std::string region_filename = "world/region_" + std::to_string(region_x) + "_" + std::to_string(region_y) + ".dat";
	std::fstream lbfile(region_filename, std::ios::in | std::ios::binary);
	boost::iostreams::filtering_stream<boost::iostreams::input> inflate;
	inflate.push(boost::iostreams::zlib_decompressor());
    inflate.push(lbfile);
	region_t region;

	deserialize(inflate, region.region_x);
	deserialize(inflate, region.region_y);
	deserialize(inflate, region.biome_idx);
	std::size_t number_of_tiles;
	deserialize(inflate, number_of_tiles);
	for (std::size_t i=0; i<number_of_tiles; ++i) {
		tile_t tile;
		deserialize(inflate, tile.base_type);
		deserialize(inflate, tile.contents);
		deserialize(inflate, tile.flags);
		region.tiles[i] = tile;
	}

	region.calculate_render_tiles();
	return region;
}

void region_t::determine_tile_standability(const int &x, const int &y, const int &z) {
	const int index = idx(x,y,z);
	const int index_above = idx(x,y,z+1);
	const bool solid = tiles[index].flags.test(tile_flags::SOLID);
	const bool above_solid = tiles[index_above].flags.test(tile_flags::SOLID);

	// TODO: This isn't right!
	if (solid) {
		tiles[index].flags.reset(tile_flags::CAN_STAND_HERE);
	} else {
		tiles[index].flags.set(tile_flags::CAN_STAND_HERE);
	}

	// You can stand on stairs
	if (tiles[index].flags.test(tile_flags::CONSTRUCTION)) {
		const int content_idx = tiles[index].contents;
		auto finder = tile_contents.find(tiles[index].contents);
		if (finder != tile_contents.end()) {
			if (finder->second.stairs != 0) tiles[index].flags.set(tile_flags::CAN_STAND_HERE);
		}
	}
}

void region_t::determine_tile_connectivity(const int &x, const int &y, const int &z) {
	const int index = idx(x,y,z);
	const bool solid = tiles[index].flags.test(tile_flags::SOLID);
	if (solid) {
		// It's solid - so we can't go anywhere!
		tiles[index].flags.reset(tile_flags::CAN_GO_NORTH);
		tiles[index].flags.reset(tile_flags::CAN_GO_EAST);
		tiles[index].flags.reset(tile_flags::CAN_GO_SOUTH);
		tiles[index].flags.reset(tile_flags::CAN_GO_WEST);
		tiles[index].flags.reset(tile_flags::CAN_GO_UP);
		tiles[index].flags.reset(tile_flags::CAN_GO_DOWN);
		tiles[index].flags.reset(tile_flags::CAN_STAND_HERE);
	} else {
		// Check each of the directions to see if the destination is open
		if (y>1 && tiles[idx(x,y-1,z)].flags.test(tile_flags::CAN_STAND_HERE)) 
			tiles[index].flags.set(tile_flags::CAN_GO_NORTH);
		if (y<REGION_HEIGHT-1 && tiles[idx(x,y+1,z)].flags.test(tile_flags::CAN_STAND_HERE)) 
			tiles[index].flags.set(tile_flags::CAN_GO_SOUTH);
		if (x<REGION_WIDTH-1 && tiles[idx(x+1,y,z)].flags.test(tile_flags::CAN_STAND_HERE)) 
			tiles[index].flags.set(tile_flags::CAN_GO_EAST);
		if (x>1 && tiles[idx(x-1,y,z)].flags.test(tile_flags::CAN_STAND_HERE)) 
			tiles[index].flags.set(tile_flags::CAN_GO_WEST);

		// Stairs gain can_go flags based on type
		if (tiles[index].flags.test(tile_flags::CONSTRUCTION)) {
			const int content_idx = tiles[index].contents;
			auto finder = tile_contents.find(tiles[index].contents);
			if (finder != tile_contents.end()) {
				if ((finder->second.stairs == 1 || finder->second.stairs == 2)) tiles[index].flags.set(tile_flags::CAN_GO_UP);
				if ((finder->second.stairs == 1 || finder->second.stairs == 3)) tiles[index].flags.set(tile_flags::CAN_GO_DOWN);
			}
		}
	}
}

void region_t::determine_connectivity() {
	for (int z=0; z<REGION_DEPTH-1; ++z) {
		for (int y=0; y<REGION_HEIGHT; ++y) {
			for (int x=0; x<REGION_WIDTH; ++x) {
				determine_tile_standability(x, y, z);
			}
		}
	}

	for (int z=0; z<REGION_DEPTH; ++z) {
		for (int y=0; y<REGION_HEIGHT; ++y) {
			for (int x=0; x<REGION_WIDTH; ++x) {
				determine_tile_connectivity(x, y, z);
				//if (tiles[idx(x,y,z)].flags.test(tile_flags::CONSTRUCTION))
				//	std::cout << tiles[idx(x,y,z)].flags << "\n";
			}
		}
	}
}

void region_t::calculate_render_tiles() {
	for (int z=0; z<REGION_DEPTH-1; ++z) {
		for (int y=0; y<REGION_HEIGHT; ++y) {
			for (int x=0; x<REGION_WIDTH; ++x) {
				const int index = idx(x,y,z);
				calculate_render_tile(index);
			}
		}
	}
}

vchar get_render_char_for_base(const uint8_t base_type) {
	if (base_type == 0) return vchar{' ', rltk::colors::MAGENTA, rltk::colors::BLACK};

	auto finder = tile_types.find(base_type);
	if (finder != tile_types.end()) {
		return vchar{ finder->second.glyph, finder->second.fg, finder->second.bg };
	} else {
		return vchar{'?', rltk::colors::MAGENTA, rltk::colors::BLACK};
	}
}

void region_t::calculate_render_tile(const int &idx) {
	vchar result;

	if (tiles[idx].flags.test(tile_flags::SOLID) && !tiles[idx].flags.test(tile_flags::CONSTRUCTION) && tiles[idx].base_type > 0) {
		result = get_render_char_for_base(tiles[idx].base_type);
	} else {
		if (tiles[idx].contents>0) {
			auto finder = tile_contents.find(tiles[idx].contents);
			if (finder != tile_contents.end()) {
				//std::cout << finder->second.name << ", ";
				result = vchar{ finder->second.glyph, finder->second.fg, finder->second.bg };
			} else {
				result = vchar{'?', rltk::colors::MAGENTA, rltk::colors::BLACK};
			}
		} else {
			result=get_render_char_for_base(tiles[idx].base_type);
		}
	}

	tiles[idx].render_as = result;
}