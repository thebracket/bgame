#include "settler_drop_tool.hpp"
#include "../../messages/inventory_changed_message.hpp"

namespace tasks {

void drop_current_tool(const entity_t &e, settler_ai_t &ai, const position_t &pos) {
	if (ai.current_tool == 0) return;
	emit(drop_item_message{ ai.current_tool, pos.x, pos.y, pos.z });
	ai.current_tool = 0;
}

}