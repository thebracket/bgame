#pragma once

#include <rltk.hpp>
#include "../components/position.hpp"
#include "../../messages/messages.hpp"

class grazer_ai_system : public rltk::mailbox_system<action_available_message> {
public:
    grazer_ai_system() { system_name = "Grazer AI"; }
	virtual void on_message(const action_available_message &msg) override final;
    void wander_randomly(rltk::entity_t &entity, position_t &original);
};
