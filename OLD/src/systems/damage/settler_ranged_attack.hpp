#pragma once

#include <rltk.hpp>
#include "../../messages/messages.hpp"

class settler_ranged_attack_system : public rltk::mailbox_system<settler_ranged_attack_message> {
public:
    settler_ranged_attack_system() { system_name = "Settler Ranged"; }
    virtual void on_message(const settler_ranged_attack_message &msg) override final;
};

