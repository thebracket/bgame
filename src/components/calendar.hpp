#pragma once

#include "../stdafx.h"
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/polymorphic.hpp>
#include "helpers/shift.hpp"
#include "../bengine/ecs_impl.hpp"

struct calendar_t {
	uint16_t year = 2525;
	uint8_t month = 0;
	uint8_t day = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;
	uint8_t second = 0;
	float sun_x = 0.0f;
	float sun_y = 0.0f;
	float sun_z = 0.0f;
	float sun_r = 1.0f;
	float sun_g = 1.0f;
	float sun_b = 1.0f;
	float moon_x = 0.0f;
	float moon_y = 0.0f;
	float moon_z = 0.0f;
	float moon_r = 68.0f / 255.0f;
	float moon_g = 128.0f / 255.0f;
	float moon_b = 178.0f / 255.0f;

	std::vector<shift_t> defined_shifts;

	std::string get_date_time() const;
	void next_minute();

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( year, month, day, hour, minute, second, defined_shifts ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(bengine::impl::component_store_t<bengine::impl::component_t<calendar_t>>)