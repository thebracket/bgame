#pragma once

#include <rltk.hpp>
#include "../../messages/messages.hpp"

class kill_system : public rltk::mailbox_system<entity_slain_message> {
public:
    kill_system() { system_name = "Kill System"; }
    void on_message(const entity_slain_message &msg) override final;
};

