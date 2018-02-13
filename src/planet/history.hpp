#pragma once

#include <fstream>
#include <vector>
#include <unordered_map>

struct life_event_t {
	life_event_t() {}
	life_event_t(const int &YEAR, const std::string &TYPE) : year(YEAR), type(TYPE) {}
	int year = 0;
	std::string type = "";

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(year, type);
	}
};

struct history_t {
	std::unordered_map<std::size_t, std::vector<life_event_t>> settler_life_events;

	template<class Archive>
	void serialize(Archive & archive)
	{
		archive(settler_life_events);
	}
};
