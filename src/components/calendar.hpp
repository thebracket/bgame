#pragma once

#include <rltk.hpp>
#include <string>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>
#include "helpers/shift.hpp"

using namespace rltk;

struct calendar_t {
	uint16_t year = 2525;
	uint8_t month = 0;
	uint8_t day = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;
	uint8_t second = 0;

	std::vector<shift_t> defined_shifts;

	std::string get_date_time() const;
	void next_minute();

	inline float sun_arc_percent() { 
		if (hour < 12) {
			return (hour/12.0F) + ((minute/60.0F)/1000.0F);
		} else {
			return ((24.0F-hour)/12.0F) - ((minute/60.0F)/1000.0F);
		}
	}

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( year, month, day, hour, minute, second, defined_shifts ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<calendar_t>>)