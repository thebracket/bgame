#include "planet.hpp"
#include <fstream>
#include <rltk.hpp>
#include <iostream>
#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>

using namespace rltk;

const std::string planet_filename = "world/planet.dat";

void save_planet(const planet_t &planet) {
	std::fstream lbfile(planet_filename, std::ios::out | std::ios::binary);

    cereal::XMLOutputArchive oarchive(lbfile);
    oarchive(planet);
}

planet_t load_planet() {
	planet_t planet;

	std::fstream lbfile(planet_filename, std::ios::in | std::ios::binary);
    cereal::XMLInputArchive iarchive(lbfile);
    iarchive(planet);

	return planet;
}
