#pragma once

#include "window.h"
#include "../world/world.h"
#include "line_buffer.h"
#include "colors.h"
#include "../game.h"
#include "settler_compositor.h"
#include "../world/tables.h"

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
	      ss << "Age: " << stats->age << " years. ";
	      const int height_inches_total = species->height_cm * 0.39;
	      const int height_feet = height_inches_total / 12;
	      const int height_inches = height_inches_total % 12;
	      ss << "Height: " << height_feet << "'" << height_inches << "\". ";
	      ss << "Weight: " << (species->weight_kg * 2.2) << " lbs.";
	      lines.add_line ( SDL, ss.str(), sdl_yellow );
          }
          
          const string gender_pronoun = (species->gender == gender_t::MALE) ? "He" : "She";
	  
	  {
	      stringstream ss;
	      
	      switch (species->skin_color)
	      {
		case skin_color_t::AFRICAN : ss << gender_pronoun << " is of african descent. "; break;
		case skin_color_t::ASIAN : ss << gender_pronoun << " is of asian descent. "; break;
		case skin_color_t::INDIAN : ss << gender_pronoun << " is of indian-subcontinent descent. "; break;
		case skin_color_t::CAUCASIAN : ss << gender_pronoun << " is caucasian. "; break;
	      }
	      switch (species->hair_color)
	      {
		case hair_color_t::BLACK : ss << gender_pronoun << " has black hair, "; break;
		case hair_color_t::BLONDE : ss << gender_pronoun << " has blonde hair, "; break;
		case hair_color_t::BROWN : ss << gender_pronoun << " has brown hair, "; break;
		case hair_color_t::WHITE : ss << gender_pronoun << " has grey hair, "; break;
	      }
	      switch (species->hair_style)
	      {
		case hair_style_t::BALD : ss << " but is bald."; break;
		case hair_style_t::BALDING : ss << " or what little remains of it, anyway."; break;
		case hair_style_t::LONG : ss << " flowing long and free."; break;
		case hair_style_t::MOHAWK : ss << " in a punk mohawk."; break;
		case hair_style_t::PIGTAILS : ss << " tied into cute pigtails."; break;
		case hair_style_t::SHORT : ss << " cropped short."; break;
		case hair_style_t::TRIANGLE : ss << " carefully teased into a perfect triangle."; break;
	      }
	      if (species->bearded) ss << " " << gender_pronoun << " sports a beard.";
	      
	      lines.add_line ( SDL, ss.str(), sdl_yellow );
          }
	  
	  {
	      stringstream comment;
	      switch ( stat_modifier( stats->strength ) ) {
		case -6 : comment << gender_pronoun << " is so weak they can barely move. "; break;
		case -5 : comment << gender_pronoun << " is exceptionally weak. "; break;
		case -4 : comment << gender_pronoun << " is very weak. "; break;
		case -3: comment << gender_pronoun << " is weak. "; break;
		case -2 : comment << gender_pronoun << " is quite weak. "; break;
		case -1 : comment << gender_pronoun << " is a little weak. "; break;
		case 1 : comment << gender_pronoun << " is quite strong. "; break;
		case 2 : comment << gender_pronoun << " is very strong. "; break;
		case 3 : comment << gender_pronoun << " is remarkably strong. "; break;
		case 4 : comment << gender_pronoun << " is exceptionally strong. "; break;
		case 5 : comment << gender_pronoun << " could give Sueprman a run for his money in a weight-lifting contest. "; break;
	      }
	      
	      switch ( stat_modifier( stats->dexterity ) ) {
		case -6 : comment << gender_pronoun << " is too clumsy to trust with anything breakable. "; break;
		case -5 : comment << gender_pronoun << " is exceptionally clumsy. "; break;
		case -4 : comment << gender_pronoun << " is very clumsy. "; break;
		case -3: comment << gender_pronoun << " is clumsy. "; break;
		case -2 : comment << gender_pronoun << " is quite clumsy. "; break;
		case -1 : comment << gender_pronoun << " is a little clumsy. "; break;
		case 1 : comment << gender_pronoun << " is quite agile. "; break;
		case 2 : comment << gender_pronoun << " is very agile. "; break;
		case 3 : comment << gender_pronoun << " is remarkably agile. "; break;
		case 4 : comment << gender_pronoun << " is exceptionally agile. "; break;
		case 5 : comment << gender_pronoun << " is fast enough to dodge bullets. "; break;
	      }
	      
	      switch ( stat_modifier( stats->constitution ) ) {
		case -6 : comment << gender_pronoun << " is so fragile that it's a wonder they made it this long. "; break;
		case -5 : comment << gender_pronoun << " is exceptionally fragile. "; break;
		case -4 : comment << gender_pronoun << " is very fragile. "; break;
		case -3: comment << gender_pronoun << " is fragile. "; break;
		case -2 : comment << gender_pronoun << " is quite fragile. "; break;
		case -1 : comment << gender_pronoun << " is a little fragile. "; break;
		case 1 : comment << gender_pronoun << " is quite robust. "; break;
		case 2 : comment << gender_pronoun << " is very robust. "; break;
		case 3 : comment << gender_pronoun << " is remarkably robst. "; break;
		case 4 : comment << gender_pronoun << " is exceptionally robust. "; break;
		case 5 : comment << gender_pronoun << " can survive just about anything. "; break;
	      }
	      
	      switch ( stat_modifier( stats->intelligence ) ) {
		case -6 : comment << gender_pronoun << " has trouble with even basic concepts. "; break;
		case -5 : comment << gender_pronoun << " is exceptionally stupid. "; break;
		case -4 : comment << gender_pronoun << " is very stupid. "; break;
		case -3: comment << gender_pronoun << " is stupid. "; break;
		case -2 : comment << gender_pronoun << " is quite stupid. "; break;
		case -1 : comment << gender_pronoun << " is a little stupid. "; break;
		case 1 : comment << gender_pronoun << " is quite clever. "; break;
		case 2 : comment << gender_pronoun << " is very clever. "; break;
		case 3 : comment << gender_pronoun << " is remarkably clever. "; break;
		case 4 : comment << gender_pronoun << " is exceptionally clever. "; break;
		case 5 : comment << gender_pronoun << " can reason through any problem. "; break;
	      }
	      
	      switch ( stat_modifier( stats->wisdom ) ) {
		case -6 : comment << gender_pronoun << " buys everything from infomercials. "; break;
		case -5 : comment << gender_pronoun << " is exceptionally gullible. "; break;
		case -4 : comment << gender_pronoun << " is very gullible. "; break;
		case -3: comment << gender_pronoun << " is gullible. "; break;
		case -2 : comment << gender_pronoun << " is quite gullible. "; break;
		case -1 : comment << gender_pronoun << " is a little gullible. "; break;
		case 1 : comment << gender_pronoun << " is quite wise. "; break;
		case 2 : comment << gender_pronoun << " is very wise. "; break;
		case 3 : comment << gender_pronoun << " is remarkably wise. "; break;
		case 4 : comment << gender_pronoun << " is exceptionally wise. "; break;
		case 5 : comment << gender_pronoun << " is never fooled. "; break;
	      }
	      
	      switch ( stat_modifier( stats->charisma ) ) {
		case -6 : comment << gender_pronoun << " has a personality that even repels priests. "; break;
		case -5 : comment << gender_pronoun << " is exceptionally hard to get along with. "; break;
		case -4 : comment << gender_pronoun << " is very hard to get along with. "; break;
		case -3: comment << gender_pronoun << " is hard to get along with. "; break;
		case -2 : comment << gender_pronoun << " is quite hard to get along with. "; break;
		case -1 : comment << gender_pronoun << " is a little hard to get along with. "; break;
		case 1 : comment << gender_pronoun << " is quite likeable. "; break;
		case 2 : comment << gender_pronoun << " is very likeable. "; break;
		case 3 : comment << gender_pronoun << " is remarkably likeable. "; break;
		case 4 : comment << gender_pronoun << " is exceptionally likeable. "; break;
		case 5 : comment << gender_pronoun << " could sell ice to the inuit. "; break;
	      }
	      
	      switch ( stat_modifier( stats->comeliness ) ) {
		case -6 : comment << gender_pronoun << " is often asked to wear a paper bag on their face. "; break;
		case -5 : comment << gender_pronoun << " is exceptionally ugly. "; break;
		case -4 : comment << gender_pronoun << " is very ugly. "; break;
		case -3: comment << gender_pronoun << " is ugly. "; break;
		case -2 : comment << gender_pronoun << " is ugly. "; break;
		case -1 : comment << gender_pronoun << " is a little ugly. "; break;
		case 1 : comment << gender_pronoun << " is quite attractive. "; break;
		case 2 : comment << gender_pronoun << " is very attractive. "; break;
		case 3 : comment << gender_pronoun << " is remarkably attractive. "; break;
		case 4 : comment << gender_pronoun << " is exceptionally attractive. "; break;
		case 5 : comment << gender_pronoun << " has a face that could launch a thousand ships. "; break;
	      }
	      lines.add_line ( SDL, comment.str(), sdl_yellow );
	  }
	  
	  {
	      stringstream comment;
	      switch ( stat_modifier( stats->ethics ) ) {
		case -6 : comment << settler->first_name << " would sell their entire planet out for a profit. "; break;
		case -5 : comment << settler->first_name << " would sell their grandparents, if it were profitable. "; break;
		case -4 : comment << settler->first_name << " would sell out their friends in the name of a profit. "; break;
		case -3: comment << settler->first_name << " routinely lies and cheats. "; break;
		case -2 : comment << settler->first_name << " sometimes lies and cheats. "; break;
		case -1 : comment << settler->first_name << " sees only gray areas. "; break;	      
		case 1 : comment << settler->first_name << " tends to follow the rules. "; break;
		case 2 : comment << settler->first_name << " thinks everyone should follow the rules. "; break;
		case 3 : comment << settler->first_name << " is unbending in applying the rules. "; break;
		case 4 : comment << settler->first_name << " has a true belief in order. "; break;
		case 5 : comment << settler->first_name << " wouldn't break the rules, even if the entire planet depended upon it. "; break;
	      }
	      if (comment.str().size() > 0)
		  lines.add_line ( SDL, comment.str(), sdl_yellow );
	  }

          const int height = ( lines.size() *16 ) +32;
          location = { 0, 0, lines.get_width() +32, height };

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
          
          render_settler_composite ( SDL, stats->entity_id, location.w - 32, 32 );

          SDL->reset_texture_target();
     }

private:
     const settler_ai_component * settler;
};

}
