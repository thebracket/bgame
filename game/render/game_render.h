#pragma once

#include "../world/world.h"
#include "../systems/system_factory.h"
#include "../game.h"
#include "../raws/raws.h"
#include <sstream>

using namespace engine;

class game_render : public engine::base_node
{
public:
    virtual void render ( sdl2_backend * SDL ) override;

private:
    void process_mouse_events();
    
    void render_tool_tips ( sdl2_backend * SDL );
    void render_power_bar ( sdl2_backend * SDL );
    void render_emotes ( sdl2_backend * SDL );
    void render_date_time ( sdl2_backend * SDL );
    void render_paused ( sdl2_backend * SDL );
    void set_base_source ( SDL_Rect &source, const int &idx );
    bool set_covering_source ( SDL_Rect &source, const int &idx );
    void render_map_ascii ( sdl2_backend * SDL );
    void render_map ( sdl2_backend * SDL );
    
    int mouse_x = 0;
    int mouse_y = 0;
    int mouse_vx = 0;
    int mouse_vy = 0;
    int mouse_hover_time = 0;
    
    std::pair<int,int> get_region_coordinates() const;

};