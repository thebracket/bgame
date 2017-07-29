#include "planet.hpp"
#include <cereal/archives/binary.hpp>
#include "../utils/filesystem.hpp"

using namespace rltk;

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
