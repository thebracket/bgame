#include "workflow_ui_system.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../raws/reactions.hpp"
#include "../../raws/defs/reaction_t.hpp"
#include "../helpers/inventory_assistant.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../global_assets/building_designations.hpp"
#include "../../bengine/btabs.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include <map>

namespace systems {
	namespace workflow_ui {
		const std::string win_workflow = std::string(ICON_FA_WRENCH) + " Workflow Management";
		const std::string btn_build = std::string(ICON_FA_PLUS) + " Queue Job";
		const std::string btn_close = std::string(ICON_FA_TIMES) + " Cancel";
		const std::string win_queue = std::string(ICON_FA_LIST) + " Jobs Queue";
		const std::string win_available = std::string(ICON_FA_INDUSTRY) + " Make Work Orders";
		const std::string win_orders = std::string(ICON_FA_LINE_CHART) + " Standing Orders";
		int selected_work_item = 0;
		int selected_build_item = 0;
		static bool show_window = true;

		static bool queue_headings_first_run = true;
		static std::vector<bengine::table_heading_t> queue_headings{
			{ "No. Queued", -1.0f}, { "Job Name", -1.0f }, { "Options", -1.0f }
		};

		static bool wo_headings_first_run = true;
		static std::vector<bengine::table_heading_t> wo_headings{
			{ "Reaction", -1.0f },{ "Workshop", -1.0f },{ "Inputs", -1.0f },{ "Outputs", -1.0f }, {"# Queued", -1.0f} ,{ "Options", -1.0f }
		};

		static bool so_headings_first_run = true;
		static std::vector<bengine::table_heading_t> so_headings{
			{ "Item", -1.0f },{ "# To Keep In Stock", -1.0f },{ "Options", -1.0f }
		};

