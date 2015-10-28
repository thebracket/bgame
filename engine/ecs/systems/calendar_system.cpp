#include "calendar_system.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "../../../game/world/world.h"
#include "../../globals.h"

using std::string;
using std::vector;
using std::stringstream;

namespace engine {
namespace ecs {

namespace calendar_detail {

enum season_t {WINTER=0, SPRING=1, SUMMER=2, AUTUMN=3};
const vector<string> months {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
const vector<int> days_in_month {31,28,31,30,31,30,31,31,30,31,30,31};
const vector<season_t> season_months {WINTER, WINTER, SPRING, SPRING, SPRING, SUMMER, SUMMER, SUMMER, AUTUMN, AUTUMN, AUTUMN, WINTER};
const vector<string> season_names {"Winter", "Spring", "Summer", "Autumn" };
constexpr int MONTHS_PER_YEAR = 12;
constexpr int MINUTES_PER_HOUR = 60;
constexpr int HOURS_PER_DAY = 24;
constexpr int DAYS_PER_YEAR = 365;

// world::system_time defines the current time, in number of seconds since January 1, 2525.


}

void calendar_system::advance_calendar ( calendar_component * time )
{
     time->system_time++;
     time->minute++;
     
     if (time->minute >= calendar_detail::MINUTES_PER_HOUR) {
	time->minute = 0;
	++time->hour;
	
	if (time->hour >= calendar_detail::HOURS_PER_DAY) {
	    time->hour = 0;
	    ++time->day;
	    
	    if (time->day >= calendar_detail::days_in_month[time->month]) {
		time->day = 0;
		++time->month;
		
		if (time->month >= calendar_detail::MONTHS_PER_YEAR) {
		    time->month = 0;
		    ++time->year;
		}
	    }
	}
     }
}

void calendar_system::update_display_time ( const calendar_component* t )
{
    stringstream day;
    day << calendar_detail::months[t->month] << " " << (t->day+1) << ", " << t->year;
    stringstream time;
    if (t->hour < 8) time << "0";
    time << (t->hour+1) << ":";
    if (t->minute < 8) time << "0";
    time << (t->minute+2);
  
    const string season = calendar_detail::season_names[calendar_detail::season_months[t->month]];
    world::display_day_month = day.str();
    world::display_time = time.str();
    world::display_season = season;
}

float calendar_system::calculate_sun_angle ( const calendar_component* t ) const
{
    // TODO: Vary by season!
    const int hour = t->hour+1;
    switch (hour) {
      case 5 : return 10.0F;
      case 6 : return 20.0F;
      case 7 : return 40.0F;
      case 8 : return 50.0F;
      case 9 : return 60.0F;
      case 10 : return 70.0F;
      case 11 : return 80.0F;
      case 12 : return 90.0F;
      case 13 : return 100.0F;
      case 14 : return 110.0F;
      case 15 : return 120.0F;
      case 16 : return 130.0F;
      case 17 : return 140.0F;
      case 18 : return 160.0F;
      case 19 : return 170.0F;
      default : return 0.0F;
    }
}


void calendar_system::tick ( const double &duration_ms )
{
     entity * cordex = engine::globals::ecs->get_entity_by_handle ( world::cordex_handle );
     int calendar_handle = cordex->find_component_by_type ( calendar );
     calendar_component * calendar = engine::globals::ecs->get_component_by_handle<calendar_component> ( calendar_handle );

     calendar->duration_buffer += duration_ms;
     if ( calendar->duration_buffer > TICK_LENGTH ) {
          calendar->duration_buffer = 0.0;
	  advance_calendar(calendar);
	  update_display_time(calendar);
          world::sun_angle = calculate_sun_angle(calendar);
          if ( world::sun_angle > 180.0F ) world::sun_angle = 0.0F;
     }
}

}
}