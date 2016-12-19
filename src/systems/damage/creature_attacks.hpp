#pragma once

#include <rltk.hpp>
#include "../../messages/messages.hpp"

class creature_attacks_system : public rltk::mailbox_system<creature_attack_message> {
public:
    creature_attacks_system() { system_name = "Creature Attacks"; }
    void on_message(const creature_attack_message &msg);
};
