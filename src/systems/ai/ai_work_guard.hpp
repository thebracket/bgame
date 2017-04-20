#pragma once

#include <rltk.hpp>
#include "ai_work_template.hpp"
#include "../../components/ai_tag_work_guarding.hpp"

class ai_work_guard : public ai_work_template<ai_tag_work_guarding> {
public:
    ai_work_guard() { system_name = "AI work - guard"; }
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
};
