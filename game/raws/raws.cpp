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

#include "../../engine/globals.h"
#include "../../engine/rng.h"

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
  
}

const string get_index_filename()
{
     return "../raw/index.txt";
}

vector<string> get_files_to_read()
{
     vector<string> result;

     ifstream index ( get_index_filename() );
     if ( !index.is_open() ) throw 101;

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
enum nested_enum {NONE,STRUCTURE,STARTING_PROFESSION,ITEM};
nested_enum nested_type;
bool nested;

/* Lookup Functions */

unsigned char find_glyph_by_name(const string &name) {
    auto finder = detail::glyphs.find(name);
    if (finder == detail::glyphs.end()) {
      std::cout << "Error; could not find glyph; [" << name << "]\n";
      throw 104;
    }
    return finder->second;
}

engine::vterm::color_t find_color_by_name(const string &name) {
    auto finder = detail::colors.find(name);
    if (finder == detail::colors.end()) {
      std::cout << "Error; could not find color; [" << name << "]\n";
      throw 105;
    }
    return finder->second;
}

/* Raw Factory Functions */

void parse_raw_name(const vector<string> &chunks) {
    //std::cout << "Parsing name: " << chunks[1] << "\n";
    current_name = chunks[1];
    unique_ptr<raw_name> name = make_unique<raw_name>(chunks[1]);
    current->children.push_back(std::move(name));
}

void parse_raw_description(const vector<string> &chunks) {
    const string description = chunks[1];
    unique_ptr<raw_description> desc = make_unique<raw_description>(description);
    current->children.push_back(std::move(desc));
}

void parse_raw_obstruction(const vector<string> &chunks) {
    const string view_s = chunks[1];
    const string walk_s = chunks[2];
    
    bool view = false;
    bool walk = false;
    if (view_s == "Y") view = true;
    if (walk_s == "Y") walk = true;
    unique_ptr<raw_obstruction> obs = make_unique<raw_obstruction>(view,walk);
    current->children.push_back(std::move(obs));
}

void parse_raw_glyph(const vector<string> &chunks) {
    const string glyph_name = chunks[1];
    const unsigned char glyph = find_glyph_by_name(glyph_name);
    unique_ptr<raw_glpyh> g = make_unique<raw_glpyh>(glyph);
    current_render->children.push_back(std::move(g));
}

void parse_raw_color_pair(const vector<string> &chunks) {
    const string fg = chunks[1];
    const string bg = chunks[2];
    const color_t fgc = find_color_by_name(fg);
    const color_t bgc = find_color_by_name(bg);
    unique_ptr<raw_color_pair> cp = make_unique<raw_color_pair>(fgc,bgc);
    current_render->children.push_back(std::move(cp));
}

void parse_raw_power_generator(const vector<string> &chunks) {
    const string mode = chunks[1];
    const string amount_i = chunks[2];
    const int amount = std::stoi(amount_i);
    power_generator_condition power_mode;
    if (mode == "daylight") power_mode = DAYLIGHT;
    unique_ptr<raw_power_generator> gen = make_unique<raw_power_generator>(power_mode, amount);
    current->children.push_back(std::move(gen));
}

void parse_raw_power_battery(const vector<string> &chunks) {
    const string capacity_s = chunks[1];
    const int capacity = std::stoi(capacity_s);
    unique_ptr<raw_power_battery> batt = make_unique<raw_power_battery>(capacity);
    current->children.push_back(std::move(batt));
}

/* Specific parser functions */

void parse_structure(const vector<string> &chunks) {
    //std::cout << "Adding " << chunks[0] << " to structure.\n";
    if (chunks[0] == "/STRUCTURE") {
	nested = false;
	nested_type = NONE;
	detail::structures[current_name] = std::move(current);
	current.reset();
	return;
    }
    if (chunks[0] == "/RENDER") {
	current->children.push_back(std::move(current_render));
	current_render.reset();
	return;
    }
    if (chunks[0] == "/SETTLER-ACTION") {
	current->children.push_back(std::move(current_sub_chunk));
	     current_sub_chunk.reset();
	return;
    }
    if (chunks[0] == "/BUILDABLE") {
	current->children.push_back(std::move(current_sub_chunk));
	     current_sub_chunk.reset();
	return;
    }
    if (chunks[0] == "RENDER") {
	current_render = make_unique<raw_renderable>();
	return;
    }
    if (chunks[0] == "SETTLER-ACTION") {
	     current_sub_chunk = make_unique<raw_settler_action>();
	return;
    }
    if (chunks[0] == "BUILDABLE") {
	     current_sub_chunk = make_unique<raw_buildable>();
	return;
    }
    if (chunks[0] == "NAME") {
	parse_raw_name(chunks);
	return;
    }
    if (chunks[0] == "DESCRIPTION") {
	parse_raw_description(chunks);
	return;
    }
    if (chunks[0] == "RENDER_GLYPH") {
	parse_raw_glyph(chunks);
	return;
    }
    if (chunks[0] == "RENDER_COLOR") {
	parse_raw_color_pair(chunks);
	return;
    }
    if (chunks[0] == "OBSTRUCTS") {
	parse_raw_obstruction(chunks);
	return;
    }
    if (chunks[0] == "GENERATOR") {
	parse_raw_power_generator(chunks);
	return;
    }
    if (chunks[0] == "BATTERY") {
	parse_raw_power_battery(chunks);
	return;
    }
    if (chunks[0] == "SA-NAME") {
	raw_settler_action * tmp = static_cast<raw_settler_action *>(current_sub_chunk.get());
	tmp->action_name = chunks[1];
	return;
    }
    if (chunks[0] == "SA-PROVIDES") {
	raw_settler_action * tmp = static_cast<raw_settler_action *>(current_sub_chunk.get());
	tmp->provides = chunks[1];
	tmp->provides_qty = std::stoi(chunks[2]);
	return;
    }
    if (chunks[0] == "SA-POWER_USE") {
	raw_settler_action * tmp = static_cast<raw_settler_action *>(current_sub_chunk.get());
	tmp->power_drain = std::stoi(chunks[1]);
	return;
    }
    if (chunks[0] == "BUILDABLE_COMPONENT") {
	current_sub_chunk->children.push_back( make_unique<raw_component>(chunks[1], std::stoi(chunks[2])));
	return;
    }
    if (chunks[0] == "BUILDABLE_SKILL") {
	current_sub_chunk->children.push_back( make_unique<raw_skill_required>(chunks[1], std::stoi(chunks[2]), std::stoi(chunks[3])));
    	return;
    }
    if (chunks[0] == "SLEEP_HERE") {
	current->children.push_back( make_unique<raw_sleepable>() );
	return;
    }
    
    std::cout << "WARNING: GOT TO END OF STRUCTURE CHUNK READER WITH NO MATCH FOR [" << chunks[0] << "]\n";
}

void parse_starting_profession(const vector<string> &chunks) {
    if (chunks[0] == "/STARTING_PROFESSION") {
	nested = false;
	nested_type = NONE;
	detail::starting_professions[current_name] = std::move(current);
	current.reset();
	return;
    }    
    if (chunks[0] == "NAME") {
	parse_raw_name(chunks);
	return;
    }
}

void parse_item(const vector<string> &chunks) {
    if (chunks[0] == "/ITEM") {
	nested = false;
	nested_type = NONE;
	detail::items[current_name] = std::move(current);
	current.reset();
	return;
    }
    if (chunks[0] == "/RENDER") {
	current->children.push_back(std::move(current_render));
	current_render.reset();
	return;
    }
    if (chunks[0] == "RENDER") {
	current_render = make_unique<raw_renderable>();
	return;
    }
    if (chunks[0] == "NAME") {
	parse_raw_name(chunks);
	return;
    }
    if (chunks[0] == "DESCRIPTION") {
	parse_raw_description(chunks);
	return;
    }
    if (chunks[0] == "RENDER_GLYPH") {
	parse_raw_glyph(chunks);
	return;
    }
    if (chunks[0] == "RENDER_COLOR") {
	parse_raw_color_pair(chunks);
	return;
    }
}

/* Chunk Parsing */

void parse_nested_chunk ( const vector<string> &chunks )
{
    switch (nested_type) {
      case STRUCTURE : parse_structure(chunks); break;
      case STARTING_PROFESSION : parse_starting_profession(chunks); break;
      case ITEM : parse_item(chunks); break;
      case NONE : throw 103;
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
          const unsigned char glyph = static_cast<unsigned char>(glyph_ascii_int);
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
     
     // Tags that start a nested structure
     if (chunks[0] == "STRUCTURE") {
	nested = true;
	nested_type = STRUCTURE;
	current = make_unique<base_raw>();
	return;
     }
     if (chunks[0] == "STARTING_PROFESSION") {
	nested = true;
	nested_type = STARTING_PROFESSION;
	current = make_unique<base_raw>();
	return;
     }
     if (chunks[0] == "ITEM") {
	nested = true;
	nested_type = ITEM;
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

inline bool ignore_char(const char &c) {
  if (c == ' ') return false;
  if (c>40 and c<123 and c != '[' and c != ']') return false;
  return true;
}

void parse_line ( const string &line )
{
     if ( line.empty() ) return; // No parsing of empty lines
     if ( line[0] == '*' ) return; // No parsing of comments

     // Break the line into : separated chunks, ignoring [ and ]
     vector<string> chunks;
     string current_line;
     for ( unsigned int i=0; i<line.size(); ++i ) {
          if ( !ignore_char(line[i]) ) {
               if ( line[i] != ':' ) {
                    current_line += line[i];
               } else {
                    string chunk(current_line);
                    chunks.push_back ( chunk );
                    current_line.clear();
               }
          }
          if (line[i] == ']') chunks.push_back(current_line);
     }
     
     /*std::cout << "Parsed/Chunked Line: ";
     for (const string &s : chunks) {
	std::cout << "[" << s << "] ";
     }
     std::cout<< "\n";*/

     if (!chunks.empty()) {
	parse_chunks ( chunks );
     }
}

void parse_raw_file ( const string &filename )
{
     ifstream file ( filename );
     if ( !file.is_open() ) throw 102;
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

int create_structure_from_raws(const string &name, const int &x, const int &y) {
    auto finder = detail::structures.find(name);
    if (finder == detail::structures.end()) {
      std::cout << "ERROR: Cannot create structure of type [" << name << "]\n";
      throw 105;
    }
  
    entity e;
    e.handle = engine::globals::ecs->get_next_entity_handle();
    engine::globals::ecs->add_entity(e);
    
    engine::globals::ecs->add_component(e, position_component(x,y));
    
    finder->second->build_components(e, x, y);
    
    return e.handle;
}

string to_proper_noun_case(const std::string &original) {
    string result;
    result += std::toupper(original[0]);
    for (std::size_t i = 1; i<original.size(); ++i) {
	result += std::tolower(original[i]);
    }
    return result;
}

string get_random_starting_profession()
{
    const int max = detail::starting_professions.size();
    const int die_roll = engine::roll_dice(1,max) - 1;
    int i = 0;
    for (auto it = detail::starting_professions.begin(); it != detail::starting_professions.end(); ++it) {
	if (i == die_roll) return to_proper_noun_case(it->first);
	++i;
    }
    return "Peasant";
}

constexpr int NUMBER_OF_MALE_FIRST_NAMES = 1218;
constexpr int NUMBER_OF_FEMALE_FIRST_NAMES = 4274;
constexpr int NUMBER_OF_LAST_NAMES = 4759;

string line_from_text_file(const string &filename, const int &line_number) {
    ifstream file ( filename );
    if ( !file.is_open() ) throw 102;
    string line;
    int i = 0;
     while ( getline ( file,line ) ) {
       if (i == line_number) return to_proper_noun_case(line);
       ++i;
    }
    return "Peasant";
}

string get_random_first_name ( const short int& gender )
{
    if (gender == gender_t::FEMALE) {
	return line_from_text_file("../raw/first_names_female.txt", engine::roll_dice(1,NUMBER_OF_FEMALE_FIRST_NAMES)-1);
    } else {
	return line_from_text_file("../raw/first_names_male.txt", engine::roll_dice(1,NUMBER_OF_MALE_FIRST_NAMES)-1);      
    }
}

string get_random_last_name ()
{
    const int die_roll = engine::roll_dice(1, NUMBER_OF_LAST_NAMES)-1;
    return line_from_text_file("../raw/last_names.txt", die_roll);
}


}
