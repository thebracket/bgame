#pragma once

#include <rltk.hpp>

class ai_idle : public rltk::base_system {
public:
    ai_idle() { system_name = "AI idle mode"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
