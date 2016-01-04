#include "settler_compositor.h"
#include "../raws/raws.h"
#include "../game.h"

namespace render {

void render_settler_composite ( sdl2_backend* SDL, const int& entity_id, const int& x, const int& y )
{
     game_species_component * species = ECS->find_entity_component<game_species_component> ( entity_id );

     SDL_Rect src;
     SDL_Rect dest {x,y,16,16};

     // Mask the background
     //src = raws::get_tile_source_by_name("PERSON_BLACK_MASK");
     //SDL->render_bitmap( "spritesheet", src, dest );

     // Render the base gender tile
     if ( species->gender == gender_t::FEMALE ) {
          src = raws::get_tile_source_by_name ( "FEMALE_BARE" );
     } else {
          src = raws::get_tile_source_by_name ( "MALE_BARE" );
     }
     switch ( species->skin_color ) {
       case skin_color_t::AFRICAN : SDL->set_color_mod ( "spritesheet", 128, 58, 45 ); break;
       case skin_color_t::ASIAN : SDL->set_color_mod ( "spritesheet", 199, 151, 120 ); break;
       case skin_color_t::INDIAN : SDL->set_color_mod ( "spritesheet", 179, 121, 100 ); break;
     }
     SDL->render_bitmap ( "spritesheet", src, dest );

     // Hair
     if ( species->hair_style != hair_style_t::BALD ) {
          switch ( species->hair_style ) {
          case hair_style_t::BALDING :
               src = raws::get_tile_source_by_name ( "POINTY_HAIR_MASK" );
               break;
          case hair_style_t::LONG :
               src = raws::get_tile_source_by_name ( "LONG_HAIR_MASK" );
               break;
          case hair_style_t::MOHAWK :
               src = raws::get_tile_source_by_name ( "MOHAWK_HAIR_MASK" );
               break;
          case hair_style_t::PIGTAILS :
               src = raws::get_tile_source_by_name ( "PIGTAIL_HAIR_MASK" );
               break;
          case hair_style_t::SHORT :
               src = raws::get_tile_source_by_name ( "SHORT_HAIR_MASK" );
               break;
          case hair_style_t::TRIANGLE :
               src = raws::get_tile_source_by_name ( "TRIANGLE_HAIR_MASK" );
               break;
          }
          switch ( species->hair_color ) {
          case hair_color_t::BLACK : {
               SDL->set_color_mod ( "spritesheet", 32, 32, 32 );
          }
          break;
          case hair_color_t::BLONDE : {
               SDL->set_color_mod ( "spritesheet", 255, 255, 0 );
          }
          break;
          case hair_color_t::BROWN : {
               SDL->set_color_mod ( "spritesheet", 64, 0, 0 );
          }
          break;
          case hair_color_t::WHITE : {
               SDL->set_color_mod ( "spritesheet", 200, 200, 200 );
          }
          break;
          }

          SDL->render_bitmap ( "spritesheet", src, dest );
          if ( species->bearded ) {
               src = raws::get_tile_source_by_name ( "BEARD_MASK" );
               SDL->render_bitmap ( "spritesheet", src, dest );
          }
     }

     //SDL->set_color_mod ( "spritesheet", 255, 255, 255 );
     vector<item_carried_component> * carried = ECS->find_components_by_type<item_carried_component>();
     for (const item_carried_component item_c : *carried) {
	  if ( item_c.carried_by_id == entity_id ) {
	      const item_component * item = ECS->find_entity_component<item_component>( item_c.entity_id );
	      
	      if (item != nullptr and item->clothing_slot > 0) {
		  const renderable_component * item_r = ECS->find_entity_component<renderable_component>( item_c.entity_id );
		  src = raws::get_tile_source( item_r->tile_idx );
		  if ( item->is_tinted ) {
		    SDL->set_color_mod( "spritesheet", item->tint_red, item->tint_green, item->tint_blue );
		  } else {
		    SDL->set_color_mod ( "spritesheet", 255, 255, 255 );
		  }
		  SDL->render_bitmap ( "spritesheet", src, dest );
	      }
	  }
     }
     
     // Reset color mod
     SDL->set_color_mod ( "spritesheet", 255, 255, 255 );
     
}

}