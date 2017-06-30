#include "lighting_system.hpp"
#include "../../messages/map_dirty_message.hpp"
#include "../../messages/entity_moved_message.hpp"
#include "../../messages/power_changed_message.hpp"
#include "../ai/visibility_system.hpp"
#include "../../components/lightsource.hpp"
#include "../../planet/region/region.hpp"
#include "../../main/game_designations.hpp"
#include "../../utils/gl/chunks/chunk.hpp"

using namespace rltk;

boost::container::flat_map<int, lit_tile_t> lit_tiles;

void lighting_system::configure() {
    system_name = "Lighting System";
    subscribe_mbox<map_rerender_message>();
    subscribe_mbox<power_changed_message>();
}

inline void reveal(const int &idx, const lightsource_t view, const int light_pos) {
    int x,y,z;
    int lx,ly,lz;
    std::tie(x,y,z) = idxmap(idx);
    std::tie(lx,ly,lz) = idxmap(light_pos);
    const int distance = rltk::distance3d_squared(x, y, z, lx, ly, lz);

	auto finder = lit_tiles.find(idx);
	if (finder == lit_tiles.end()) {
		lit_tiles[idx] = lit_tile_t{light_pos, view.color, distance};
	} else {
        if (distance < finder->second.light_distance_squared) {
            finder->second = lit_tile_t{light_pos, view.color, distance};
        }
	}

    // TODO: Implement a comparison to only update if we have to!
	const int chunk_index = gl::chunk_idx(x/gl::CHUNK_SIZE, y/gl::CHUNK_SIZE, z/gl::CHUNK_SIZE);
    gl::chunks[chunk_index].dirty = true;
}

void update_normal_light(entity_t &e, position_t &pos, lightsource_t &view) {
    if (view.alert_status) {
        float power_percent = (float)designations->current_power / (float)designations->total_capacity;
        view.color = rltk::lerp(rltk::colors::RED, rltk::colors::WHITE, power_percent);
    }
    const int idx = mapidx(pos);
    //lit_tiles[idx] = std::make_pair(idx, view.color); // Always light yourself
	reveal(idx, view, idx);

    constexpr float step = 0.0174533f * 5.0f;
    for (float angle = 0.0f; angle < 6.28319f; angle += step) {
        auto projection = project_angle(0, 0, view.radius, angle);

        line_func_cancellable(pos.x, pos.y, pos.x + projection.first, pos.y + projection.second,
                              [&pos, &idx, &view] (int x, int y) {
                                  const int pidx = mapidx(x, y, pos.z);
                                  reveal(pidx, view, idx);
                                  return !region::opaque(pidx);
                              });
    }
}

void lighting_system::update(double time_ms) {
    std::queue<map_rerender_message> * map_change = mbox<map_rerender_message>();
	while (!map_change->empty()) {
		dirty = true;
		map_change->pop();
	}
    std::queue<power_changed_message> * power_change = mbox<power_changed_message>();
	while (!power_change->empty()) {
		dirty = true;
		power_change->pop();
	}

    if (dirty) {
        // Rebuild the light map
        lit_tiles.clear();
        each<position_t, lightsource_t>(update_normal_light);
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