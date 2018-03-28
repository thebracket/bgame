#include "node_editor.hpp"
#include <cstdint>
#include <algorithm>
#include "main_window.hpp"
#include "../global_assets/game_camera.hpp"
#include "../render_engine/camera.hpp"

const float NODE_SLOT_RADIUS = 5.0f;
const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

static ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return { lhs.x + rhs.x, lhs.y + rhs.y }; }
static ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return { lhs.x - rhs.x, lhs.y - rhs.y }; }

static uint32_t s_id = 0;

static struct node_graph::node_type_t s_node_types[] =
{
	// Machinery Input Nodes
	{
		"Input",
		// Levers have no inputs
		{
		},
		// Output
		{
			{ "On/Off", node_graph::CONNECTION_TYPE_FLOAT },
		},
	},
	// Machinery Processor Nodes
	{
		"AND",
		// Levers have no inputs
		{
			{ "Input1", node_graph::CONNECTION_TYPE_FLOAT },
			{ "Input2", node_graph::CONNECTION_TYPE_FLOAT },
		},
		// Output
		{
			{ "On/Off", node_graph::CONNECTION_TYPE_FLOAT },
		},
	},
	{
		"OR",
		{
			{ "Input1", node_graph::CONNECTION_TYPE_FLOAT },
			{ "Input2", node_graph::CONNECTION_TYPE_FLOAT },
		},
		{
			{ "On/Off", node_graph::CONNECTION_TYPE_FLOAT },
		},
	},
	{
		"NOT",
		{
			{ "Input1", node_graph::CONNECTION_TYPE_FLOAT },
		},
		{
			{ "On/Off", node_graph::CONNECTION_TYPE_FLOAT },
		},
	},
	{
		"NAND",
		{
			{ "Input1", node_graph::CONNECTION_TYPE_FLOAT },
			{ "Input2", node_graph::CONNECTION_TYPE_FLOAT },
		},
		{
			{ "On/Off", node_graph::CONNECTION_TYPE_FLOAT },
		},
	},
	{
		"NOR",
		{
			{ "Input1", node_graph::CONNECTION_TYPE_FLOAT },
			{ "Input2", node_graph::CONNECTION_TYPE_FLOAT },
		},
		{
			{ "On/Off", node_graph::CONNECTION_TYPE_FLOAT },
		},
	},
	{
		"ENOR",
		{
			{ "Input1", node_graph::CONNECTION_TYPE_FLOAT },
			{ "Input2", node_graph::CONNECTION_TYPE_FLOAT },
		},
		{
			{ "On/Off", node_graph::CONNECTION_TYPE_FLOAT },
		},
	},
	// Machinery Output Nodes
	{
		"Output",
		// Levers have no inputs
		{
			{ "On/Off", node_graph::CONNECTION_TYPE_FLOAT },
		},
		{
		},
	},
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Math

	{
		"Multiply",
		// Input connections
		{
			{ "Input1", node_graph::CONNECTION_TYPE_FLOAT },
			{ "Input2", node_graph::CONNECTION_TYPE_FLOAT },
		},
		// Output
		{
			{ "Out", node_graph::CONNECTION_TYPE_FLOAT },
		},
	},

	{
		"Add",
		// Input connections
		{
			{ "Input1", node_graph::CONNECTION_TYPE_FLOAT },
			{ "Input2", node_graph::CONNECTION_TYPE_FLOAT },
		},
		// Output
		{
			{ "Out", node_graph::CONNECTION_TYPE_FLOAT },
		},
	},
};

static void setup_connections(std::vector<std::unique_ptr<node_graph::connection_t>> &connections, std::vector<node_graph::connection_description_t> &connection_descs)
{
	for (const auto &desc : connection_descs)
	{
		auto con = std::make_unique<node_graph::connection_t>();
		con->desc = desc;

		connections.emplace_back(std::move(con));
	}
}

