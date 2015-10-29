#include "gui_main_game_view.h"
#include <iostream>
#include "../../engine/globals.h"

using engine::ecs::position_component;

namespace engine {

void gui_main_game_view::render(const screen_region viewport)
{
    const position_component * camera_pos = engine::globals::ecs->find_entity_component<position_component>(world::camera_handle);

    const int left_x = std::max ( 0, camera_pos->x - viewport.w/2 );
    const int top_y = std::max ( 0, camera_pos->y - viewport.h/2 );
    const int right_x = std::min ( landblock_width-1, camera_pos->x + viewport.w/2 );
    const int bottom_y = std::min ( landblock_height-1, camera_pos->y + viewport.h/2 );

    const float sun_angle = world::sun_angle;

    int screen_y = viewport.y;
    for ( int y=top_y; y<=bottom_y; ++y ) {
        int screen_x = viewport.x;
        for ( int x=left_x; x<=right_x; ++x ) {
            const int region_idx = world::current_region->idx ( x,y );
            tile t = world::current_region->tiles[ region_idx ];
            if ( world::current_region->revealed[ region_idx ] ) {
                if ( world::current_region->visible[ region_idx ] ) {
                    auto finder = world::entity_render_list.find(region_idx);
                    if (finder == world::entity_render_list.end()) {
                        const color_t foreground = t.foreground;
                        const color_t background = t.background;
                        const float terrain_angle = t.surface_normal;
                        const float angle_difference = std::abs(terrain_angle - sun_angle);
                        float intensity_pct = 1.0 - (std::abs(angle_difference)/90.0F);
                        if (intensity_pct < 0.25) intensity_pct = 0.25;
                        if (intensity_pct > 1.0) intensity_pct = 1.0;

                        const float red = std::get<0>(foreground) * intensity_pct;
                        const float green = std::get<1>(foreground) * intensity_pct;
                        const float blue = std::get<2>(foreground) * intensity_pct;

                        const color_t fg {
                            red,green,blue
                        };

                        //std::cout << "Ground angle: " << terrain_angle << ", difference: " << angle_difference << ", intensity: " << intensity_pct << "\n";

                        vterm::set_char_xy ( screen_x, screen_y, { t.display, fg, background } );
                    } else {
			vterm::set_char_xy ( screen_x, screen_y, finder->second );
                    }
                } else {
                    vterm::set_char_xy ( screen_x, screen_y, { t.display, color_t{16,16,64}, t.background } );
                }
            }
            ++screen_x;
        }
        ++screen_y;
    }

}

}
