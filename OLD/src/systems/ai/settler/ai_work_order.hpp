#pragma once

#include <rltk.hpp>

class ai_work_order : public rltk::base_system {
public:
    ai_work_order() { system_name = "AI work - work orders"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
