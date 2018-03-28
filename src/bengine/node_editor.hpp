#pragma once
#include <vector>
#include <string>
#include <memory>
#include "imgui.h"

namespace node_graph
{
	constexpr int MAX_CONNECTION_COUNT = 32;

	enum connection_type_t { CONNECTION_TYPE_COLOR, CONNECTION_TYPE_VEC3, CONNECTION_TYPE_FLOAT, CONNECTION_TYPE_INT };

	struct connection_description_t
	{
		std::string name;
		connection_type_t type = CONNECTION_TYPE_COLOR;
	};

	struct node_type_t
	{
		std::string name;
		std::vector<connection_description_t> input_connections;
		std::vector<connection_description_t> output_connections;
	};

	struct connection_t
	{
		ImVec2 pos;
		node_graph::connection_description_t desc;

		connection_t()
		{
			pos.x = pos.y = 0.0f;
			input = nullptr;
		}

		connection_t * input;
		std::vector<connection_t*> output;
	};

	struct node_t
	{
		ImVec2 pos;
		ImVec2 size;
		int id;
		std::string name;
		std::vector<std::unique_ptr<connection_t>> input_connections;
		std::vector<std::unique_ptr<connection_t>> output_connections;
	};
}

void ShowExampleAppCustomNodeGraph(bool* opened, std::vector<std::unique_ptr<node_graph::node_t>> &all_nodes);
std::unique_ptr<node_graph::node_t> create_node_from_name(const ImVec2 pos, const char* name);