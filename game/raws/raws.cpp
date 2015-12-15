#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "raws.h"

#include "../../engine/virtual_terminal.h"
#include "raw_renderable.h"
#include "raw_name.h"
#include "raw_glyph.h"
#include "raw_color_pair.h"
#include "raw_obstruction.h"
#include "raw_power_generator.h"
#include "raw_power_battery.h"
#include "raw_description.h"
#include "raw_settler_action.h"
#include "raw_buildable.h"
#include "raw_skill_required.h"
#include "raw_component.h"
#include "raw_sleepable.h"
#include "raw_tile.h"
#include "raw_render_layer.h"
#include "raw_item_type.h"
#include "raw_modifier.h"
#include "raw_clothing_color.h"
#include "raw_clothing_slot.h"
#include "raw_clothing.h"
#include "raw_workshop.h"
#include "raw_emote.h"
#include "raw_input.h"
#include "raw_power_drain.h"
#include "raw_output.h"
#include "raw_particle_emitter.h"

#include "../game.h"

using std::ifstream;
using std::string;
using std::vector;
using std::unordered_map;
using std::unique_ptr;
using std::make_unique;

namespace raws {

namespace detail {


unordered_map<string, engine::vterm::color_t> colors;
unordered_map<string, unsigned char> glyphs;
unordered_map<string, unique_ptr<base_raw>> structures;
unordered_map<string, unique_ptr<base_raw>> starting_professions;
unordered_map<string, unique_ptr<base_raw>> items;
unordered_map<string, unique_ptr<base_raw>> reactions;
unordered_map<string, int> tiles;

}

const string get_index_filename()
{
     return "../raw/index.txt";
}

vector<string> get_files_to_read()
{
     vector<string> result;

     ifstream index ( get_index_filename() );
     if ( !index.is_open() ) {
          throw 101;
     }

     string line;
     while ( getline ( index, line ) ) {
          result.push_back ( "../raw/" + line );
     }

     return result;
}

unique_ptr<base_raw> current;
unique_ptr<base_raw> current_render;
unique_ptr<base_raw> current_sub_chunk;
string current_name;
enum nested_enum {NONE,STRUCTURE,STARTING_PROFESSION,ITEM,REACTION};
nested_enum nested_type;
bool nested;

/* Lookup Functions */

unsigned char find_glyph_by_name ( const string &name )
{
     auto finder = detail::glyphs.find ( name );
     if ( finder == detail::glyphs.end() ) {
          std::cout << "Error; could not find glyph; [" << name << "]\n";
          throw 104;
     }
     return finder->second;
}

engine::vterm::color_t find_color_by_name ( const string &name )
{
     auto finder = detail::colors.find ( name );
     if ( finder == detail::colors.end() ) {
          std::cout << "Error; could not find color; [" << name << "]\n";
          throw 105;
     }
     return finder->second;
}

int find_tile_by_name ( const string &name )
{
     auto finder = detail::tiles.find ( name );
     if ( finder == detail::tiles.end() ) {
          std::cout << "Error; could not find tile; [" << name << "]\n";
          throw 106;
     }
     return finder->second;
}

SDL_Rect get_tile_source ( const int idx )
{
     //const int width = 112;
     const int width_t = 9;

     const int x = idx % width_t;
     const int y = idx / width_t;

     return SDL_Rect { x*16, y*16, 16, 16 };
}

SDL_Rect get_tile_source_by_name ( const string &name )
{
     const int idx = find_tile_by_name ( name );
     return get_tile_source ( idx );
}

/* Raw Factory Functions */

void parse_raw_name ( const vector<string> &chunks )
{
     //std::cout << "Parsing name: " << chunks[1] << "\n";
     current_name = chunks[1];
     unique_ptr<raw_name> name = make_unique<raw_name> ( chunks[1] );
     current->children.push_back ( std::move ( name ) );
}

void parse_raw_description ( const vector<string> &chunks )
{
     const string description = chunks[1];
     unique_ptr<raw_description> desc = make_unique<raw_description> ( description );
     current->children.push_back ( std::move ( desc ) );
}

void parse_raw_obstruction ( const vector<string> &chunks )
{
     const string view_s = chunks[1];
     const string walk_s = chunks[2];

     bool view = false;
     bool walk = false;
     if ( view_s == "Y" ) {
          view = true;
     }
     if ( walk_s == "Y" ) {
          walk = true;
     }
     unique_ptr<raw_obstruction> obs = make_unique<raw_obstruction> ( view,walk );
     current->children.push_back ( std::move ( obs ) );
}

void parse_raw_glyph ( const vector<string> &chunks )
{
     const string glyph_name = chunks[1];
     const unsigned char glyph = find_glyph_by_name ( glyph_name );
     unique_ptr<raw_glpyh> g = make_unique<raw_glpyh> ( glyph );
     current_render->children.push_back ( std::move ( g ) );
}

void parse_raw_tile ( const vector<string> &chunks )
{
     const string tile_name = chunks[1];
     const int tile_idx = find_tile_by_name ( tile_name );
     unique_ptr<raw_tile> t = make_unique<raw_tile> ( tile_idx );
     current_render->children.push_back ( std::move ( t ) );
}

void parse_raw_render_layer ( const vector<string> &chunks )
{
     const string layer_s = chunks[1];

     int layer_idx = 2;
     if ( layer_s == "BUILDING" ) {
          layer_idx = 1;
     }
     if ( layer_s == "FLOOR" ) {
          layer_idx = 0;
     }
     unique_ptr<raw_render_layer> t = make_unique<raw_render_layer> ( layer_idx );
     current_render->children.push_back ( std::move ( t ) );
}

void parse_raw_color_pair ( const vector<string> &chunks )
{
     const string fg = chunks[1];
     const string bg = chunks[2];
     const color_t fgc = find_color_by_name ( fg );
     const color_t bgc = find_color_by_name ( bg );
     unique_ptr<raw_color_pair> cp = make_unique<raw_color_pair> ( fgc,bgc );
     current_render->children.push_back ( std::move ( cp ) );
}

void parse_raw_power_generator ( const vector<string> &chunks )
{
     const string mode = chunks[1];
     const string amount_i = chunks[2];
     const int amount = std::stoi ( amount_i );
     power_generator_condition power_mode;
     if ( mode == "daylight" ) {
          power_mode = DAYLIGHT;
     }
     unique_ptr<raw_power_generator> gen = make_unique<raw_power_generator> ( power_mode, amount );
     current->children.push_back ( std::move ( gen ) );
}

void parse_raw_power_battery ( const vector<string> &chunks )
{
     const string capacity_s = chunks[1];
     const int capacity = std::stoi ( capacity_s );
     unique_ptr<raw_power_battery> batt = make_unique<raw_power_battery> ( capacity );
     current->children.push_back ( std::move ( batt ) );
}

void parse_raw_item_type ( const vector<string> &chunks )
{
     const string type = chunks[1];
     unique_ptr<raw_item_type> rtype = make_unique<raw_item_type> ( type );
     current->children.push_back ( std::move ( rtype ) );
}

void parse_raw_modifier ( const vector<string> &chunks )
{
     //std::cout << "Parsing name: " << chunks[1] << "\n";
     string stat_name = chunks[1];
     int stat_mod = std::stoi(chunks[2]);
     unique_ptr<raw_modifier> name = make_unique<raw_modifier> ( chunks[1], stat_mod );
     current->children.push_back ( std::move ( name ) );
}

void parse_raw_clothing_slot ( const vector<string> &chunks )
{
    const string slot_name = chunks[1];
    unique_ptr< raw_clothing_slot > slot = make_unique< raw_clothing_slot > ( slot_name );
    current->children.push_back ( std::move ( slot ) );
}

void parse_raw_clothing_color ( const vector<string> &chunks )
{
    unique_ptr< raw_clothing_color > color = make_unique< raw_clothing_color > ( );
    for (int i=1; i<chunks.size(); ++i) {
      color->colors.push_back( chunks[i] );
    }
    current->children.push_back ( std::move ( color ) );
}

void parse_raw_clothing ( const vector<string> &chunks )
{
    const string clothing_slot = chunks[1];
    const string clothing_item = chunks[2];
    int gender = 0;
    if (chunks[0] == "CLOTHING_MALE") gender = 1;
    if (chunks[0] == "CLOTHING_FEMALE") gender = 2;
    unique_ptr< raw_clothing > slot = make_unique< raw_clothing > ( clothing_slot, clothing_item, gender );
    current->children.push_back ( std::move ( slot ) );
}

void parse_raw_workshop ( const vector<string> &chunks )
{
     const string description = chunks[1];
     unique_ptr<raw_workshop> desc = make_unique<raw_workshop> ( description );
     current->children.push_back ( std::move ( desc ) );
}

void parse_raw_emote ( const vector<string> &chunks )
{
     const string description = chunks[1];
     unique_ptr<raw_emote> desc = make_unique<raw_emote> ( description );
     current->children.push_back ( std::move ( desc ) );
}

void parse_raw_input ( const vector<string> &chunks )
{
     //std::cout << "Parsing name: " << chunks[1] << "\n";
     string item = chunks[1];
     int qty = std::stoi(chunks[2]);
     unique_ptr<raw_input> name = make_unique<raw_input> ( chunks[1], qty );
     current->children.push_back ( std::move ( name ) );
}

void parse_raw_output ( const vector<string> &chunks )
{
     //std::cout << "Parsing name: " << chunks[1] << "\n";
     string item = chunks[1];
     int qty = std::stoi(chunks[2]);
     unique_ptr<raw_output> name = make_unique<raw_output> ( chunks[1], qty );
     current->children.push_back ( std::move ( name ) );
}

void parse_raw_power_drain ( const vector<string> &chunks )
{
     const string drain_text = chunks[1];
     const int drain_int = std::stoi( drain_text );
     unique_ptr<raw_power_drain> desc = make_unique<raw_power_drain> ( drain_int );
     current->children.push_back ( std::move ( desc ) );
}

/* Specific parser functions */

void parse_structure ( const vector<string> &chunks )
{
     //std::cout << "Adding " << chunks[0] << " to structure.\n";
     if ( chunks[0] == "/STRUCTURE" ) {
          nested = false;
          nested_type = NONE;
          detail::structures[current_name] = std::move ( current );
          current.reset();
          return;
     }
     if ( chunks[0] == "/RENDER" ) {
          current->children.push_back ( std::move ( current_render ) );
          current_render.reset();
          return;
     }
     if ( chunks[0] == "/SETTLER-ACTION" ) {
          current->children.push_back ( std::move ( current_sub_chunk ) );
          current_sub_chunk.reset();
          return;
     }
     if ( chunks[0] == "/BUILDABLE" ) {
          current->children.push_back ( std::move ( current_sub_chunk ) );
          current_sub_chunk.reset();
          return;
     }
     if ( chunks[0] == "RENDER" ) {
          current_render = make_unique<raw_renderable>();
          return;
     }
     if ( chunks[0] == "SETTLER-ACTION" ) {
          current_sub_chunk = make_unique<raw_settler_action>();
          return;
     }
     if ( chunks[0] == "BUILDABLE" ) {
          current_sub_chunk = make_unique<raw_buildable>();
          return;
     }
     if ( chunks[0] == "NAME" ) {
          parse_raw_name ( chunks );
          return;
     }
     if ( chunks[0] == "DESCRIPTION" ) {
          parse_raw_description ( chunks );
          return;
     }
     if ( chunks[0] == "RENDER_GLYPH" ) {
          parse_raw_glyph ( chunks );
          return;
     }
     if ( chunks[0] == "RENDER_COLOR" ) {
          parse_raw_color_pair ( chunks );
          return;
     }
     if ( chunks[0] == "RENDER_TILE" ) {
          parse_raw_tile ( chunks );
          return;
     }
     if ( chunks[0] == "RENDER_LAYER" ) {
          parse_raw_render_layer ( chunks );
          return;
     }
     if ( chunks[0] == "OBSTRUCTS" ) {
          parse_raw_obstruction ( chunks );
          return;
     }
     if ( chunks[0] == "GENERATOR" ) {
          parse_raw_power_generator ( chunks );
          return;
     }
     if ( chunks[0] == "BATTERY" ) {
          parse_raw_power_battery ( chunks );
          return;
     }
     if ( chunks[0] == "SA-NAME" ) {
          raw_settler_action * tmp = static_cast<raw_settler_action *> ( current_sub_chunk.get() );
          tmp->action_name = chunks[1];
          return;
     }
     if ( chunks[0] == "SA-PROVIDES" ) {
          raw_settler_action * tmp = static_cast<raw_settler_action *> ( current_sub_chunk.get() );
          tmp->provides = chunks[1];
          tmp->provides_qty = std::stoi ( chunks[2] );
          return;
     }
     if ( chunks[0] == "SA-POWER_USE" ) {
          raw_settler_action * tmp = static_cast<raw_settler_action *> ( current_sub_chunk.get() );
          tmp->power_drain = std::stoi ( chunks[1] );
          return;
     }
     if ( chunks[0] == "BUILDABLE_COMPONENT" ) {
          current_sub_chunk->children.push_back ( make_unique<raw_component> ( chunks[1], std::stoi ( chunks[2] ) ) );
          return;
     }
     if ( chunks[0] == "BUILDABLE_SKILL" ) {
          current_sub_chunk->children.push_back ( make_unique<raw_skill_required> ( chunks[1], std::stoi ( chunks[2] ), std::stoi ( chunks[3] ) ) );
          return;
     }
     if ( chunks[0] == "SLEEP_HERE" ) {
          current->children.push_back ( make_unique<raw_sleepable>() );
          return;
     }
     if ( chunks[0] == "PARTICLE" ) {
	  const string message = chunks[1];
	  const int frequency = std::stoi( chunks[2] );
	  const int ttl = std::stoi( chunks[3] );
	  const string color_s = chunks[4];
	  chat_emote_color_t color = WHITE;
	  if (color_s == "YELLOW") color = YELLOW;
	  if (color_s == "CYAN") color = CYAN;
	  if (color_s == "GREEN") color = GREEN;
	  if (color_s == "MAGENTA") color = MAGENTA;
	  if (color_s == "RED") color = RED;
	  if (color_s == "BLUE") color = BLUE;
	  if (color_s == "BLACK") color = BLACK;
          current->children.push_back ( make_unique<raw_particle_emitter>( message, ttl, frequency, color ) );
          return;
     }

     std::cout << "WARNING: GOT TO END OF STRUCTURE CHUNK READER WITH NO MATCH FOR [" << chunks[0] << "]\n";
}

void parse_starting_profession ( const vector<string> &chunks )
{
     if ( chunks[0] == "/STARTING_PROFESSION" ) {
          nested = false;
          nested_type = NONE;
          detail::starting_professions[current_name] = std::move ( current );
          current.reset();
          return;
     }
     if ( chunks[0] == "NAME" ) {
          parse_raw_name ( chunks );
          return;
     }
     if ( chunks[0] == "MODIFIER" ) {
          parse_raw_modifier ( chunks );
          return;
     }
     if ( chunks[0] == "CLOTHING" ) {
          parse_raw_clothing ( chunks );
          return;
     }
     if ( chunks[0] == "CLOTHING_MALE" ) {
          parse_raw_clothing ( chunks );
          return;
     }
     if ( chunks[0] == "CLOTHING_FEMALE" ) {
          parse_raw_clothing ( chunks );
          return;
     }
}

void parse_item ( const vector<string> &chunks )
{
     if ( chunks[0] == "/ITEM" ) {
          nested = false;
          nested_type = NONE;
          detail::items[current_name] = std::move ( current );
          current.reset();
          return;
     }
     if ( chunks[0] == "/RENDER" ) {
          current->children.push_back ( std::move ( current_render ) );
          current_render.reset();
          return;
     }
     if ( chunks[0] == "ITEM_TYPE" ) {
          parse_raw_item_type ( chunks );
          return;
     }
     if ( chunks[0] == "RENDER" ) {
          current_render = make_unique<raw_renderable>();
          return;
     }
     if ( chunks[0] == "NAME" ) {
          parse_raw_name ( chunks );
          return;
     }
     if ( chunks[0] == "DESCRIPTION" ) {
          parse_raw_description ( chunks );
          return;
     }
     if ( chunks[0] == "RENDER_GLYPH" ) {
          parse_raw_glyph ( chunks );
          return;
     }
     if ( chunks[0] == "RENDER_COLOR" ) {
          parse_raw_color_pair ( chunks );
          return;
     }
     if ( chunks[0] == "RENDER_TILE" ) {
          parse_raw_tile ( chunks );
          return;
     }
     if ( chunks[0] == "RENDER_LAYER" ) {
          parse_raw_render_layer ( chunks );
          return;
     }
     if ( chunks[0] == "CLOTHING_SLOT" ) {
          parse_raw_clothing_slot ( chunks );
          return;
     }
     if ( chunks[0] == "CLOTHING_COLOR" ) {
          parse_raw_clothing_color ( chunks );
          return;
     }
}

void parse_reaction ( const vector<string> &chunks )
{
     if ( chunks[0] == "/REACTION" ) {
          nested = false;
          nested_type = NONE;
          detail::reactions[current_name] = std::move ( current );
          current.reset();
          return;
     }
     if ( chunks[0] == "NAME" ) {
          parse_raw_name ( chunks );
          return;
     }
     if ( chunks[0] == "WORKSHOP" ) {
          parse_raw_workshop ( chunks );
          return;
     }
     if ( chunks[0] == "EMOTE" ) {
          parse_raw_emote ( chunks );
          return;
     }
     if ( chunks[0] == "INPUT" ) {
          parse_raw_input ( chunks );
          return;
     }
     if ( chunks[0] == "POWER_DRAIN" ) {
          parse_raw_power_drain ( chunks );
          return;
     }
     if ( chunks[0] == "OUTPUT" ) {
          parse_raw_output ( chunks );
          return;
     }
}

/* Chunk Parsing */

void parse_nested_chunk ( const vector<string> &chunks )
{
     switch ( nested_type ) {
     case STRUCTURE :
          parse_structure ( chunks );
          break;
     case STARTING_PROFESSION :
          parse_starting_profession ( chunks );
          break;
     case ITEM :
          parse_item ( chunks );
          break;
     case REACTION :
	  parse_reaction ( chunks );
	  break;
     case NONE :
          throw 103;
     }
}

void parse_whole_chunk ( const vector<string> &chunks )
{
     //std::cout << "Reading whole chunk of type: " << chunks[0] << "\n";

     // Simple definition tags
     if ( chunks[0] == "GLYPH" ) {
          const string glyph_name = chunks[1];
          const string glyph_ascii_number = chunks[2];
          const int glyph_ascii_int = std::stoi ( glyph_ascii_number );
          const unsigned char glyph = static_cast<unsigned char> ( glyph_ascii_int );
          detail::glyphs[glyph_name] = glyph;
          //std::cout << "Learned glyph: " << glyph_name << "[" << +glyph << "]\n";
          return;
     }
     if ( chunks[0] == "COLOR" ) {
          const string color_name = chunks[1];
          const string red_string = chunks[2];
          const string green_string = chunks[3];
          const string blue_string = chunks[4];
          const int red_i = std::stoi ( red_string );
          const int green_i = std::stoi ( green_string );
          const int blue_i = std::stoi ( blue_string );
          detail::colors[color_name] = engine::vterm::color_t {red_i, green_i, blue_i};
          return;
     }
     if ( chunks[0] == "TILE" ) {
          const string tile_name = chunks[1];
          const string tile_idx_s = chunks[2];
          const int tile_idx = std::stoi ( tile_idx_s );
          detail::tiles [ tile_name ] = tile_idx;
          return;
     }

     // Tags that start a nested structure
     if ( chunks[0] == "STRUCTURE" ) {
          nested = true;
          nested_type = STRUCTURE;
          current = make_unique<base_raw>();
          return;
     }
     if ( chunks[0] == "STARTING_PROFESSION" ) {
          nested = true;
          nested_type = STARTING_PROFESSION;
          current = make_unique<base_raw>();
          return;
     }
     if ( chunks[0] == "ITEM" ) {
          nested = true;
          nested_type = ITEM;
          current = make_unique<base_raw>();
          return;
     }
     if ( chunks[0] == "REACTION" ) {
	  nested = true;
	  nested_type = REACTION;
	  current = make_unique<base_raw>();
	  return;
     }

     std::cout << "WARNING: GOT TO END OF CHUNK READER WITH NO MATCH FOR [" << chunks[0] << "]\n";
}

void parse_chunks ( const vector<string> &chunks )
{
     //std::cout << "Nested? " << nested << "\n";
     if ( nested ) {
          parse_nested_chunk ( chunks );
     } else {
          parse_whole_chunk ( chunks );
     }
}

inline bool ignore_char ( const char &c )
{
     if ( c == ' ' ) {
          return false;
     }
     if ( c>40 and c<123 and c != '[' and c != ']' ) {
          return false;
     }
     return true;
}

void parse_line ( const string &line )
{
     if ( line.empty() ) {
          return;     // No parsing of empty lines
     }
     if ( line[0] == '*' ) {
          return;     // No parsing of comments
     }

     // Break the line into : separated chunks, ignoring [ and ]
     vector<string> chunks;
     string current_line;
     for ( unsigned int i=0; i<line.size(); ++i ) {
          if ( !ignore_char ( line[i] ) ) {
               if ( line[i] != ':' ) {
                    current_line += line[i];
               } else {
                    string chunk ( current_line );
                    chunks.push_back ( chunk );
                    current_line.clear();
               }
          }
          if ( line[i] == ']' ) {
               chunks.push_back ( current_line );
          }
     }

     /*std::cout << "Parsed/Chunked Line: ";
     for (const string &s : chunks) {
     std::cout << "[" << s << "] ";
     }
     std::cout<< "\n";*/

     if ( !chunks.empty() ) {
          parse_chunks ( chunks );
     }
}

void parse_raw_file ( const string &filename )
{
     ifstream file ( filename );
     if ( !file.is_open() ) {
          throw 102;
     }
     string line;
     while ( getline ( file,line ) ) {
          parse_line ( line );
     }
}

/* Control Flow */

void read_raws()
{
     nested = false;
     nested_type = NONE;

     const vector<string> files_to_read = get_files_to_read();
     for ( string file_to_read : files_to_read ) {
          parse_raw_file ( file_to_read );
     }
}

/* Entity-Component Generation */

int create_structure_from_raws ( const string &name, const int &x, const int &y )
{
     auto finder = detail::structures.find ( name );
     if ( finder == detail::structures.end() ) {
          std::cout << "ERROR: Cannot create structure of type [" << name << "]\n";
          throw 105;
     }

     entity e = game_engine->ecs->add_entity();
     game_engine->ecs->add_component ( e, position_component ( x,y ) );
     finder->second->build_components ( e, x, y );

     return e.handle;
}

int create_structure_from_raws ( const string &name, const location_t loc )
{
     auto finder = detail::structures.find ( name );
     if ( finder == detail::structures.end() ) {
          std::cout << "ERROR: Cannot create structure of type [" << name << "]\n";
          throw 105;
     }

     entity e = game_engine->ecs->add_entity();
     game_engine->ecs->add_component ( e, position_component3d ( loc, OMNI ) );
     finder->second->build_components ( e, 0, 0 );

     return e.handle;
}

int create_placeholder_structure_from_raws ( const string &name, const int &x, const int &y )
{
     auto finder = detail::structures.find ( name );
     if ( finder == detail::structures.end() ) {
          std::cout << "ERROR: Cannot create structure of type [" << name << "]\n";
          throw 105;
     }

     entity e = game_engine->ecs->add_entity ();
     game_engine->ecs->add_component ( e, position_component ( x,y ) );
     finder->second->build_components ( e, x, y, true );

     return e.handle;
}

int create_item_from_raws ( const string &name )
{
     auto finder = detail::items.find ( name );
     if ( finder == detail::items.end() ) {
          std::cout << "ERROR: Cannot create item of type [" << name << "]\n";
          throw 105;
     }
     
     string item_type = "";
     
     item_component item( item_type );
     item.is_tinted = true;

     entity e = game_engine->ecs->add_entity ();
     // Note: no position is created, that's the caller's responsibility. Since it could be worn/held, in a container
     // or on the landscape, this would become an enormous factory if we try and offer an interface to everything!
     finder->second->build_components ( e, 0, 0 );

     for ( const unique_ptr<base_raw> &i : finder->second->children ) {
	if (i->type == CLOTHING_COLOR) {
	    raw_clothing_color * rwc = static_cast<raw_clothing_color *>(i.get());
	    int color_roll = game_engine->rng.roll_dice(1, rwc->colors.size());
	    string color = rwc->colors[color_roll-1];
	    
	    if (color == "Green") { item.tint_red = 0; item.tint_green = 200; item.tint_blue = 0; }
	    else if (color == "Black") { item.tint_red = 0; item.tint_green = 0; item.tint_blue = 0; }
	    else if (color == "White") { item.tint_red = 200; item.tint_green = 200; item.tint_blue = 200; }
	    else if (color == "Yellow") { item.tint_red = 200; item.tint_green = 200; item.tint_blue = 0; }
	    else if (color == "Brown") { item.tint_red = 165; item.tint_green = 42; item.tint_blue = 42; }
	    else if (color == "Blue") { item.tint_red = 0; item.tint_green = 0; item.tint_blue = 200; }
	    else if (color == "Red") { item.tint_red = 200; item.tint_green = 0; item.tint_blue = 0; }
	    else if (color == "Grey") { item.tint_red = 128; item.tint_green = 128; item.tint_blue = 128; }
	    else if (color == "LightGreen") { item.tint_red = 128; item.tint_green = 200; item.tint_blue = 128; }
	    else if (color == "Khaki") { item.tint_red = 240; item.tint_green = 230; item.tint_blue = 140; }
	    else if (color == "Navy") { item.tint_red = 0; item.tint_green = 0; item.tint_blue = 128; }
	    else { std::cout << "Unknown color: " << color << "\n"; }
	    
	}
     }
     game_engine->ecs->add_component ( e, item );
     
     return e.handle;
}

string to_proper_noun_case ( const std::string &original )
{
     string result;
     result += std::toupper ( original[0] );
     for ( std::size_t i = 1; i<original.size(); ++i ) {
          result += std::tolower ( original[i] );
     }
     return result;
}

pair< string, base_raw * > get_random_starting_profession()
{
     const int max = detail::starting_professions.size();
     const int die_roll = game_engine->rng.roll_dice ( 1,max ) - 1;
     int i = 0;
     for ( auto it = detail::starting_professions.begin(); it != detail::starting_professions.end(); ++it ) {
          if ( i == die_roll ) {
               return std::make_pair( to_proper_noun_case ( it->first ), it->second.get() );
          }
          ++i;
     }
     return std::make_pair("Peasant", nullptr);
}

constexpr int NUMBER_OF_MALE_FIRST_NAMES = 1218;
constexpr int NUMBER_OF_FEMALE_FIRST_NAMES = 4274;
constexpr int NUMBER_OF_LAST_NAMES = 4759;

string line_from_text_file ( const string &filename, const int &line_number )
{
     ifstream file ( filename );
     if ( !file.is_open() ) {
          throw 102;
     }
     string line;
     int i = 0;
     while ( getline ( file,line ) ) {
          if ( i == line_number ) {
               return to_proper_noun_case ( line );
          }
          ++i;
     }
     return "Peasant";
}

string get_random_first_name ( const short int& gender )
{
     if ( gender == gender_t::FEMALE ) {
          return line_from_text_file ( "../raw/first_names_female.txt", game_engine->rng.roll_dice ( 1,NUMBER_OF_FEMALE_FIRST_NAMES )-1 );
     } else {
          return line_from_text_file ( "../raw/first_names_male.txt", game_engine->rng.roll_dice ( 1,NUMBER_OF_MALE_FIRST_NAMES )-1 );
     }
}

string get_random_last_name ()
{
     const int die_roll = game_engine->rng.roll_dice ( 1, NUMBER_OF_LAST_NAMES )-1;
     return line_from_text_file ( "../raw/last_names.txt", die_roll );
}

vector< tuple<string,int,vector<string>>> get_buildables()
{
     vector< tuple<string,int,vector<string>>> result;

     for ( auto it = detail::structures.begin(); it != detail::structures.end(); ++it ) {
          string name;
          int tile_idx = 0;
          const bool is_buildable = it->second->get_buildable ( name, tile_idx );

          if ( is_buildable ) {
               vector<string> tmp = it->second->get_buildable_requirements();
               result.push_back ( std::make_tuple ( name, tile_idx, tmp ) );
          }
     }

     return result;
}

vector<string> get_buildable_requirements ( const string &name )
{
     const vector< tuple<string,int,vector<string>>> buildables = get_buildables();
     for ( const tuple<string,int,vector<string>> &buildable : buildables ) {
          if ( std::get<0> ( buildable ) == name ) {
               return std::get<2> ( buildable );
          }
     }
     return {};
}

vector<base_raw *> get_possible_reactions_for_structure ( const string &structure_name )
{
    vector<base_raw *> result;
    
    for ( auto it = detail::reactions.begin(); it != detail::reactions.end(); ++it ) {
	const bool has_possible_reactions = it->second->has_reactions_for_workshop ( structure_name );
	if ( has_possible_reactions ) result.push_back( it->second.get() );
    }
    
    return result;
}

int get_tile_idx_by_name ( const string& name )
{
    auto finder = detail::tiles.find ( name );
    if ( finder == detail::tiles.end() ) return 0;
    return finder->second;
}


}
