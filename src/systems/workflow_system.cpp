#include "workflow_system.hpp"
#include "../messages/messages.hpp"

void workflow_system::update(const double duration_ms) {
}

void workflow_system::configure() {
    subscribe<update_workflow_message>([this] (update_workflow_message &msg) {
		dirty = true;
	});
}