std::unique_ptr<node_graph::node_t> create_node_from_type(const ImVec2 pos, node_graph::node_type_t* node_type)
{
	auto node = std::make_unique<node_graph::node_t>();
	node->id = s_id++;
	node->name = node_type->name;

	auto title_size = ImGui::CalcTextSize(node->name.c_str());
	title_size.y *= 3;

	setup_connections(node->input_connections, node_type->input_connections);
	setup_connections(node->output_connections, node_type->output_connections);

	// Calculate the size needed for the whole box

	ImVec2 input_text_size(0.0f, 0.0f);

	for (auto &c : node->input_connections)
	{
		const auto text_size = ImGui::CalcTextSize(c->desc.name.c_str());
		input_text_size.x = std::max<float>(text_size.x, input_text_size.x);

		c->pos = ImVec2(0.0f, title_size.y + input_text_size.y + text_size.y / 2.0f);

		input_text_size.y += text_size.y;
		input_text_size.y += 4.0f;		// size between text entries
	}

	input_text_size.x += 40.0f;

	// max text size + 40 pixels in between

	const auto x_start = input_text_size.x;

	// Calculate for the outputs

	for (auto &c : node->output_connections)
	{
		const auto text_size = ImGui::CalcTextSize(c->desc.name.c_str());
		input_text_size.x = std::max<float>(x_start + text_size.x, input_text_size.x);
		input_text_size.y += text_size.y;
		input_text_size.y += 4.0f;		// size between text entries
	}

	node->pos = pos;
	node->size.x = input_text_size.x;
	node->size.y = input_text_size.y + title_size.y;

	input_text_size.y = 0.0f;

	// set the positions for the output nodes when we know where the place them

	for (auto &c : node->output_connections)
	{
		const auto text_size = ImGui::CalcTextSize(c->desc.name.c_str());

		c->pos = ImVec2(node->size.x, title_size.y + input_text_size.y + text_size.y / 2.0f);

		input_text_size.y += text_size.y;
		input_text_size.y += 4.0f;		// size between text entries
	}

	// calculate the size of the node depending on nuber of connections

	return node;
}

std::unique_ptr<node_graph::node_t> create_node_from_name(const ImVec2 pos, const char* name)
{
	for (auto &node : s_node_types)
	{
		if (!strcmp(node.name.c_str(), name))
			return create_node_from_type(pos, &node);
	}

	return nullptr;
}

enum drag_state_t { DRAG_STATE_DEFAULT, DRAG_STATE_HOVER, DRAG_STATE_BEGIN_DRAG, DRAG_STATE_DRAGGING, DRAG_STATE_CONNECT };

struct drag_node_t
{
	ImVec2 pos{ 0.0,0.0 };
	node_graph::connection_t* con = nullptr;
};

static drag_node_t s_drag_node;
static drag_state_t s_drag_state = DRAG_STATE_DEFAULT;

//static std::vector<std::unique_ptr<node_graph::node_t>> all_nodes;

void drawHermite(ImDrawList* draw_list, const ImVec2 p1, const ImVec2 p2, const int n_steps)
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

	draw_list->PathStroke(ImColor(200, 200, 100), false, 3.0f);
}

static bool is_connector_hovered(node_graph::connection_t* c, const ImVec2 offset)
{
	const auto mouse_pos = ImGui::GetIO().MousePos;
	const auto con_pos = offset + c->pos;

	const auto xd = mouse_pos.x - con_pos.x;
	const auto yd = mouse_pos.y - con_pos.y;

	return ((xd * xd) + (yd *yd)) < (NODE_SLOT_RADIUS * NODE_SLOT_RADIUS);
}

static node_graph::connection_t* getHoverCon(ImVec2 offset, ImVec2* pos, std::vector<std::unique_ptr<node_graph::node_t>> &all_nodes)
{
	for (auto &node : all_nodes)
	{
		const auto node_pos = node->pos + offset;

		for (auto &con : node->input_connections)
		{
			if (is_connector_hovered(con.get(), node_pos))
			{
				*pos = node_pos + con->pos;
				return con.get();
			}
		}

		for (auto &con : node->output_connections)
		{
			if (is_connector_hovered(con.get(), node_pos))
			{
				*pos = node_pos + con->pos;
				return con.get();
			}
		}
	}

	s_drag_node.con = nullptr;
	return nullptr;
}

