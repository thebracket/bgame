#pragma once

#include <array>
#include <istream>
#include <ostream>
#include <rltk.hpp>

enum shift_type_t { SLEEP_SHIFT, LEISURE_SHIFT, WORK_SHIFT };

struct shift_t {
	std::string shift_name;
	std::array<shift_type_t, 24> hours;
    void to_xml(rltk::xml_node * c);
    void from_xml(rltk::xml_node * c);
};
