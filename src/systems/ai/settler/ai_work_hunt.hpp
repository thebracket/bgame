#pragma once

#include <rltk.hpp>

namespace jobs_board {
    bool butcher_exist();
}

class ai_work_hunt  : public rltk::base_system {
public:
    ai_work_hunt() { system_name = "AI work - hunting"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
