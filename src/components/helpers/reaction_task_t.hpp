#pragma once

#include <vector>
#include <utility>
#include <string>

struct reaction_task_t {
	reaction_task_t() = default;
    reaction_task_t(const int id, const std::string &name, const std::string &tag, const std::vector<std::pair<int,bool>> comps) : building_id(id), job_name(name), reaction_tag(tag), components(comps) {}

    int building_id;
    std::string job_name;
	std::string reaction_tag;
    std::vector<std::pair<int,bool>> components;
};
