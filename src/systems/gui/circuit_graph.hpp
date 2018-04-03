#pragma once

namespace systems
{
	namespace circuit_graph
	{
		void build_nodes_list();
		void restore_state();
		void display_circuit_graph(bool * show_window);
		void sync_node_list_to_ecs();
	}
}
