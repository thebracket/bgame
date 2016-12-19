#pragma once

#include <rltk.hpp>
#include "../../messages/messages.hpp"

class settler_melee_attacks_system : public rltk::mailbox_system<settler_attack_message> {
public:
    settler_melee_attacks_system() { system_name = "Settler Melee"; }
    virtual void on_message(const settler_attack_message &msg) override final;
};
