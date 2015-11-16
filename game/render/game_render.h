#pragma once

#include "../world/world.h"
#include "../systems/system_factory.h"
#include "../game.h"
#include "../raws/raws.h"
#include "panel_tooltip.h"
#include "panel_tile_info.h"
#include <sstream>

using namespace engine;

class game_render : public engine::base_node
{
public:
    virtual void render ( sdl2_backend * SDL ) override;

private:
    enum RENDER_MODE_T { NORMAL, TILE_SELECT };
    RENDER_MODE_T mode = NORMAL;
    int selected_tile_x;
    int selected_tile_y;
  
    void process_mouse_events();
    
    void render_tile_options ( sdl2_backend * SDL );
    void render_tool_tips ( sdl2_backend * SDL );
    void render_power_bar ( sdl2_backend * SDL );
    void render_emotes ( sdl2_backend * SDL );
    void render_date_time ( sdl2_backend * SDL );
    void render_paused ( sdl2_backend * SDL );
    void set_base_source ( SDL_Rect &source, const int &idx );
    bool set_covering_source ( SDL_Rect &source, const int &idx );
    void render_map_ascii ( sdl2_backend * SDL );
    void render_map ( sdl2_backend * SDL );
    void render_lighting_visibility_mask ( sdl2_backend* SDL, const int& idx, SDL_Rect& source, SDL_Rect& dest );
    
    int mouse_x = 0;
    int mouse_y = 0;
    int mouse_vx = 0;
    int mouse_vy = 0;
    int mouse_hover_time = 0;
    
    std::pair<int,int> get_region_coordinates() const;
    
    std::unique_ptr<render::panel_tooltip> current_tooltip;
    std::unique_ptr<render::panel_tile_info> current_info;

};