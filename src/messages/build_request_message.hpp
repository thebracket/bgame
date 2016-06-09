#pragma once

#include <rltk.hpp>
#include "../components/position.hpp"

struct build_request_message : public rltk::base_message_t {
	build_request_message() {}
};
