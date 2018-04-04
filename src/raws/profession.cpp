#include "profession.hpp"
#include "lua_bridge.hpp"
#include "clothing.hpp"
#include "defs/profession_t.hpp"
#include "../utils/system_log.hpp"

std::vector<profession_t> starting_professions;

profession_t * get_random_profession(bengine::random_number_generator &rng) noexcept {
	const auto number_of_professions = starting_professions.size();
	const auto selected_profession = rng.roll_dice(1, static_cast<int>(number_of_professions) -1);
	return &starting_professions[selected_profession];
}

void sanity_check_professions() noexcept
{
    for (const auto &prof : starting_professions) {
        if (prof.name.empty()) glog(log_target::LOADER, log_severity::warning, "WARNING: Profession with no name");
        for (const auto &cloth : prof.starting_clothes) {
            if (std::get<0>(cloth) > 3) glog(log_target::LOADER, log_severity::warning, "WARNING: {0} clothing item has invalid gender tag\n", prof.name);
			if (std::get<1>(cloth) != "head" && std::get<1>(cloth) != "torso" && std::get<1>(cloth) != "legs" && std::get<1>(cloth) != "shoes") {
				glog(log_target::LOADER, log_severity::warning, "WARNING: {0} has an invalid slot {1}", prof.name, std::get<1>(cloth));
			}
            const auto finder = get_clothing_by_tag(std::get<2>(cloth));
			if (!finder) {
				glog(log_target::LOADER, log_severity::warning, "WARNING: {0} has non-existent clothing type: {1}", prof.name, std::get<2>(cloth));
			}
        }
    }
}

void read_professions() noexcept
{

    std::string tag;
    profession_t p;

    read_lua_table("starting_professions",
                   [&p, &tag] (const auto &key) { tag = key; p = profession_t{}; },
                   [&p] (const auto &key) { starting_professions.push_back(p); },
                   lua_parser{
                           {"name", [&p] () { p.name = lua_str(); }},
                           {"modifiers", [&p] ()     {
                               read_stat_modifiers("modifiers", p);
                           }},
                           {"clothing", [&p] ()      {
                               lua_pushstring(lua_state, "clothing");
                               lua_gettable(lua_state, -2);
                               while (lua_next(lua_state, -2) != 0) {
                                   const std::string gender_specifier = lua_tostring(lua_state, -2);
                                   lua_pushstring(lua_state, gender_specifier.c_str());
                                   lua_gettable(lua_state, -2);
                                   while (lua_next(lua_state, -2) != 0) {
                                       const std::string slot = lua_tostring(lua_state, -2);
                                       const std::string item = lua_tostring(lua_state, -1);
                                       //std::cout << p.name << " - " << slot << ", " << item << "\n";
                                       int gender_tag = 0;
                                       if (gender_specifier == "male") gender_tag = 1;
                                       if (gender_specifier == "female") gender_tag = 2;
                                       p.starting_clothes.emplace_back( std::make_tuple(gender_tag, slot, item));
                                       lua_pop(lua_state, 1);
                                   }
                                   lua_pop(lua_state, 1);
                               }
                           }}
                   }
    );
}
