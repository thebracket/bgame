#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/unordered_set.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/array.hpp>

#include "planet.hpp"
#include "../bengine/filesystem.hpp"

const std::string planet_filename = get_save_path() + std::string("/planet.dat");

void save_planet(const planet_t &planet) {
	std::fstream lbfile(planet_filename, std::ios::out | std::ios::binary);

    cereal::BinaryOutputArchive oarchive(lbfile);
    oarchive(planet);
}

planet_t load_planet() {
	planet_t planet;

	std::fstream lbfile(planet_filename, std::ios::in | std::ios::binary);
    cereal::BinaryInputArchive iarchive(lbfile);
    iarchive(planet);

	return planet;
}
