#include "node_editor.hpp"
#include <cstdint>
#include <cstdio>
#include <algorithm>

#define sizeof_array(t) (sizeof(t) / sizeof(t[0]))
const float NODE_SLOT_RADIUS = 5.0f;
const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }

static uint32_t s_id = 0;

static struct node_graph::node_type_t s_nodeTypes[] =
{
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

static void setupConnections(std::vector<node_graph::connection_t*>& connections, std::vector<node_graph::connection_description_t> &connectionDescs)
{
	for (int i = 0; i < connectionDescs.size(); ++i)
	{
		const node_graph::connection_description_t& desc = connectionDescs[i];

		node_graph::connection_t* con = new node_graph::connection_t;
		con->desc = desc;

		connections.push_back(con);
	}
}

static std::unique_ptr<node_graph::node_t> createNodeFromType(ImVec2 pos, node_graph::node_type_t* nodeType)
{
	auto node = std::make_unique<node_graph::node_t>();
	node->id = s_id++;
	node->name = nodeType->name;

	ImVec2 titleSize = ImGui::CalcTextSize(node->name.c_str());

	titleSize.y *= 3;

	setupConnections(node->input_connections, nodeType->input_connections);
	setupConnections(node->output_connections, nodeType->output_connections);

	// Calculate the size needed for the whole box

	ImVec2 inputTextSize(0.0f, 0.0f);
	ImVec2 outputText(0.0f, 0.0f);

	for (node_graph::connection_t* c : node->input_connections)
	{
		ImVec2 textSize = ImGui::CalcTextSize(c->desc.name.c_str());
		inputTextSize.x = std::max<float>(textSize.x, inputTextSize.x);

		c->pos = ImVec2(0.0f, titleSize.y + inputTextSize.y + textSize.y / 2.0f);

		inputTextSize.y += textSize.y;
		inputTextSize.y += 4.0f;		// size between text entries
	}

	inputTextSize.x += 40.0f;

	// max text size + 40 pixels in between

	float xStart = inputTextSize.x;

	// Calculate for the outputs

	for (node_graph::connection_t* c : node->output_connections)
	{
		ImVec2 textSize = ImGui::CalcTextSize(c->desc.name.c_str());
		inputTextSize.x = std::max<float>(xStart + textSize.x, inputTextSize.x);
	}

	node->pos = pos;
	node->size.x = inputTextSize.x;
	node->size.y = inputTextSize.y + titleSize.y;

	inputTextSize.y = 0.0f;

	// set the positions for the output nodes when we know where the place them

	for (node_graph::connection_t* c : node->output_connections)
	{
		ImVec2 textSize = ImGui::CalcTextSize(c->desc.name.c_str());

		c->pos = ImVec2(node->size.x, titleSize.y + inputTextSize.y + textSize.y / 2.0f);

		inputTextSize.y += textSize.y;
		inputTextSize.y += 4.0f;		// size between text entries
	}

	// calculate the size of the node depending on nuber of connections

	return node;
}

std::unique_ptr<node_graph::node_t> createNodeFromName(ImVec2 pos, const char* name)
{
	for (int i = 0; i < (int)sizeof_array(s_nodeTypes); ++i)
	{
		if (!strcmp(s_nodeTypes[i].name.c_str(), name))
			return createNodeFromType(pos, &s_nodeTypes[i]);
	}

	return 0;
}

enum DragState
{
	DragState_Default,
	DragState_Hover,
	DragState_BeginDrag,
	DragState_Draging,
	DragState_Connect,
};

struct DragNode
{
	ImVec2 pos;
	node_graph::connection_t* con;
};

static DragNode s_dragNode;
static DragState s_dragState = DragState_Default;

static std::vector<std::unique_ptr<node_graph::node_t>> s_nodes;

void drawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int STEPS)
{
	ImVec2 t1 = ImVec2(+80.0f, 0.0f);
	ImVec2 t2 = ImVec2(+80.0f, 0.0f);

	for (int step = 0; step <= STEPS; step++)
	{
		float t = (float)step / (float)STEPS;
		float h1 = +2 * t*t*t - 3 * t*t + 1.0f;
		float h2 = -2 * t*t*t + 3 * t*t;
		float h3 = t * t*t - 2 * t*t + t;
		float h4 = t * t*t - t * t;
		drawList->PathLineTo(ImVec2(h1*p1.x + h2 * p2.x + h3 * t1.x + h4 * t2.x, h1*p1.y + h2 * p2.y + h3 * t1.y + h4 * t2.y));
	}

	drawList->PathStroke(ImColor(200, 200, 100), false, 3.0f);
}

