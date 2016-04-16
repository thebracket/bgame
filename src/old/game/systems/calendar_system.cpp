#include "calendar_system.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "../world/universe.hpp"
#include "../game.h"

using std::string;
using std::vector;
using std::stringstream;

namespace calendar_detail
{

enum season_t
{
	WINTER = 0, SPRING = 1, SUMMER = 2, AUTUMN = 3
};
const vector<string> months{ "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
const vector<int> days_in_month{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
const vector<season_t> season_months{ WINTER, WINTER, SPRING, SPRING, SPRING, SUMMER, SUMMER, SUMMER, AUTUMN, AUTUMN, AUTUMN, WINTER };
const vector<string> season_names{ "Winter", "Spring", "Summer", "Autumn" };
constexpr int MONTHS_PER_YEAR = 12;
constexpr int MINUTES_PER_HOUR = 60;
constexpr int HOURS_PER_DAY = 24;
constexpr int DAYS_PER_YEAR = 365;

// world::system_time defines the current time, in number of seconds since January 1, 2525.

}

void calendar_system::advance_calendar(calendar_component * time)
{
	time->system_time++;
	time->minute++;

	if (time->minute >= calendar_detail::MINUTES_PER_HOUR - 1)
	{
		time->minute = 0;
		++time->hour;

		if (time->hour >= calendar_detail::HOURS_PER_DAY)
		{
			time->hour = 0;
			++time->day;

			if (time->day >= calendar_detail::days_in_month[time->month])
			{
				time->day = 0;
				++time->month;

				if (time->month >= calendar_detail::MONTHS_PER_YEAR)
				{
					time->month = 0;
					++time->year;
				}
			}
		}
	}
}

void calendar_system::update_display_time(calendar_component* t)
{
	stringstream day;
	day << calendar_detail::months[t->month] << " " << (t->day + 1) << ", "
			<< (t->year + 2525);
	stringstream time;
	if (t->hour < 10)
		time << "0";
	time << +(t->hour) << ":";
	if (t->minute < 8)
		time << "0";
	time << (t->minute + 2);

	const string season = calendar_detail::season_names[calendar_detail::season_months[t->month]];
	t->display_day_month = day.str();
	t->display_time = time.str();
	t->display_season = season;
}

void calendar_system::calculate_sun_angle(calendar_component* t) const
{
	if ( t->hour < 6 or t->hour > 20 ) {
		t->is_daytime = false;
		t->sun_x = 0;
		return;
	}

	const int hours_after_dawn = t->hour - 6;
	const float minute_fraction = t->minute / 60.0F;
	const float day_fraction = (hours_after_dawn + minute_fraction) / 13.0F;
	const float sun_x = 0.0F -( (768.0F * day_fraction) - 384.0F );
	t->sun_x = std::floor(sun_x);
	t->is_daytime = true;
}

void calendar_system::tick(const double &duration_ms)
{
	calendar_component * calendar = ECS->find_entity_component<calendar_component>(universe->globals.cordex_handle);
	for (command_message &cmd : *game_engine->messaging->get_messages_by_type<command_message>())
	{
		if (!cmd.deleted and cmd.command == TOGGLE_PAUSE)
		{
			universe->globals.paused = !universe->globals.paused;
			cmd.deleted = true;
		}
	}

	if (universe->globals.paused)
	{
		update_display_time(calendar);
		return;
	}

	calendar->duration_buffer += duration_ms;
	if (calendar->duration_buffer > TICK_LENGTH)
	{
		calendar->duration_buffer = 0.0;
		advance_calendar(calendar);
		update_display_time(calendar);
		calculate_sun_angle(calendar);
	}
}
