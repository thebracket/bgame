#pragma once

#include <rltk.hpp>
#include "../../messages/messages.hpp"

class turret_ranged_attack_system : public rltk::mailbox_system<turret_ranged_attack_message> {
public:
    turret_ranged_attack_system() { system_name = "Turret Ranged"; }
    virtual void on_message(const turret_ranged_attack_message &msg) override final;
};

