#pragma once

#include <rltk.hpp>

class ai_work_lever_pull : public rltk::base_system {
public:
    ai_work_lever_pull() { system_name = "AI work - levers"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