		static void render_queue()
		{
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "The following jobs have been queued up, and will be performed when settlers, components and workshops are available.");
			bool zebra = true;
			std::vector<std::string> to_remove;
			if (!building_designations->build_orders.empty()) {
				bengine::begin_table(queue_headings_first_run, queue_headings, "workqueue_list_grid", true);
				for (auto &order : building_designations->build_orders)
				{
					bengine::zebra_row(zebra);
					bengine::begin_zebra_col(zebra);
					ImGui::Text("%i", order.first);
					bengine::end_zebra_col();

					bengine::begin_zebra_col(zebra);
					auto const finder = get_reaction_def(order.second);
					if (finder != nullptr) {
						ImGui::Text("%s", finder->name.c_str());
					}
					bengine::end_zebra_col();

					bengine::begin_zebra_col(zebra);
					const std::string btn_id = btn_close + std::string("##") + order.second;
					if (ImGui::SmallButton(btn_id.c_str()))
					{
						if (order.first > 1)
						{
							--order.first;
						} else
						{
							to_remove.emplace_back(order.second);
						}
					}
					bengine::end_zebra_col();
				}
				bengine::end_table();

				for (auto &j : to_remove)
				{
					building_designations->build_orders.erase(
						std::remove_if(
							building_designations->build_orders.begin(),
							building_designations->build_orders.end(),
							[&j](const auto &b) { return b.second == j; }
						),
						building_designations->build_orders.end()
					);
				}
			} else
			{
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "There are no enqueued work orders. You can create orders on the \"Make Work Orders\" tab.");
			}

		}

		char filter[254];
		std::string filter_str;

		static bool meets_filter(const std::pair<std::string, std::string> &r)
		{
			if (filter_str.empty()) return true;
			auto tmp = r.second;
			std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
			return tmp.find(filter_str) != std::string::npos;
		}

		static void render_available()
		{
			std::map<std::string, int> orders;
			for (const auto &j : building_designations->build_orders)
			{
				orders.insert(std::make_pair(j.second, j.first));
			}

			ImGui::TextColored(ImVec4(0.0f, 1.0f, 1.0f, 1.0f), "The following jobs are available to be added to the work queue.");
			ImGui::Text("%s", "Filter: ");
			ImGui::SameLine();
			ImGui::InputText("##filter", (char *)&filter, 254);
			filter_str = std::string(filter);
			std::transform(filter_str.begin(), filter_str.end(), filter_str.begin(), ::tolower);

			auto zebra = true;
			auto available_reactions = inventory::get_available_reactions();
			if (!available_reactions.empty()) {
				bengine::begin_table(wo_headings_first_run, wo_headings, "workorder_list_grid", true);
				for (const auto &r : available_reactions)
				{
					if (meets_filter(r)) {
						bengine::zebra_row(zebra);
						bengine::begin_zebra_col(zebra);
						ImGui::Text("%s", r.second.c_str());
						bengine::end_zebra_col();

						bengine::begin_zebra_col(zebra);
						const auto rf = get_reaction_def(r.first);
						const auto bf = get_building_def(rf->workshop);
						ImGui::Text("%s", bf->name.c_str());
						bengine::end_zebra_col();

						bengine::begin_zebra_col(zebra);
						std::string inputs;
						for (const auto &i : rf->inputs)
						{
							const auto ifinder = get_item_def(i.tag);
							if (i.required_material > 0)
							{
								const auto mf = get_material(i.required_material);
								inputs += std::string("{") + mf->name + std::string("}");
							}
							if (i.required_material_type > 0)
							{
								switch (i.required_material_type)
								{
								case CLUSTER_ROCK: inputs += std::string("[rock] "); break;
								case ROCK: inputs += std::string("[rock] "); break;
								case SOIL: inputs += std::string("[soil] "); break;
								case SAND: inputs += std::string("[sand] "); break;
								case METAL: inputs += std::string("[metal] "); break;
								case SYNTHETIC: inputs += std::string("[synthetic] "); break;
								case ORGANIC: inputs += std::string("[organic] "); break;
								case LEATHER: inputs += std::string("[leather] "); break;
								case FOOD: inputs += std::string("[food] "); break;
								case SPICE: inputs += std::string("[spice] "); break;
								case BLIGHT: inputs += std::string("[blight] "); break;
								default: inputs += std::string("[?]");
								}
							}
							const auto name = ifinder ? ifinder->name : std::string("");
							inputs += name + std::string(" (x") + std::to_string(i.quantity) + ") ";
						}
						if (rf->power_drain > 0) inputs += std::string("Drains ") + std::to_string(rf->power_drain) + std::string(" power. ");
						ImGui::Text("%s", inputs.c_str());
						bengine::end_zebra_col();

						bengine::begin_zebra_col(zebra);
						std::string outputs;
						for (const auto &i : rf->outputs)
						{
							const auto ifinder = get_item_def(i.first);
							outputs += ifinder->name + std::string(" (x") + std::to_string(i.second) + ") ";
						}
						ImGui::Text("%s", outputs.c_str());
						bengine::end_zebra_col();

						bengine::begin_zebra_col(zebra);
						const auto qf = orders.find(r.first);
						if (qf == orders.end()) {
							ImGui::Text("%s", "0");
						} else
						{
							ImGui::Text("%d", qf->second);
						}
						bengine::end_zebra_col();

						bengine::begin_zebra_col(zebra);
						const std::string btn_id = btn_build + std::string("##") + rf->tag;
						if (ImGui::SmallButton(btn_id.c_str()))
						{
							auto found = false;
							for (auto &job : building_designations->build_orders)
							{
								if (job.second == rf->tag)
								{
									found = true;
									++job.first;
								}
							}
							if (!found)
							{
								building_designations->build_orders.emplace_back(std::make_pair(1, rf->tag));
							}
						}
						bengine::end_zebra_col();
					}
				}
				bengine::end_table();
			}
			else
			{
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "There are no available tasks! Try creating some workshops.");
			}
		}

		static void render_orders()
		{
			std::set<std::tuple<std::string, std::string, std::string>> can_build; // Name, reaction tag, item tag

			auto available_reactions = inventory::get_available_reactions();
			for (const auto &r : available_reactions)
			{
				const auto reaction_finder = get_reaction_def(r.first);
				if (reaction_finder) {
					for (const auto &o : reaction_finder->outputs)
					{
						const auto item_finder = get_item_def(o.first);
						if (item_finder)
						{
							can_build.insert(std::make_tuple(item_finder->name, reaction_finder->tag, item_finder->tag));
						}
					}
				}
			}

			bengine::begin_table(so_headings_first_run, so_headings, "so_list_grid", true);
			auto zebra = true;
			for (const auto &so : building_designations->standing_build_orders)
			{
				bengine::zebra_row(zebra);
				bengine::begin_zebra_col(zebra);
				const auto item_finder = get_item_def(so.first);
				ImGui::Text("%s", item_finder->name.c_str());
				bengine::end_zebra_col();

				bengine::begin_zebra_col(zebra);
				ImGui::Text("%d", so.second.first);
				bengine::end_zebra_col();

				bengine::begin_zebra_col(zebra);
				const std::string btn_plus = std::string("+##") + so.first;
				const std::string btn_minus = std::string("-##") + so.first;
				if (ImGui::SmallButton(btn_plus.c_str()))
				{
					++building_designations->standing_build_orders[so.first].first;
				}
				ImGui::SameLine();
				if (ImGui::SmallButton(btn_minus.c_str()))
				{
					--building_designations->standing_build_orders[so.first].first;
					if (building_designations->standing_build_orders[so.first].first == 0)
					{
						building_designations->standing_build_orders.erase(so.first);
					}
				}
				bengine::end_zebra_col();
			}

			for (const auto &it : can_build)
			{
				const auto finder = building_designations->standing_build_orders.find(std::get<2>(it));
				if (finder == building_designations->standing_build_orders.end()) {
					bengine::zebra_row(zebra);
					bengine::begin_zebra_col(zebra);
					ImGui::Text("%s", std::get<0>(it).c_str());
					bengine::end_zebra_col();

					bengine::begin_zebra_col(zebra);
					ImGui::Text("%s", "0");
					bengine::end_zebra_col();

					bengine::begin_zebra_col(zebra);
					const std::string btn_plus1 = std::string("+##") + std::get<2>(it);
					if (ImGui::SmallButton(btn_plus1.c_str()))
					{
						building_designations->standing_build_orders.insert(std::make_pair(std::get<2>(it), std::make_pair(1, std::get<1>(it))));
					}
					bengine::end_zebra_col();
				}
			}

			bengine::end_table();
		}

		static bengine::btabs_t unit_tabs{
			{
				bengine::btab_t{ win_queue, render_queue, std::make_pair("Q", GLFW_KEY_Q) },
				bengine::btab_t{ win_available, render_available, std::make_pair("A", GLFW_KEY_A) },
				bengine::btab_t{ win_orders, render_orders, std::make_pair("O", GLFW_KEY_O) },
			}
		};

		void run(const double &duration_ms) {

			bengine::begin_info_window(win_workflow, &show_window);
			bengine::render_btab_bar(unit_tabs);
			ImGui::End();

			if (!show_window) {
				game_master_mode = PLAY;
				show_window = true;
			}
		}
	}
}
