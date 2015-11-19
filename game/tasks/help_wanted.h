#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>

using std::fstream;
using std::vector;
using std::string;
using std::unordered_map;

namespace ai {

enum job_type_t { BUILD_STRUCTURE };
enum job_step_t { MOVE_TO, PICK_UP, CONSTRUCT_WITH_SKILL, CONVERT_PLACEHOLDER_STRUCTURE };

int get_next_job_id();

struct job_step {
     job_step_t type;
     int target_x = 0;
     int target_y = 0;
     bool requires_skill = false;
     string skill_name;
     int placeholder_structure_id;
};

struct job {
     job_type_t type;
     int job_id;
     vector<job_step> steps;
     int current_step = 0;
     int assigned_to = 0;
};

extern unordered_map<int, job> jobs_board;

void save_help_wanted ( fstream& lbfile );
void load_help_wanted ( fstream& lbfile );
}