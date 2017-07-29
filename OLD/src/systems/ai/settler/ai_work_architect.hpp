#pragma once

#include <rltk.hpp>

class ai_work_architect : public rltk::base_system {
public:
    ai_work_architect() { system_name = "AI work - architect"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
