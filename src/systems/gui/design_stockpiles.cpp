#include "stdafx.h"
#include "design_stockpiles.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../mouse.hpp"
#include "../../global_assets/game_mode.hpp"
#include "../../planet/region/region.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../render_engine/chunks/chunks.hpp"

using namespace tile_flags;

namespace systems {
	namespace design_stockpiles {
		static int selected_stockpile = 0;
		static const std::string win_stockpiles = std::string(ICON_FA_AREA_CHART) + " Stockpiles";

		void run(const double &duration_ms) {
			using namespace bengine;
			using namespace region;

			std::vector<std::pair<std::size_t, std::string>> stockpiles;
			each<stockpile_t>([&stockpiles](entity_t &e, stockpile_t &sp) {
				stockpiles.emplace_back(std::make_pair(e.id, std::string("Stockpile #" + std::to_string(e.id))));
			});
			std::vector<const char *> stockpile_listbox_items(stockpiles.size());
			for (std::size_t i = 0; i<stockpiles.size(); ++i) {
				stockpile_listbox_items[i] = stockpiles[i].second.c_str();
				if (i == static_cast<size_t>(selected_stockpile)) current_stockpile = stockpiles[i].first;
			}

			// Stockpiles list
			ImGui::Begin(win_stockpiles.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoCollapse);
			ImGui::TextWrapped("Stockpiles - select a stockpile from the right panel, click to add, right click to remove.");
			ImGui::PushItemWidth(-1);
			if (!stockpile_listbox_items.empty()) {
				ImGui::ListBox("## Stockpiles", &selected_stockpile, &stockpile_listbox_items.at(0), stockpile_listbox_items.size(), 10);
			}
			if (ImGui::Button("+ Add Stockpile")) {
				const auto sp = create_entity()->assign(stockpile_t{});
				current_stockpile = sp->id;
			}
			ImGui::SameLine();
			if (ImGui::Button("- Remove Stockpile")) {
				delete_entity(current_stockpile);
				delete_stockpile(current_stockpile);
				current_stockpile = 0;
				selected_stockpile = 0;
			}
			if (current_stockpile != 0) {
				auto sp_entity = entity(current_stockpile);
				if (sp_entity) {
					auto sp = sp_entity->component<stockpile_t>();
					if (sp) {
						auto count = 0;
						each_stockpile([&sp, &count](stockpile_def_t * it) {
							if (sp->category.test(it->index)) {
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.6f, 0.0f, 1.0f });
								const auto sp_label = std::string(ICON_FA_CHECK) + std::string(" ") + it->name;
								if (ImGui::Button(sp_label.c_str())) {
									sp->category.reset(it->index);
								}
								ImGui::PopStyleColor();
							}
							else {
								ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.6f, 0.0f, 0.0f, 1.0f });
								const auto sp_label = std::string(ICON_FA_TIMES) + std::string(" ") + it->name;
								if (ImGui::Button(sp_label.c_str())) {
									sp->category.set(it->index);
								}
								ImGui::PopStyleColor();
							}
							if (count != 5) ImGui::SameLine();
							++count;
							count = count % 6;
						});
					}
				}
			}
			ImGui::End();

			if (current_stockpile>0) {

				const auto idx = mapidx(mouse_wx, mouse_wy, mouse_wz);
				if (flag(idx, CAN_STAND_HERE)) {
					if (left_click) {
						if (stockpile_id(idx) == 0) {
							set_stockpile_id(idx, current_stockpile);
							calc_render(idx);
							chunks::mark_chunk_dirty_by_tileidx(idx);
						}
						else {
							current_stockpile = stockpile_id(idx);
						}
					}
					else if (right_click) {
						set_stockpile_id(idx, 0);
						calc_render(idx);
						chunks::mark_chunk_dirty_by_tileidx(idx);
					}
				}
			}
		}
	}
}
