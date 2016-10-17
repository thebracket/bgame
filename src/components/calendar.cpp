#include "calendar.hpp"
#include <iomanip>
#include <sstream>
#include "../utils/serialization_wrapper.hpp"

std::string calendar_t::get_date_time() const {
    std::stringstream result;
    switch (month) {
        case 0 : result << "Jan"; break;
        case 1 : result << "Feb"; break;
        case 2 : result << "Mar"; break;
        case 3 : result << "Apr"; break;
        case 4 : result << "May"; break;
        case 5 : result << "Jun"; break;
        case 6 : result << "Jul"; break;
        case 7 : result << "Aug"; break;
        case 8 : result << "Sep"; break;
        case 9 : result << "Oct"; break;
        case 10 : result << "Nov"; break;
        case 11 : result << "Dec"; break;
    }
    result << " " << std::setfill('0') << std::setw(2) << (day+1) << " " <<
        std::setfill('0') << std::setw(2) << +hour << ":" << std::setfill('0') << std::setw(2) << +minute;
    return result.str();
}

void calendar_t::next_minute() {
    second += 30;
    if (second > 60) { second = 0; ++minute; }
    if (minute > 60) { minute = 0; ++hour; }
    if (hour > 23) { hour = 0; ++day; }
    if (day > 30) { day = 0; ++month; }
    if (month > 11) { month = 0; ++year; }
}

void calendar_t::save(std::ostream &lbfile) {
    Serialize(lbfile, year, month, day, hour, minute, second, defined_shifts);
}

calendar_t calendar_t::load(std::istream &lbfile) {
    calendar_t c;
    Deserialize(lbfile, c.year, c.month, c.day, c.hour, c.minute, c.second, c.defined_shifts);

    return c;
}