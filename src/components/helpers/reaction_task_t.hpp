#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include <rltk.hpp>

struct reaction_task_t {
    reaction_task_t() {}
    reaction_task_t(const std::size_t id, const std::string name, const std::string tag, std::vector<std::pair<std::size_t,bool>> comps) : building_id(id), job_name(name), reaction_tag(tag), components(comps) {}

    std::size_t building_id;
    std::string job_name;
	std::string reaction_tag;
    std::vector<std::pair<std::size_t,bool>> components;

    void to_xml(rltk::xml_node * c);
    void from_xml(rltk::xml_node * c);
};
