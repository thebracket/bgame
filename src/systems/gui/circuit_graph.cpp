#include "circuit_graph.hpp"
#include "bengine/btabs.hpp"
#include <set>
#include "global_assets/game_ecs.hpp"
#include "global_assets/game_camera.hpp"
#include "render_engine/camera.hpp"

namespace systems
{
	namespace circuit_graph
	{
		using namespace bengine;

		enum class node_type_t { UNKNOWN, LEVER, PRESSURE_PLATE, OSCILLATOR, FLOAT_GAUGE, BRIDGE, DOOR, SPIKES };

		struct node_t
		{
			int id;
			std::string name;
			int world_x, world_y, world_z;
			int x, y;
			node_type_t type = node_type_t::UNKNOWN;
			bool has_input = false;
			bool has_output = false;

			ImVec2 input_pos;
			ImVec2 output_pos;
		};

		static std::string get_node_type(const node_t &n)
		{
			switch (n.type)
			{
			case node_type_t::UNKNOWN: return "";
			case node_type_t::LEVER: return "(Lever)";
			case node_type_t::PRESSURE_PLATE: return "(Pressure Plate)";
			case node_type_t::OSCILLATOR: return "(Oscillator)";
			case node_type_t::FLOAT_GAUGE: return "(Float Gauge)";
			case node_type_t::BRIDGE: return "(Bridge)";
			case node_type_t::DOOR: return "(Door)";
			case node_type_t::SPIKES: return "(Spikes)";
			default: return "";
			}
		}

		static std::vector<node_t> nodes;
		static int next_x = 200;
		static float offset = 100;

		static void insert_node(entity_t &e, name_t &n, position_t &p)
		{
			node_t node{ e.id, n.first_name + std::string(" #") + std::to_string(e.id) };
			const auto pos = e.component<node_graph_position_t>();
			if (pos)
			{
				node.x = pos->x;
				node.y = pos->y;
			} else
			{
				node.x = next_x;
				node.y = 100;
				e.assign(node_graph_position_t{ next_x, 100 });
				next_x += 108;
			}
			node.world_x = p.x;
			node.world_y = p.y;
			node.world_z = p.z;

			if (e.component<lever_t>()) node.type = node_type_t::LEVER;
			if (e.component<pressure_plate_t>()) node.type = node_type_t::PRESSURE_PLATE;
			if (e.component<float_gauge_t>()) node.type = node_type_t::FLOAT_GAUGE;
			if (e.component<oscillator_t>()) node.type = node_type_t::OSCILLATOR;
			if (e.component<bridge_t>()) node.type = node_type_t::BRIDGE;
			if (e.component<construct_door_t>()) node.type = node_type_t::DOOR;
			const auto b = e.component<building_t>();
			if (b && b->tag == "spike_trap") node.type = node_type_t::SPIKES;

			switch (node.type)
			{
			case node_type_t::LEVER: { node.has_input = false; node.has_output = true; } break;
			case node_type_t::PRESSURE_PLATE: { node.has_input = false; node.has_output = true; } break;
			case node_type_t::OSCILLATOR: { node.has_input = false; node.has_output = true; } break;
			case node_type_t::FLOAT_GAUGE: { node.has_input = false; node.has_output = true; } break;
			case node_type_t::BRIDGE: { node.has_input = true; node.has_output = false; } break;
			case node_type_t::DOOR: { node.has_input = true; node.has_output = false; } break;
			case node_type_t::SPIKES: { node.has_input = true; node.has_output = false; } break;
			default: {}
			}

			nodes.emplace_back(node);
		}

		void build_nodes_list()
		{
			std::set<int> nodes_included;
			nodes.clear();
			next_x = 0;

			each<sends_signal_t, name_t, position_t>([&nodes_included](entity_t &e, sends_signal_t &s, name_t &n, position_t &p)
			{
				const auto finder = nodes_included.find(e.id);
				if (finder == nodes_included.end()) {
					nodes_included.insert(e.id);

					insert_node(e, n, p);
				}
			});

			each<receives_signal_t, name_t, position_t>([&nodes_included](entity_t &e, receives_signal_t &s, name_t &n, position_t &p)
			{
				const auto finder = nodes_included.find(e.id);
				if (finder == nodes_included.end()) {
					nodes_included.insert(e.id);

					insert_node(e, n, p);
				}
			});
		}

		void restore_state()
		{
			for (const auto &n : nodes)
			{
				auto e = entity(n.id);

				// Update node position
				const auto gpos = e->component<node_graph_position_t>();
				if (gpos) {
					gpos->x = n.x;
					gpos->y = n.y;
				}
			}
		}

		static void render_nodes()
		{
			ImGui::SameLine();
			ImGui::BeginGroup();

			auto draw_list = ImGui::GetWindowDrawList();
			draw_list->ChannelsSplit(2);

			for (auto &n : nodes)
			{
				ImGui::PushID(n.id);
				draw_list->ChannelsSetCurrent(1); // Background
				const auto text_size = ImGui::CalcTextSize(n.name.c_str());
				ImGui::SetCursorPos(ImVec2{ n.x + offset - (text_size.x / 2.0f), static_cast<float>(n.y) + (offset/2.0f) });
				ImGui::Text("%s", n.name.c_str());
				ImGui::SetCursorPos(ImVec2{ n.x + offset - (text_size.x / 2.0f), n.y + text_size.y + (offset/2.0f) });
				ImGui::TextColored(ImVec4(0.0f, 0.7f, 0.7f, 0.7f), "%s", get_node_type(n).c_str());

				const ImVec2 node_pos{ static_cast<float>(n.x) + offset, static_cast<float>(n.y) + offset };
				const ImVec2 node_size{ std::max(100.0f, text_size.x), 100.0f };
				const ImVec2 node_br{ node_pos.x + node_size.x, node_pos.y + node_size.y };

				draw_list->ChannelsSetCurrent(0); // Background

				ImGui::SetCursorScreenPos(node_pos);
				ImGui::InvisibleButton("node", node_size);
				const auto hovered = ImGui::IsItemHovered();
				if (hovered)
				{
					camera_position->region_x = n.world_x;
					camera_position->region_y = n.world_y;
					camera_position->region_z = n.world_z;
					render::camera_moved = true;
				}
				const ImU32 node_bg_color = hovered ? ImColor(0, 0, 75) : ImColor(0, 0, 60);
				draw_list->AddRectFilled(node_pos, node_br, node_bg_color, 4.0f);
				draw_list->AddRect(node_pos, node_br, ImColor(100, 100, 100), 4.0f);

				// Inputs & outputs
				n.input_pos = node_pos;
				n.input_pos.y += 80;
				n.input_pos.x += 8;
				n.output_pos = node_pos;
				n.output_pos.y += 80;
				n.output_pos.x += 92;
				if (n.has_input)
				{
					draw_list->AddCircleFilled(n.input_pos, 8.0f, ImColor(255, 255, 0));
				}
				if (n.has_output)
				{
					draw_list->AddCircleFilled(n.output_pos, 8.0f, ImColor(255, 0, 255));
				}
				
				// Dragging
				if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
				{
					const auto delta = ImGui::GetIO().MouseDelta;
					n.x += delta.x;
					n.y += delta.y;
				}

				// Cleanup
				ImGui::PopID();
			}

			draw_list->ChannelsMerge();
			ImGui::EndGroup();
		}

		void display_circuit_graph(bool * show_window)
		{
			bengine::begin_info_window("Circuit Management", show_window);
			render_nodes();
			ImGui::End();
		}
	}
}