static bool isConnectorHovered(node_graph::connection_t* c, ImVec2 offset)
{
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImVec2 conPos = offset + c->pos;

	float xd = mousePos.x - conPos.x;
	float yd = mousePos.y - conPos.y;

	return ((xd * xd) + (yd *yd)) < (NODE_SLOT_RADIUS * NODE_SLOT_RADIUS);
}

static node_graph::connection_t* getHoverCon(ImVec2 offset, ImVec2* pos)
{
	for (auto &node : s_nodes)
	{
		const ImVec2 node_pos = node->pos + offset;

		for (node_graph::connection_t* con : node->input_connections)
		{
			if (isConnectorHovered(con, node_pos))
			{
				*pos = node_pos + con->pos;
				return con;
			}
		}

		for (node_graph::connection_t* con : node->output_connections)
		{
			if (isConnectorHovered(con, node_pos))
			{
				*pos = node_pos + con->pos;
				return con;
			}
		}
	}

	s_dragNode.con = 0;
	return 0;
}

void updateDraging(ImVec2 offset)
{
	switch (s_dragState)
	{
	case DragState_Default:
	{
		ImVec2 pos;
		node_graph::connection_t* con = getHoverCon(offset, &pos);

		if (con)
		{
			s_dragNode.con = con;
			s_dragNode.pos = pos;
			s_dragState = DragState_Hover;
			return;
		}

		break;
	}

	case DragState_Hover:
	{
		ImVec2 pos;
		node_graph::connection_t* con = getHoverCon(offset, &pos);

		// Make sure we are still hovering the same node

		if (con != s_dragNode.con)
		{
			s_dragNode.con = 0;
			s_dragState = DragState_Default;
			return;
		}

		if (ImGui::IsMouseClicked(0) && s_dragNode.con)
			s_dragState = DragState_Draging;

		break;
	}

	case DragState_BeginDrag:
	{
		break;
	}

	case DragState_Draging:
	{
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		drawList->ChannelsSetCurrent(0); // Background

		drawHermite(drawList, s_dragNode.pos, ImGui::GetIO().MousePos, 12);

		if (!ImGui::IsMouseDown(0))
		{
			ImVec2 pos;
			node_graph::connection_t* con = getHoverCon(offset, &pos);

			// Make sure we are still hovering the same node

			if (con == s_dragNode.con)
			{
				s_dragNode.con = 0;
				s_dragState = DragState_Default;
				return;
			}

			// Lets connect the nodes.
			// TODO: Make sure we connect stuff in the correct way!

			con->input = s_dragNode.con;
			s_dragNode.con = 0;
			s_dragState = DragState_Default;
		}

		break;
	}

	case DragState_Connect:
	{
		break;
	}
	}
}

