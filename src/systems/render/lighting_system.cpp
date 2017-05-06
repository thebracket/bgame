#include "lighting_system.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/entity_moved_message.hpp"
#include "../../messages/power_changed_message.hpp"
#include "../ai/visibility_system.hpp"
#include "../../components/lightsource.hpp"
#include "../../main/game_region.hpp"
#include "../../main/game_designations.hpp"

using namespace rltk;

std::vector<color_t> light_map;
std::unordered_map<int, std::pair<int, rltk::color_t>> lit_tiles;

void lighting_system::configure() {
    system_name = "Lighting System";
    subscribe_mbox<map_rerender_message>();
    subscribe_mbox<power_changed_message>();

    subscribe<entity_moved_message>([this] (entity_moved_message &msg) {
		lighting_changed = true;
	});
    light_map.resize(REGION_TILES_COUNT);
    std::fill(light_map.begin(), light_map.end(), rltk::colors::WHITE);
}

inline void reveal(const int &idx, const lightsource_t view, const int light_pos) {
	auto finder = lit_tiles.find(idx);
	if (finder == lit_tiles.end()) {
		lit_tiles[idx] = std::make_pair(light_pos, view.color);
	} else {
		const int new_light_level = view.color.r + view.color.g + view.color.b;
		const int old_light_level = finder->second.second.r + finder->second.second.g + finder->second.second.b;
		if (new_light_level > old_light_level) {
			finder->second.first = light_pos;
			finder->second.second = view.color;
		}
	}
}

inline void internal_light_to(position_t &pos, lightsource_t &view, int x, int y, int z) {
	const float dist_square = (float)(view.radius * view.radius);

	line_func_3d_cancellable(pos.x, pos.y, pos.z, pos.x+x, pos.y+y, pos.z+z, [&view, &pos, &dist_square] (int X, int Y, int Z) {
		const auto idx = mapidx(X, Y, Z);
		const float distance = distance3d_squared(pos.x, pos.y, pos.z, X, Y, Z);
		if (distance > dist_square) {
			return false;
		}
        bool blocked = current_region->opaque[idx];
        if (blocked_visibility.find(idx) != blocked_visibility.end()) blocked = true;
		reveal(idx, view, mapidx(pos));
		return !blocked;
	});
}

void update_normal_light(entity_t &e, position_t &pos, lightsource_t &view) {
    if (view.alert_status) {
        float power_percent = (float)designations->current_power / (float)designations->total_capacity;
        view.color = rltk::lerp(rltk::colors::RED, rltk::colors::WHITE, power_percent);
    }
    const int idx = mapidx(pos);
    lit_tiles[idx] = std::make_pair(idx, view.color); // Always light yourself
	for (int z=(0-view.radius); z<view.radius; ++z) {
		for (int i=0-view.radius; i<view.radius; ++i) {
			internal_light_to(pos, view, i, 0-view.radius, z);
			internal_light_to(pos, view, i, view.radius, z);
			internal_light_to(pos, view, 0-view.radius, i, z);
			internal_light_to(pos, view, view.radius, i, z);
		}
	}
}

void lighting_system::update(double time_ms) {
    std::queue<map_rerender_message> * map_change = mbox<map_rerender_message>();
	while (!map_change->empty()) {
		dirty = true;
        lighting_changed = true;
		map_change->pop();
	}
    std::queue<power_changed_message> * power_change = mbox<power_changed_message>();
	while (!power_change->empty()) {
		dirty = true;
        lighting_changed = true;
		power_change->pop();
	}

    if (dirty) {
        // Rebuild the light map
        lit_tiles.clear();
        each<position_t, lightsource_t>(update_normal_light);
        lighting_changed = true;
        dirty = false;
    }

    /*
    timer += time_ms;
    if (lighting_changed && timer > 33.0) {
        timer = 0.0;


        // Update visible lighting
        const int term_width = term(1)->term_width;
        const int term_height = term(1)->term_height;
        const int tile_size = term_width * term_height;
        if (light_map.size() != tile_size) {
            light_map.resize(tile_size);
        }
        std::fill(light_map.begin(), light_map.end(), color_t{0,0,0});

        const float sun_arc = calendar->sun_arc_percent();
        color_t ambient{80,80,90};
        color_t lit{170,170,196};

        const float latitude = ((float)current_region->region_y - ((float)WORLD_HEIGHT/2.0F)) * 90.0F;
        const float sun_offset_y = latitude * (float)REGION_HEIGHT;

        float sun_offset_x = 0.0F;
        if (calendar->hour > 5 && calendar->hour < 22) {
            sun_offset_x = (1.0F - sun_arc) * (float)REGION_WIDTH;
            ambient = lerp(color_t{80,80,90}, color_t{170,170,165}, sun_arc);
            lit = lerp(color_t{192,192,240}, color_t{255,255,251}, sun_arc);
        }
        if (calendar->hour > 12) sun_offset_x = 0.0F - sun_offset_x;
        const float sun_step_x = sun_offset_x / (float)(REGION_DEPTH);
        const float sun_step_y = sun_offset_y / (float)(REGION_DEPTH);
        const float sun_step_z = 0.5F;

        int Y = 0;
        for (int y=clip_top; y<clip_bottom; ++y) {
            int X = 0;
            for (int x=clip_left; x<clip_right; ++x) {
                const int scridx = (term(1)->term_width * Y) + X;
                const int tileidx = render_tiles[scridx];
                int z = tileidx / (REGION_WIDTH * REGION_HEIGHT);

                if (tileidx > 0) {
                    auto finder = lit_tiles.find(tileidx);
                    if (finder != lit_tiles.end()) {
                        light_map[scridx] = finder->second.second;
                    } else if (current_region->above_ground[tileidx]) {
                        // Determine sunlight
                        bool shadowed = false;

                        float light_x = (float)x;
                        float light_y = (float)y;
                        float light_z = (float)z;
                        int last_z = (int)std::floor(light_z);
                        bool done = false;
                        while (!done) {
                            if (!(light_x < 0 || light_x > REGION_WIDTH-1 || light_y < 0 || light_y > REGION_HEIGHT-1) && current_region->opaque[mapidx((int)light_x, (int)light_y, (int)light_z)]) {
                                done = true;
                                shadowed = true;
                            } else {
                                light_x += sun_step_x;
                                light_y += sun_step_y;
                                light_z += sun_step_z;

                                last_z = std::floor(light_z);

                                if (light_x > REGION_WIDTH || light_x < 0 || light_y > REGION_HEIGHT || light_y < 0 || light_z > REGION_DEPTH-1) {
                                    done = true;
                                }

                                if (!done && last_z != std::floor(light_z)) {
                                    if (current_region->tile_type[mapidx(light_x, light_y, std::floor(light_z))] == tile_type::FLOOR) {
                                        done = true;
                                        shadowed = true;
                                    }
                                }
                            }
                        }


                        if (shadowed) {
                            light_map[scridx] = ambient;
                        } else {
                            light_map[scridx] = lit;
                        }
                    } else {
                        light_map[scridx] = color_t{64,64,64};
                    }
                }
                ++X;
            }
            ++Y;
        }

        lighting_changed = false;
    }*/
}