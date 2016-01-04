#pragma once

#include <memory>
#include <utility>
#include "scene_graph.h"

using std::unique_ptr;
using std::pair;

namespace engine {

enum return_mode {
	CONTINUE, PUSH, POP
};

/*
 * Basic game mode to inherit from, for the mode stack system.
 */
class base_mode {
public:
	virtual ~base_mode() {}

	virtual void init() =0;
	virtual void done() =0;
	virtual pair<return_mode, unique_ptr<base_mode>> tick(
			const double time_elapsed) =0;
	virtual void on_push() {
	}

	virtual void on_pop() {
	}

	virtual scene_graph * get_render_target() {
		return nullptr;
	}
};

#define NO_PUSHED_MODE unique_ptr<base_mode>()

}
