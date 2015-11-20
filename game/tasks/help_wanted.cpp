#include "help_wanted.h"

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

void save_help_wanted ( fstream& lbfile ) {};
void load_help_wanted ( fstream& lbfile ) {};

}