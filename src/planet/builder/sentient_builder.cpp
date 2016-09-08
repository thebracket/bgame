#include "sentient_builder.hpp"
#include "../../raws/raws.hpp"
#include "../../components/components.hpp"

void create_sentient(const int x, const int y, const int z, rltk::random_number_generator &rng, planet_t &planet, region_t &region, const std::size_t person_id) 
{
    species_t species;

    species.tag = planet.civs.unimportant_people[person_id].species_tag;
    auto species_finder = species_defs.find(species.tag);
    if (species_finder == species_defs.end()) {
        std::cout << "WARNING: Could not find raws for [" << species.tag << "]\n";
        return;
    }
    if (planet.civs.unimportant_people[person_id].male) {
        species.gender = MALE;
    } else {
        species.gender = FEMALE;
    }

    //std::cout << species_finder->second.glyph << "\n";
    auto sentient = create_entity()
        ->assign(position_t{x,y,z})
        ->assign(name_t{ species_finder->second.name, OCCUPATION_NAMES[planet.civs.unimportant_people[person_id].occupation] })
        ->assign(renderable_t{ species_finder->second.glyph ,rltk::colors::WHITE, rltk::colors::BLACK })
        ->assign(viewshed_t{ 8, false });

    /*auto sentient = create_entity()
		->assign(position_t{ x,y,z })
		->assign(renderable_t{ species_finder->second.glyph ,rltk::colors::WHITE, rltk::colors::BLACK })
		->assign(name_t{ first_name, last_name })
		->assign(std::move(species))
		->assign(std::move(health))
		->assign(std::move(stats))
		->assign(std::move(ai))
		->assign(viewshed_t{ 8, false });
    */
}
