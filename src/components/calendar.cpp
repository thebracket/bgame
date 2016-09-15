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
    second += 30;
    if (second > 60) { second = 0; ++minute; }
    if (minute > 60) { minute = 0; ++hour; }
    if (hour > 23) { hour = 0; ++day; }
    if (day > 30) { day = 0; ++month; }
    if (month > 11) { month = 0; ++year; }
}

void calendar_t::save(std::ostream &lbfile) {
    serialize(lbfile, year);
    serialize(lbfile, month);
    serialize(lbfile, day);
    serialize(lbfile, hour);
    serialize(lbfile, minute);
    serialize(lbfile, second);
    std::size_t n_shifts = defined_shifts.size();
    serialize(lbfile, n_shifts);
    for (int j=0; j<n_shifts; ++j) {
        serialize(lbfile, defined_shifts[j].shift_name);
        for (int i=0; i<24; ++i) {
            serialize(lbfile, defined_shifts[j].hours[i]);
        }
    }
}

calendar_t calendar_t::load(std::istream &lbfile) {
    calendar_t c;
    deserialize(lbfile, c.year);
    deserialize(lbfile, c.month);
    deserialize(lbfile, c.day);
    deserialize(lbfile, c.hour);
    deserialize(lbfile, c.minute);
    deserialize(lbfile, c.second);
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