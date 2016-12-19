#pragma once

#include <rltk.hpp>
#include "../messages/messages.hpp"

class damage_system : public rltk::mailbox_system<inflict_damage_message> {
public:
	damage_system() { system_name = "Damage"; }
	void on_message(const inflict_damage_message &msg);
};
