#pragma once

#include <rltk.hpp>
#include <string>
#include <array>
#include <vector>

using namespace rltk;

enum shift_type_t { SLEEP_SHIFT, LEISURE_SHIFT, WORK_SHIFT };

struct shift_t {
	std::string shift_name;
	std::array<shift_type_t, 24> hours;
};

struct calendar_t {
	std::size_t serialization_identity = 3;

	uint16_t year = 2525;
	uint8_t month = 0;
	uint8_t day = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;
	uint8_t second = 0;

	std::vector<shift_t> defined_shifts;

	std::string get_date_time() const;
	void next_minute();
	void save(std::ostream &lbfile);
	static calendar_t load(std::istream &lbfile);
};