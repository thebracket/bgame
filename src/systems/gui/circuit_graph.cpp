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

		enum class node_type_t { UNKNOWN, LEVER, PRESSURE_PLATE, OSCILLATOR, FLOAT_GAUGE, BRIDGE, DOOR, SPIKES, AND_GATE, OR_GATE, SUPPORT, PROXIMITY_SENSOR };
		static bool node_dragging = false;
		static ImVec2 node_drag_start;
		static int node_drag_id;
		static int node_destination_id = 0;

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

			std::vector<int> outbound_connections;

			bool on = true;
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
			case node_type_t::SUPPORT: return "(Support)";
			default: return "";
			}
		}

		static std::vector<node_t> nodes;
		static int next_x = 200;
		static float offset = 100;

		static void insert_node(entity_t &e, name_t &n, position_t &p)
		{
			node_t node{ e.id, n.first_name + std::string(" #") + std::to_string(e.id) };

			const auto outputs = e.component<sends_signal_t>();
			if (outputs)
			{
				for (const auto &o : outputs->targets)
				{
					node.outbound_connections.emplace_back(o);
				}
			}

			const auto pos = e.component<node_graph_position_t>();
			if (pos)
			{
				node.x = pos->x;
				node.y = pos->y;
			}
			else
			{
				node.x = next_x;
				node.y = 100;
				e.assign(node_graph_position_t{ next_x, 100 });
				next_x += 108;
			}
			node.world_x = p.x;
			node.world_y = p.y;
			node.world_z = p.z;

			if (e.component<proximity_sensor_t>()) node.type = node_type_t::PROXIMITY_SENSOR;
			if (e.component<lever_t>()) node.type = node_type_t::LEVER;
			if (e.component<pressure_plate_t>()) node.type = node_type_t::PRESSURE_PLATE;
			if (e.component<float_gauge_t>()) node.type = node_type_t::FLOAT_GAUGE;
			if (e.component<oscillator_t>()) node.type = node_type_t::OSCILLATOR;
			if (e.component<bridge_t>()) node.type = node_type_t::BRIDGE;
			if (e.component<construct_door_t>()) node.type = node_type_t::DOOR;
			const auto b = e.component<building_t>();
			if (b && b->tag == "spike_trap") node.type = node_type_t::SPIKES;
			const auto sp = e.component<signal_processor_t>();
			if (sp)
			{
				if (sp->processor == AND) node.type = node_type_t::AND_GATE;
				if (sp->processor == OR) node.type = node_type_t::OR_GATE;
			}
			if (e.component<construct_support_t>()) node.type = node_type_t::SUPPORT;

			switch (node.type)
			{
			case node_type_t::LEVER: { node.has_input = false; node.has_output = true; } break;
			case node_type_t::PRESSURE_PLATE: { node.has_input = false; node.has_output = true; } break;
			case node_type_t::OSCILLATOR: { node.has_input = false; node.has_output = true; } break;
			case node_type_t::FLOAT_GAUGE: { node.has_input = false; node.has_output = true; } break;
			case node_type_t::BRIDGE: { node.has_input = true; node.has_output = false; } break;
			case node_type_t::DOOR: { node.has_input = true; node.has_output = false; } break;
			case node_type_t::SPIKES: { node.has_input = true; node.has_output = false; } break;
			case node_type_t::AND_GATE: { node.has_input = true; node.has_output = true; } break;
			case node_type_t::OR_GATE: { node.has_input = true; node.has_output = true; } break;
			case node_type_t::SUPPORT: { node.has_input = true; node.has_output = false; } break;
			case node_type_t::PROXIMITY_SENSOR: { node.has_input = false; node.has_output = true; } break;
			default: {}
			}

			const auto sender = e.component<sends_signal_t>();
			const auto receiver = e.component<receives_signal_t>();
			if (sender) {
				node.on = sender->active;
			} else if (receiver)
			{
				node.on = receiver->active;
			} else
			{
				node.on = false;
			}

			nodes.emplace_back(node);
		}

		void build_nodes_list()
		{
			std::set<int> nodes_included;
			nodes.clear();
			node_dragging = false;
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

		static void draw_hermite(ImDrawList* draw_list, const ImVec2 p1, const ImVec2 p2, const int n_steps, const ImColor color = { 200, 200, 100 })
		{
			const auto t1 = ImVec2(+80.0f, 0.0f);
			const auto t2 = ImVec2(+80.0f, 0.0f);

			for (auto step = 0; step <= n_steps; step++)
			{
				const auto t = static_cast<float>(step) / static_cast<float>(n_steps);
				const auto h1 = +2 * t*t*t - 3 * t*t + 1.0f;
				const auto h2 = -2 * t*t*t + 3 * t*t;
				const auto h3 = t * t*t - 2 * t*t + t;
				const auto h4 = t * t*t - t * t;
				draw_list->PathLineTo(ImVec2(h1*p1.x + h2 * p2.x + h3 * t1.x + h4 * t2.x, h1*p1.y + h2 * p2.y + h3 * t1.y + h4 * t2.y));
			}

			draw_list->PathStroke(color, false, 3.0f);
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
				ImGui::SetCursorPos(ImVec2{ n.x + offset - (text_size.x / 2.0f), n.y + text_size.y + text_size.y + (offset/2.0f) });
				const std::string is_active = n.on ? "ON" : "OFF";
				const ImVec4 active_color = n.on ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
				ImGui::TextColored(active_color, "%s", is_active.c_str());

				const ImVec2 node_pos{ static_cast<float>(n.x) + offset, static_cast<float>(n.y) + offset };
				const ImVec2 node_size{ std::max(100.0f, text_size.x), 100.0f };
				const ImVec2 node_br{ node_pos.x + node_size.x, node_pos.y + node_size.y };

				draw_list->ChannelsSetCurrent(0); // Background

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
				auto output_active = false;
				n.input_pos = node_pos;
				n.input_pos.y += 80;
				n.input_pos.x += 8;
				n.output_pos = node_pos;
				n.output_pos.y += 80;
				n.output_pos.x += 92;
				if (n.has_input)
				{
					ImVec2 nop = n.input_pos;
					nop.x -= 8.0f;
					nop.y -= 8.0f;
					if (ImGui::GetIO().MousePos.x > nop.x && ImGui::GetIO().MousePos.x < nop.x+16.0f && ImGui::GetIO().MousePos.y > nop.y && ImGui::GetIO().MousePos.y < nop.y+16.0f)
					{
						draw_list->AddCircleFilled(n.input_pos, 8.0f, ImColor(255, 255, 0));
						if (node_dragging) node_destination_id = n.id;
					} else
					{
						draw_list->AddCircleFilled(n.input_pos, 8.0f, ImColor(192, 192, 0));
					}
				}
				if (n.has_output)
				{
					ImVec2 nop = n.output_pos;
					nop.x -= 8.0f;
					nop.y -= 8.0f;
					ImGui::SetCursorScreenPos(nop);
					ImGui::InvisibleButton("nodeout", ImVec2{16.0f, 16.0f});
					if (ImGui::IsItemHovered())
					{
						output_active = true;
						draw_list->AddCircleFilled(n.output_pos, 8.0f, ImColor(255, 0, 255));

						if (ImGui::GetIO().MouseDown[0])
						{
							if (!node_dragging) {
								node_dragging = true;
								node_drag_start = n.output_pos;
								node_drag_id = n.id;
							}							
						} else
						{
							if (ImGui::GetIO().MouseDown[1] || is_key_down(GLFW_KEY_DELETE))
							{
								n.outbound_connections.clear();
							}
							node_dragging = false;
						}
					}
					else {
						draw_list->AddCircleFilled(n.output_pos, 8.0f, ImColor(192, 0, 192));
					}
				}
				
				// Dragging

				if (node_dragging)
				{
					draw_hermite(draw_list, node_drag_start, ImGui::GetIO().MousePos, 12);

					if (!ImGui::GetIO().MouseDown[0])
					{
						// We're no longer dragging
						node_dragging = false;
						if (node_destination_id > 0)
						{
							for (auto &node : nodes)
							{
								if (node.id == node_drag_id)
								{
									node.outbound_connections.emplace_back(node_destination_id);
								}
							}
						}
					}
				}
				else {
					ImGui::SetCursorScreenPos(node_pos);
					ImGui::InvisibleButton("node", node_size);
					if (!output_active && ImGui::IsItemActive() && ImGui::IsMouseDragging(0))
					{
						const auto delta = ImGui::GetIO().MouseDelta;
						n.x += delta.x;
						n.y += delta.y;
					}
				}

				// Cleanup
				ImGui::PopID();
			}

			// Render connections
			for (const auto &n : nodes)
			{
				for (const auto &start : n.outbound_connections)
				{
					const auto start_pos = n.output_pos;
					ImVec2 end_pos;
					for (const auto &end : nodes)
					{
						if (end.id == start) end_pos = end.input_pos;
					}

					draw_hermite(draw_list, start_pos, end_pos, 12, ImColor(255, 255, 255));
				}
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

		void sync_node_list_to_ecs()
		{
			// Clear all existing connections
			for (const auto &n : nodes)
			{
				const auto e = entity(n.id);
				const auto sender = e->component<sends_signal_t>();
				const auto receiver = e->component<receives_signal_t>();
				if (sender) sender->targets.clear();
				if (receiver) receiver->receives_from.clear();
			}

			// Add all links back from the node graph
			for (const auto &n : nodes)
			{
				for (const auto outbound_id : n.outbound_connections)
				{
					const auto sender = entity(n.id);
					const auto receiver = entity(outbound_id);
					const auto s = sender->component<sends_signal_t>();
					const auto r = receiver->component<receives_signal_t>();
					s->targets.emplace_back(outbound_id);
					r->receives_from.emplace_back(std::make_tuple(n.id, 0, 0, 0, 0));
				}
			}
		}
	}
}
