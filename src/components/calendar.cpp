#include "calendar.hpp"

std::string calendar_t::get_date_time() const {
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

void calendar_t::next_minute() {
    second += 10;
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