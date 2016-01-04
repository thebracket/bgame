#pragma once

#include "window.h"
#include "../world/world.h"
#include "colors.h"
#include "../raws/raws.h"
#include <sstream>
#include <vector>
#include "../game.h"
#include "../messages/build_order_message.h"

using std::vector;
using std::string;

namespace render
{

struct line_t {
    string text;
    SDL_Color color;
    SDL_Rect icon_source;
    int calculated_width;
    bool is_hotspot = false;
    std::function<bool() > callback;
    int hotspot_lines = 0;
};

inline bool do_nothing()
{
    return false;
}

class build_options_window : public window
{
private:
    vector<line_t> lines;
    int current_width = 0;
    int current_y = 8;

    void add_line ( const string &s, const SDL_Color &col, const int &icon_idx = 53, const bool &hotspot = false, std::function<bool() > func = do_nothing, const int &fill_lines=0 ) {
        const pair<int,int> size = SDL->text_size ( "disco14", s );
        line_t line { s, col, raws::get_tile_source ( icon_idx ), size.first, hotspot, func, fill_lines };
        if ( size.first > current_width ) {
            current_width = size.first;
        }
        lines.push_back ( line );
    }

public:
    build_options_window ( sdl2_backend* sdl, const string& title, const bool& decor, const int rx, const int ry, const int rz ) : window ( sdl,title,decor ), region_x ( rx ), region_y ( ry ), region_z(rz) {
        const int idx = world::current_region->idx ( region_x, region_y );

        determine_buildables ( idx );

        const int height = ( lines.size() *16 ) +24;
        location = { 0, 0, current_width +32, height };

        allocate();
        render_decorations();

        SDL->texture_target ( texture_id );

        const int x = 16;
        int y = 16;
        for ( const line_t &panel_line : lines ) {
            std::string line_s = SDL->render_text_to_image ( "disco14", panel_line.text, "tmp", panel_line.color );
            SDL->render_bitmap_simple ( line_s, x, y );
            SDL->render_bitmap ( "spritesheet", panel_line.icon_source, SDL_Rect { 0, y, 16, 16 } );

            if ( panel_line.is_hotspot ) {
                hotspots.push_back ( { SDL_Rect{ 0, y, current_width+32, 16 + ( panel_line.hotspot_lines*16 ) }, panel_line.callback } );
            }

            y += 16;
        }

        SDL->reset_texture_target();
    }

    void determine_buildables ( const int &idx ) {
        // Bail out if not flat
        if ( world::current_region->tiles[idx].base_tile_type != tile_type::FLAT and world::current_region->tiles[idx].base_tile_type != tile_type::BEACH ) {
            add_line ( "You can only build on flat terrain.", sdl_white );
            return;
        }

        // Bail out if there is already something here
        bool occupied = false;
        vector<position_component3d *> positions = ECS->find_components_by_func<position_component3d> (
        [this] ( const position_component3d &c ) {
            return ( c.pos.x == this->region_x and c.pos.y == this->region_y and c.pos.z == this->region_z );
        } );

        for ( const position_component3d * pos : positions ) {
            settler_ai_component * settler = ECS->find_entity_component<settler_ai_component> ( pos->entity_id );
            if ( settler == nullptr ) {
                occupied = true;
            }
        }
        if ( occupied ) {
            add_line ( "There is already a structure here: demolish it before building a new one.", sdl_white );
            return;
        }

        // Iterate the raws and look for something buildable!
        vector< tuple<string,int, vector<string>>> buildables = raws::get_buildables();
        bool can_build_something = false;
        for ( const tuple<string,int,vector<string>> &buildable : buildables ) {
            bool have_components = true;
            int available = 0;

            for ( const string &component : std::get<2> ( buildable ) ) {
                auto finder = world::inventory.find ( component );
                if ( finder == world::inventory.end() ) {
                    have_components = false;
                } else {
                    available = finder->second.size();
                }
            }

            if ( have_components ) {
                int rx = region_x;
                int ry = region_y;
                string name = std::get<0> ( buildable );
                int tile_idx = std::get<1> ( buildable );
                can_build_something = true;
                add_line ( std::get<0> ( buildable ), sdl_yellow, tile_idx, true, [=] {
                    game_engine->messaging->add_message<build_order_message> ( build_order_message ( rx, ry, name ) );
                    return true;
                }, std::get<2> ( buildable ).size() );

                for ( const string &component : std::get<2> ( buildable ) ) {
                    std::stringstream ss;
                    ss << "  Requires: " << component << " (" << available << " available)";
                    add_line ( ss.str(), sdl_white );
                }

            }
        }
        if ( !can_build_something ) {
            // Default
            add_line ( "Nothing can be built here.", sdl_white );
        }
    }

private:
    const int region_x;
    const int region_y;
    const int region_z;

};

}