static void displayNode(ImDrawList* drawList, ImVec2 offset, node_graph::node_t* node, int& node_selected)
{
	int node_hovered_in_scene = -1;
	bool open_context_menu = false;

	ImGui::PushID(node->id);
	ImVec2 node_rect_min = offset + node->pos;

	// Display node contents first
	drawList->ChannelsSetCurrent(1); // Foreground
	bool old_any_active = ImGui::IsAnyItemActive();

	// Draw title in center

	ImVec2 textSize = ImGui::CalcTextSize(node->name.c_str());

	ImVec2 pos = node_rect_min + NODE_WINDOW_PADDING;
	pos.x = node_rect_min.x + (node->size.x / 2) - textSize.x / 2;

	ImGui::SetCursorScreenPos(pos);
	//ImGui::BeginGroup(); // Lock horizontal position
	ImGui::Text("%s", node->name);
	//ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
	//float dummy_color[3] = { node->Pos.x / ImGui::GetWindowWidth(), node->Pos.y / ImGui::GetWindowHeight(), fmodf((float)node->ID * 0.5f, 1.0f) };
	//ImGui::ColorEdit3("##color", &dummy_color[0]);

	// Save the size of what we have emitted and weither any of the widgets are being used
	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
	//node->size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
	ImVec2 node_rect_max = node_rect_min + node->size;

	// Display node box
	drawList->ChannelsSetCurrent(0); // Background

	ImGui::SetCursorScreenPos(node_rect_min);
	ImGui::InvisibleButton("node", node->size);

	if (ImGui::IsItemHovered())
	{
		node_hovered_in_scene = node->id;
		open_context_menu |= ImGui::IsMouseClicked(1);
	}

	bool node_moving_active = false;

	if (ImGui::IsItemActive() && !s_dragNode.con)
		node_moving_active = true;

	ImU32 node_bg_color = node_hovered_in_scene == node->id ? ImColor(75, 75, 75) : ImColor(60, 60, 60);
	drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);

	ImVec2 titleArea = node_rect_max;
	titleArea.y = node_rect_min.y + 30.0f;

	// Draw text bg area
	drawList->AddRectFilled(node_rect_min + ImVec2(1, 1), titleArea, ImColor(100, 0, 0), 4.0f);
	drawList->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);

	ImVec2 off;

	offset.y += 40.0f;

	offset = offset + node_rect_min;

	off.x = node_rect_min.x;
	off.y = node_rect_min.y;

	for (node_graph::connection_t* con : node->input_connections)
	{
		ImGui::SetCursorScreenPos(offset + ImVec2(10.0f, 0));
		ImGui::Text("%s", con->desc.name);

		ImColor conColor = ImColor(150, 150, 150);

		if (isConnectorHovered(con, node_rect_min))
			conColor = ImColor(200, 200, 200);

		drawList->AddCircleFilled(node_rect_min + con->pos, NODE_SLOT_RADIUS, conColor);

		offset.y += textSize.y + 2.0f;
	}

	offset = node_rect_min;
	offset.y += 40.0f;

	for (node_graph::connection_t* con : node->output_connections)
	{
		textSize = ImGui::CalcTextSize(con->desc.name.c_str());

		ImGui::SetCursorScreenPos(offset + ImVec2(con->pos.x - (textSize.x + 10.0f), 0));
		ImGui::Text("%s", con->desc.name);

		ImColor conColor = ImColor(150, 150, 150);

		if (isConnectorHovered(con, node_rect_min))
			conColor = ImColor(200, 200, 200);

		drawList->AddCircleFilled(node_rect_min + con->pos, NODE_SLOT_RADIUS, conColor);

		offset.y += textSize.y + 2.0f;
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

node_graph::node_t* findNodeByCon(node_graph::connection_t* findCon)
{
	for (auto &node : s_nodes)
	{
		for (node_graph::connection_t* con : node->input_connections)
		{
			if (con == findCon)
				return node.get();
		}

		for (node_graph::connection_t* con : node->output_connections)
		{
			if (con == findCon)
				return node.get();
		}
	}

	return 0;
}

void renderLines(ImDrawList* drawList, ImVec2 offset)
{
	for (auto &node : s_nodes)
	{
		for (node_graph::connection_t* con : node->input_connections)
		{
			if (!con->input)
				continue;

			node_graph::node_t* targetNode = findNodeByCon(con->input);

			if (!targetNode)
				continue;

			drawHermite(drawList,
				offset + targetNode->pos + con->input->pos,
				offset + node->pos + con->pos,
				12);
		}
	}
}

void ShowExampleAppCustomNodeGraph(bool* opened)
{
	ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiSetCond_FirstUseEver);
	if (!ImGui::Begin("Example: Custom Node Graph", opened))
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
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.15f, 0.15f, 0.15f, 0.8f));
	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);


	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->ChannelsSplit(2);
	//ImVec2 offset = ImGui::GetCursorScreenPos() - scrolling;

	//displayNode(draw_list, scrolling, s_emittable, node_selected);
	//displayNode(draw_list, scrolling, s_emitter, node_selected);

	for (auto &node : s_nodes)
		displayNode(draw_list, scrolling, node.get(), node_selected);

	updateDraging(scrolling);
	renderLines(draw_list, scrolling);

	draw_list->ChannelsMerge();

	// Open context menu
	if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
	{
		node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
		open_context_menu = true;
	}
	if (open_context_menu)
	{
		ImGui::OpenPopup("context_menu");
		if (node_hovered_in_list != -1)
			node_selected = node_hovered_in_list;
		if (node_hovered_in_scene != -1)
			node_selected = node_hovered_in_scene;
	}

	// Draw context menu
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("context_menu"))
	{
		if (ImGui::MenuItem("Load graph..."))
		{
			/*
			char path[1024];
			if (Dialog_open(path))
			{
			printf("file to load %s\n", path);
			}
			*/
		}

		if (ImGui::MenuItem("Save graph..."))
		{
			/*
			char path[1024];
			if (Dialog_save(path))
			{
			saveNodes(path);
			}
			*/
		}


		/*
		Node* node = node_selected != -1 ? &nodes[node_selected] : NULL;
		ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
		if (node)
		{
		ImGui::Text("Node '%s'", node->Name);
		ImGui::Separator();
		if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
		if (ImGui::MenuItem("Delete", NULL, false, false)) {}
		if (ImGui::MenuItem("Copy", NULL, false, false)) {}
		}
		*/
		//else

		for (int i = 0; i < (int)sizeof_array(s_nodeTypes); ++i)
		{
			if (ImGui::MenuItem(s_nodeTypes[i].name.c_str()))
			{
				s_nodes.emplace_back(createNodeFromType(ImGui::GetIO().MousePos, &s_nodeTypes[i]) );
			}
		}

		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

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
