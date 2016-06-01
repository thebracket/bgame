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
	uint16_t month = 0;
	uint16_t day = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;

	std::vector<shift_t> defined_shifts;

	std::string get_date_time() const {
		std::string result;
		switch (month) {
			case 0 : result += "January"; break;
			case 1 : result += "Febuary"; break;
			case 2 : result += "March"; break;
			case 3 : result += "April"; break;
			case 4 : result += "May"; break;
			case 5 : result += "June"; break;
			case 6 : result += "July"; break;
			case 7 : result += "August"; break;
			case 8 : result += "September"; break;
			case 9 : result += "October"; break;
			case 10 : result += "November"; break;
			case 11 : result += "December"; break;
		}
		result += " " + std::to_string(day+1) + ", " + std::to_string(hour) + ":" + std::to_string(minute);
		return result;
	}

	void next_minute() {
		++minute;
		if (minute > 60) { minute = 0; ++hour; }
		if (hour > 23) { hour = 0; ++day; }
		if (day > 30) { day = 0; ++month; }
		if (month > 11) { month = 0; ++year; }
	}

	void save(std::ostream &lbfile) {
		serialize(lbfile, year);
		serialize(lbfile, month);
		serialize(lbfile, day);
		serialize(lbfile, hour);
		serialize(lbfile, minute);
		std::size_t n_shifts = defined_shifts.size();
		serialize(lbfile, n_shifts);
		for (int j=0; j<n_shifts; ++j) {
			serialize(lbfile, defined_shifts[j].shift_name);
			for (int i=0; i<24; ++i) {
				serialize(lbfile, defined_shifts[j].hours[i]);
			}
		}
	}

	static calendar_t load(std::istream &lbfile) {
		calendar_t c;
		deserialize(lbfile, c.year);
		deserialize(lbfile, c.month);
		deserialize(lbfile, c.day);
		deserialize(lbfile, c.hour);
		deserialize(lbfile, c.minute);
		std::size_t n_shifts;
		deserialize(lbfile, n_shifts);
		for (std::size_t i=0; i<n_shifts; ++i) {
			shift_t shift;
			deserialize(lbfile, shift.shift_name);
			for (int j=0; j<24; ++j) {
				deserialize(lbfile, shift.hours[j]);
			}
			c.defined_shifts.push_back(shift);
		}
		return c;
	}
};