void update_dragging(const ImVec2 offset, std::vector<std::unique_ptr<node_graph::node_t>> &all_nodes)
{
	switch (s_drag_state)
	{
	case DRAG_STATE_DEFAULT:
	{
		ImVec2 pos;
		const auto con = getHoverCon(offset, &pos, all_nodes);

		if (con)
		{
			s_drag_node.con = con;
			s_drag_node.pos = pos;
			s_drag_state = DRAG_STATE_HOVER;
			return;
		}

		break;
	}

	case DRAG_STATE_HOVER:
	{
		ImVec2 pos;
		const auto con = getHoverCon(offset, &pos, all_nodes);

		// Make sure we are still hovering the same node

		if (con != s_drag_node.con)
		{
			s_drag_node.con = nullptr;
			s_drag_state = DRAG_STATE_DEFAULT;
			return;
		}

		if (ImGui::IsMouseClicked(0) && s_drag_node.con)
			s_drag_state = DRAG_STATE_DRAGGING;

		break;
	}

	case DRAG_STATE_BEGIN_DRAG:
	{
		break;
	}

	case DRAG_STATE_DRAGGING:
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->ChannelsSetCurrent(0); // Background

		drawHermite(drawList, s_drag_node.pos, ImGui::GetIO().MousePos, 12);

		if (!ImGui::IsMouseDown(0))
		{
			ImVec2 pos;
			const auto con = getHoverCon(offset, &pos, all_nodes);

			// Make sure we are still hovering the same node

			if (con == s_drag_node.con)
			{
				s_drag_node.con = nullptr;
				s_drag_state = DRAG_STATE_DEFAULT;
				return;
			}

			// Lets connect the nodes.
			// TODO: Make sure we connect stuff in the correct way!

			con->input = s_drag_node.con;
			s_drag_node.con = nullptr;
			s_drag_state = DRAG_STATE_DEFAULT;
		}

		break;
	}

	case DRAG_STATE_CONNECT:
	{
		break;
	}
	}
}

static void display_node(ImDrawList* draw_list, ImVec2 offset, node_graph::node_t* node, int& node_selected)
{
	auto node_hovered_in_scene = -1;

	ImGui::PushID(node->id);
	const auto node_rect_min = offset + node->pos;

	// Display node contents first
	draw_list->ChannelsSetCurrent(1); // Foreground
	const auto old_any_active = ImGui::IsAnyItemActive();

	// Draw title in center

	auto text_size = ImGui::CalcTextSize(node->name.c_str());

	auto pos = node_rect_min + NODE_WINDOW_PADDING;
	pos.x = node_rect_min.x + (node->size.x / 2) - text_size.x / 2;

	ImGui::SetCursorScreenPos(pos);
	//ImGui::BeginGroup(); // Lock horizontal position
	ImGui::Text("%s", node->name.c_str());
	//ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
	//float dummy_color[3] = { node->Pos.x / ImGui::GetWindowWidth(), node->Pos.y / ImGui::GetWindowHeight(), fmodf((float)node->ID * 0.5f, 1.0f) };
	//ImGui::ColorEdit3("##color", &dummy_color[0]);

	// Save the size of what we have emitted and weither any of the widgets are being used
	const auto node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
	//node->size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
	const auto node_rect_max = node_rect_min + node->size;

	// Display node box
	draw_list->ChannelsSetCurrent(0); // Background

	ImGui::SetCursorScreenPos(node_rect_min);
	ImGui::InvisibleButton("node", node->size);

	if (ImGui::IsItemHovered())
	{
		node_hovered_in_scene = node->id;

		camera_position->region_x = node->world_x;
		camera_position->region_y = node->world_y;
		camera_position->region_z = node->world_z;
		render::camera_moved = true;
	}

	auto node_moving_active = false;

	if (ImGui::IsItemActive() && !s_drag_node.con)
		node_moving_active = true;

	const ImU32 node_bg_color = node_hovered_in_scene == node->id ? ImColor(75, 75, 75) : ImColor(60, 60, 60);
	draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);

	auto title_area = node_rect_max;
	title_area.y = node_rect_min.y + 30.0f;

	// Draw text bg area
	draw_list->AddRectFilled(node_rect_min + ImVec2(1, 1), title_area, ImColor(100, 0, 0), 4.0f);
	draw_list->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);

	for (auto &con : node->input_connections)
	{
		ImGui::SetCursorScreenPos(offset + ImVec2(10.0f, 0));
		ImGui::Text("%s", con->desc.name.c_str());

		auto con_color = ImColor(150, 150, 150);

		if (is_connector_hovered(con.get(), node_rect_min))
			con_color = ImColor(200, 200, 200);

		draw_list->AddCircleFilled(node_rect_min + con->pos, NODE_SLOT_RADIUS, con_color);

		offset.y += text_size.y + 2.0f;
	}

	offset = node_rect_min;
	offset.y += 40.0f;

	for (auto &con : node->output_connections)
	{
		text_size = ImGui::CalcTextSize(con->desc.name.c_str());

		ImGui::SetCursorScreenPos(offset + ImVec2(con->pos.x - (text_size.x + 10.0f), 0));
		ImGui::Text("%s", con->desc.name.c_str());

		auto con_color = ImColor(150, 150, 150);

		if (is_connector_hovered(con.get(), node_rect_min))
			con_color = ImColor(200, 200, 200);

		draw_list->AddCircleFilled(node_rect_min + con->pos, NODE_SLOT_RADIUS, con_color);

		offset.y += text_size.y + 2.0f;
	}


	//for (int i = 0; i < node->outputConnections.size(); ++i)
	//	drawList->AddCircleFilled(offset + node->outputSlotPos(i), NODE_SLOT_RADIUS, ImColor(150,150,150,150));

	if (node_widgets_active || node_moving_active)
		node_selected = node->id;

	if (node_moving_active && ImGui::IsMouseDragging(0))
		node->pos = node->pos + ImGui::GetIO().MouseDelta;

	//ImGui::EndGroup();

	ImGui::PopID();
}

