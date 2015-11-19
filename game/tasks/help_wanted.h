#pragma once

#include <fstream>

using std::fstream;

namespace ai {
  
  struct job {
    // Job ID
    // Job Type
    // List of Steps
    // Current Step
    // Assigned To
  };
  
  struct job_step {
    // Step Type
    // Location
    // Skill if any
  };
  
  void save_help_wanted( fstream& lbfile ) {};
  void load_help_wanted( fstream& lbfile ) {};
  
  // map of jobs
}