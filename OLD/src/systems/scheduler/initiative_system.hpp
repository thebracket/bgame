#pragma once

#include <rltk.hpp>
#include "../../messages/messages.hpp"

class initiative_system : public rltk::mailbox_system<tick_message> {
public:
    initiative_system() { system_name = "Initiative"; }
    void on_message(const tick_message &msg);
};
