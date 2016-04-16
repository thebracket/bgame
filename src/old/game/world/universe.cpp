#include "universe.hpp"
#include <string>
#include "../tasks/help_wanted.hpp"

using std::string;
using std::fstream;

std::unique_ptr<universe_t> universe = std::make_unique<universe_t>();

void load_universe_state(fstream& lbfile)
{
	lbfile.read(reinterpret_cast<char *>(&universe->globals.camera_handle),
			sizeof(universe->globals.camera_handle));
	lbfile.read(reinterpret_cast<char *>(&universe->globals.cordex_handle),
			sizeof(universe->globals.cordex_handle));
	lbfile.read(reinterpret_cast<char *>(&universe->globals.stored_power),
			sizeof(universe->globals.stored_power));
	lbfile.read(reinterpret_cast<char *>(&universe->globals.starting_system_x),
			sizeof(universe->globals.starting_system_x));
	lbfile.read(reinterpret_cast<char *>(&universe->globals.starting_system_y),
			sizeof(universe->globals.starting_system_y));

	ai::load_help_wanted(lbfile);
}

void save_universe_state(fstream& lbfile)
{
	lbfile.write(
			reinterpret_cast<const char *>(&universe->globals.camera_handle),
			sizeof(universe->globals.camera_handle));
	lbfile.write(
			reinterpret_cast<const char *>(&universe->globals.cordex_handle),
			sizeof(universe->globals.cordex_handle));
	lbfile.write(
			reinterpret_cast<const char *>(&universe->globals.stored_power),
			sizeof(universe->globals.stored_power));
	lbfile.write(
			reinterpret_cast<const char *>(&universe->globals.starting_system_x),
			sizeof(universe->globals.starting_system_x));
	lbfile.write(
			reinterpret_cast<const char *>(&universe->globals.starting_system_y),
			sizeof(universe->globals.starting_system_y));

	ai::save_help_wanted(lbfile);
}

string get_universe_filename()
{
	return string("world/universe.dat");
}

void universe_t::load()
{
	const string filename = get_universe_filename();
	fstream lbfile(filename, std::ios::in | std::ios::binary);
	std::size_t n_systems = 0;
	lbfile.read(reinterpret_cast<char *>(n_systems), sizeof(n_systems));
	for (std::size_t i = 0; i < n_systems; ++i)
	{
		int system_idx = 0;
		lbfile.read(reinterpret_cast<char *>(&system_idx), sizeof(system_idx));

		solar_system_t system;
		system.load(lbfile);
		solar_systems[system_idx] = system;
	}
}

void universe_t::save() const
{
	const string filename = get_universe_filename();
	fstream lbfile(filename, std::ios::out | std::ios::binary);
	std::size_t n_systems = solar_systems.size();
	lbfile.write(reinterpret_cast<const char *>(&n_systems), sizeof(n_systems));
	for (auto it = solar_systems.begin(); it != solar_systems.end(); ++it)
	{
		lbfile.write(reinterpret_cast<const char *>(&it->second.universe_idx),
				sizeof(it->second.universe_idx));

		it->second.save(lbfile);
	}
}
