#pragma once

#include "window.h"
#include "../world/world.h"
#include "line_buffer.h"
#include "colors.h"
#include "../game.h"
#include "settler_compositor.h"

namespace render {

class settler_info_window : public window {
public:
     settler_info_window ( sdl2_backend* sdl, const string& title, const bool& decor, const settler_ai_component * dood ) : window ( sdl,title,decor ), settler ( dood ) {
          line_buffer lines;
          game_health_component * health = game_engine->ecs->find_entity_component<game_health_component> ( settler->entity_id );
          game_species_component * species = game_engine->ecs->find_entity_component<game_species_component> ( settler->entity_id );
          game_stats_component * stats = game_engine->ecs->find_entity_component<game_stats_component> ( settler->entity_id );

          {
               stringstream ss;
               ss << settler->first_name << " " << settler->last_name << " (" << settler->profession_tag << ")";
               lines.add_line ( SDL, ss.str(), sdl_yellow );
               window_title = ss.str();
               ss.clear();
          }

          {
               stringstream ss;
               ss << "Hit points: " << health->current_hit_points << " / " << health->max_hit_points;
               lines.add_line ( SDL, ss.str(), sdl_red );
          }

          switch ( species->gender ) {
          case gender_t::MALE :
               lines.add_line ( SDL, "Male", sdl_white );
               break;
          case gender_t::FEMALE :
               lines.add_line ( SDL, "Female", sdl_white );
               break;
          case gender_t::HERMAPHRODITE :
               lines.add_line ( SDL, "Hermaphrodite", sdl_white );
               break;
          case gender_t::ASEXUAL :
               lines.add_line ( SDL, "Asexual", sdl_white );
               break;
          }

          switch ( species->sexual_preference ) {
          case preference_t::HETEROSEXUAL :
               lines.add_line ( SDL, "Heterosexual", sdl_white );
               break;
          case preference_t::HOMOSEXUAL :
               lines.add_line ( SDL, "Homosexual", sdl_white );
               break;
          case preference_t::BISEXUAL :
               lines.add_line ( SDL, "Bisexual", sdl_white );
               break;
          case preference_t::ASEXUAL :
               lines.add_line ( SDL, "Asexual", sdl_white );
               break;
          }

          {
               stringstream ss;
               ss << "Strength: " << stats->strength;
               lines.add_line ( SDL, ss.str(), sdl_green );
          }
          {
               stringstream ss;
               ss << "Dexterity: " << stats->dexterity;
               lines.add_line ( SDL, ss.str(), sdl_green );
          }
          {
               stringstream ss;
               ss << "Constitution: " << stats->constitution;
               lines.add_line ( SDL, ss.str(), sdl_green );
          }
          {
               stringstream ss;
               ss << "Intelligence: " << stats->intelligence;
               lines.add_line ( SDL, ss.str(), sdl_green );
          }
          {
               stringstream ss;
               ss << "Wisdom: " << stats->wisdom;
               lines.add_line ( SDL, ss.str(), sdl_green );
          }
          {
               stringstream ss;
               ss << "Charisma: " << stats->charisma;
               lines.add_line ( SDL, ss.str(), sdl_green );
          }
          {
               stringstream ss;
               ss << "Comeliness: " << stats->comeliness;
               lines.add_line ( SDL, ss.str(), sdl_green );
          }

          const int height = ( lines.size() *16 ) +24;
          location = { 0, 0, std::min ( lines.get_width() +32, 400 ), height };

          allocate();
          render_decorations();

          SDL->texture_target ( texture_id );

          const int x = 16;
          int y = 16;
          for ( const std::pair<std::string,SDL_Color> &panel_line : lines.lines ) {
               std::string line_s = SDL->render_text_to_image ( "disco14", panel_line.first, "tmp", panel_line.second );
               SDL->render_bitmap_simple ( line_s, x, y );
               y += 16;
          }
          
          render_settler_composite ( SDL, stats->entity_id, 10, 10 );

          SDL->reset_texture_target();
     }

private:
     const settler_ai_component * settler;
};

}
