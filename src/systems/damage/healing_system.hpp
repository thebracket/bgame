#pragma once

#include <rltk.hpp>
#include "../../messages/messages.hpp"

class healing_system : public rltk::mailbox_system<hour_elapsed_message> {
public:
    healing_system() { system_name = "Healing"; }
    virtual void on_message(const hour_elapsed_message &msg) override final;
};
