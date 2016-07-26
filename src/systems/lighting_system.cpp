#include "lighting_system.hpp"
#include "camera_system.hpp"
#include "../messages/map_dirty_message.hpp"
#include "../game_globals.hpp"

using namespace rltk;

std::vector<color_t> light_map;

void lighting_system::configure() {
    system_name = "Lighting System";
    subscribe_mbox<map_rerender_message>();
}

void lighting_system::update(double time_ms) {
    std::queue<map_rerender_message> * map_change = mbox<map_rerender_message>();
	while (!map_change->empty()) {
		lighting_changed = true;
		map_change->pop();
	}

    timer += time_ms;
    if (lighting_changed && timer > 33.0) {
        timer = 0.0;
        const int term_width = term(1)->term_width;
        const int term_height = term(1)->term_height;
        const int tile_size = term_width * term_height;
        if (light_map.size() != tile_size) {
            light_map.resize(tile_size);
        }
        std::fill(light_map.begin(), light_map.end(), color_t{0,0,0});

        const float sun_arc = calendar->sun_arc_percent();
        color_t ambient{96,96,120};;
        color_t lit{127,127,180};

        const float latitude = ((float)current_region->region_y - ((float)WORLD_HEIGHT/2.0F)) * 90.0F;
        const float sun_offset_y = latitude * (float)REGION_HEIGHT;

        float sun_offset_x = 0.0F;
        if (calendar->hour > 5 && calendar->hour < 22) {
            sun_offset_x = (1.0F - sun_arc) * (float)REGION_WIDTH/4.0F;
            ambient = lerp(color_t{96,96,120}, color_t{127,127,125}, sun_arc);
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
                    // Check lightsources - or...
                    if (current_region->above_ground[tileidx]) {
                        // Determine sunlight
                        bool shadowed = false;

                        float light_x = (float)x;
                        float light_y = (float)y;
                        float light_z = (float)z;
                        bool done = false;
                        while (!done) {
                            if (current_region->solid[mapidx(light_x, light_y, light_z)]) {
                                done = true;
                                shadowed = true;
                            } else {
                                light_x += sun_step_x;
                                light_y += sun_step_y;
                                light_z += sun_step_z;

                                if (light_x > REGION_WIDTH || light_x < 0 || light_y > REGION_HEIGHT || light_y < 0 || light_z > REGION_DEPTH-1) {
                                    done = true;
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
    }
}