node_graph::node_t* find_node_by_con(node_graph::connection_t * find_con, std::vector<std::unique_ptr<node_graph::node_t>> &all_nodes)
{
	for (auto &node : all_nodes)
	{
		for (auto &con : node->input_connections)
		{
			if (con.get() == find_con)
				return node.get();
		}

		for (auto &con : node->output_connections)
		{
			if (con.get() == find_con)
				return node.get();
		}
	}

	return nullptr;
}

void render_lines(ImDrawList* draw_list, ImVec2 offset, std::vector<std::unique_ptr<node_graph::node_t>> &all_nodes)
{
	for (auto &node : all_nodes)
	{
		for (auto &con : node->input_connections)
		{
			if (!con->input)
				continue;

			const auto target_node = find_node_by_con(con->input, all_nodes);

			if (!target_node)
				continue;

			drawHermite(draw_list,
				offset + target_node->pos + con->input->pos,
				offset + node->pos + con->pos,
				12);
		}
	}
}

void ShowExampleAppCustomNodeGraph(bool* opened, std::vector<std::unique_ptr<node_graph::node_t>> &all_nodes)
{
	int screen_w, screen_h;
	glfwGetWindowSize(bengine::main_window, &screen_w, &screen_h);

	ImGui::SetNextWindowSize(ImVec2(screen_w - 100.0f, screen_h - 100.0f), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Circuitry", opened))
	{
		ImGui::End();
		return;
	}

	bool open_context_menu = false;
	int node_hovered_in_list = -1;
	int node_hovered_in_scene = -1;

	static int node_selected = -1;
	static ImVec2 scrolling = ImVec2(0.0f, 0.0f);

#if 0	

	static ImVector<Node> nodes;
	static ImVector<NodeLink> links;
	static bool inited = false;
	static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
	static int node_selected = -1;
	if (!inited)
	{
		nodes.push_back(Node(0, "MainTex", ImVec2(40, 50), 0.5f, 1, 1));
		nodes.push_back(Node(1, "BumpMap", ImVec2(40, 150), 0.42f, 1, 1));
		nodes.push_back(Node(2, "Combine", ImVec2(270, 80), 1.0f, 2, 2));
		links.push_back(NodeLink(0, 0, 2, 0));
		links.push_back(NodeLink(1, 0, 2, 1));
		inited = true;
	}

	// Draw a list of nodes on the left side
	ImGui::BeginChild("node_list", ImVec2(100, 0));
	ImGui::Text("Nodes");
	ImGui::Separator();
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node->ID);
		if (ImGui::Selectable(node->Name, node->ID == node_selected))
			node_selected = node->ID;
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_list = node->ID;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		ImGui::PopID();
	}
	ImGui::EndChild();
#endif
	ImGui::SameLine();
	ImGui::BeginGroup();

	// Create our child canvas
	//ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.15f, 0.15f, 0.15f, 0.5f));
	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);


	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->ChannelsSplit(2);
	//ImVec2 offset = ImGui::GetCursorScreenPos() - scrolling;

	//displayNode(draw_list, scrolling, s_emittable, node_selected);
	//displayNode(draw_list, scrolling, s_emitter, node_selected);

	for (auto &node : all_nodes) {
		display_node(draw_list, scrolling, node.get(), node_selected);
	}

	update_dragging(scrolling, all_nodes);
	render_lines(draw_list, scrolling, all_nodes);

	draw_list->ChannelsMerge();

	// Scrolling
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
		scrolling = scrolling - ImGui::GetIO().MouseDelta;

	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::EndGroup();

	ImGui::End();
}

node_graph::node_t * find_node_by_entity_id(const int &id, std::vector<std::unique_ptr<node_graph::node_t>> &all_nodes)
{
	for (const auto &node : all_nodes)
	{
		if (node->entity_id == id) return node.get();
	}
	return nullptr;
}