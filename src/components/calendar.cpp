#include "calendar.hpp"
#include <iomanip>
#include <sstream>

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
    minute+=2;
    if (minute > 59) { minute = 0; ++hour; }
    if (hour > 23) { hour = 0; ++day; }
    if (day > 30) { day = 0; ++month; }
    if (month > 11) { month = 0; ++year; }
}

void calendar_t::to_xml(xml_node * c) {
    component_to_xml(c,
        std::make_pair("year", year),
        std::make_pair("month", month),
        std::make_pair("day", day),
        std::make_pair("hour", hour),
        std::make_pair("minute", minute),
        std::make_pair("second", second),
        std::make_pair("defined_shifts", defined_shifts)
    );
}

void calendar_t::from_xml(xml_node * c) {
    year = c->val<uint16_t>("year");
    month = c->val<uint8_t>("month");
    day = c->val<uint8_t>("day");
    hour = c->val<uint8_t>("hour");
    minute = c->val<uint8_t>("minute");
    second = c->val<uint8_t>("second");
    defined_shifts.clear();
    c->iterate_child("defined_shifts", [this] (xml_node * s) {
        shift_t shift;
        shift.from_xml(s);
        defined_shifts.push_back(shift);
    });
}