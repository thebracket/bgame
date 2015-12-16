#include "help_wanted.h"
#include "../components/component_types.h"

using namespace serialization_generic;

namespace ai {

namespace ai_detail {
int next_job_id = 1;
}

int get_next_job_id()
{
     ++ai_detail::next_job_id;
     return ai_detail::next_job_id;
}

unordered_map<int, job_t> jobs_board;

void save_help_wanted ( fstream& lbfile ) {
     save_primitive<int>( lbfile, ai_detail::next_job_id );
     save_primitive<int>( lbfile, jobs_board.size() );
     for (auto it = jobs_board.begin(); it != jobs_board.end(); ++it) {
	save_primitive<int>( lbfile, it->second.type );
	save_primitive<int>( lbfile, it->second.job_id );
	save_primitive<int>( lbfile, it->second.current_step );
	save_primitive<int>( lbfile, it->second.assigned_to );
	save_primitive<int>( lbfile, it->second.steps.size() );
	for (auto steps = it->second.steps.begin(); steps != it->second.steps.end(); ++steps) {
	    save_primitive<int>( lbfile, steps->type );
	    save_primitive<int16_t>( lbfile, steps->target_x );
	    save_primitive<int16_t>( lbfile, steps->target_y );
	    save_primitive<uint8_t>( lbfile, steps->target_z );
	    save_primitive<int>( lbfile, steps->component_id );
	    save_primitive<bool>( lbfile, steps->requires_skill );
	    save_primitive<string>( lbfile, steps->skill_name );
	    save_primitive<int>( lbfile, steps->placeholder_structure_id );
	    save_primitive<char>( lbfile, steps->required_skill_difficulty );
	}
     }
}

void load_help_wanted ( fstream& lbfile ) {
     load_primitive<int>( lbfile, ai_detail::next_job_id );
     int number_of_jobs = 0;
     load_primitive<int>( lbfile, number_of_jobs );
     for (int i=0; i<number_of_jobs; ++i) {
	int job_type, job_id, current_step, assigned_to, number_of_steps;
	load_primitive <int> ( lbfile, job_type );
	load_primitive <int> ( lbfile, job_id );
	load_primitive <int> ( lbfile, current_step );
	load_primitive <int> ( lbfile, assigned_to );
	load_primitive <int> ( lbfile, number_of_steps );
	job_t job{ static_cast<job_type_e>(job_type), job_id, {}, current_step, assigned_to };
	for (int j=0; j<number_of_steps; ++j) {
	    int step_type, component_id, placeholder_structure_id;
	    bool requires_skill;
	    std::string skill_name;
	    char required_skill_difficulty;
	    int16_t target_x, target_y;
	    uint8_t target_z;
	    load_primitive<int>( lbfile, step_type );
	    load_primitive<int16_t>( lbfile, target_x );
	    load_primitive<int16_t>( lbfile, target_y );
	    load_primitive<uint8_t>( lbfile, target_z );
	    load_primitive<int>( lbfile, component_id );
	    load_primitive<bool>( lbfile, requires_skill );
	    load_primitive<string>( lbfile, skill_name );
	    load_primitive<int>( lbfile, placeholder_structure_id );
	    load_primitive<char>( lbfile, required_skill_difficulty );
	    job.steps.push_back( job_step_t{ static_cast<job_step_e>(step_type), target_x, target_y, target_z, component_id, requires_skill, skill_name, placeholder_structure_id, required_skill_difficulty } );
	}
	jobs_board [ job_id ] = job;
     }
}

}