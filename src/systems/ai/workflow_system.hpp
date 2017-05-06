#pragma once

#include <rltk.hpp>
#include <memory>
#include "../../components/settler_ai.hpp"
#include "../../components/designations.hpp"
#include "../../components/ai_tags/ai_tag_work_order.hpp"

class workflow_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    bool dirty = true;
};

bool is_auto_reaction_task_available(const settler_ai_t &ai);
std::unique_ptr<reaction_task_t> find_automatic_reaction_task(const ai_tag_work_order &ai);
std::unique_ptr<reaction_task_t> find_queued_reaction_task(const ai_tag_work_order &ai);
void free_workshop(const std::size_t &id);
