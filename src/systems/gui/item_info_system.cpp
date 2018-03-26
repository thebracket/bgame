#include "stdafx.h"
#include "item_info_system.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../global_assets/game_mode.hpp"
#include "../../raws/items.hpp"
#include "../../raws/defs/item_def_t.hpp"
#include "../../raws/materials.hpp"
#include "../../raws/defs/material_def_t.hpp"
#include "../../raws/clothing.hpp"
#include "../../raws/defs/clothing_t.hpp"
#include "../../raws/reactions.hpp"
#include "../../raws/defs/reaction_t.hpp"
#include "../../raws/buildings.hpp"
#include "../../raws/defs/building_def_t.hpp"
#include "units_info_system.hpp"
#include <fmt/format.h>
#include "../../bengine/btabs.hpp"

namespace systems {
	namespace item_ui {
		std::size_t selected_item = 0;
		const static std::string win_item_info = std::string(ICON_FA_SHIELD) + " Item Information";
		const static std::string btn_close = std::string(ICON_FA_TIMES) + " Close";
		const static std::string tab_summary = std::string(ICON_FA_INFO_CIRCLE) + " Description";
		const static std::string tab_ammo = std::string(ICON_FA_INFO_CIRCLE) + " Ammo Info";
		const static std::string tab_melee = std::string(ICON_FA_INFO_CIRCLE) + " Melee Info";
		const static std::string tab_ranged = std::string(ICON_FA_INFO_CIRCLE) + " Ranged Info";
		const static std::string tab_sources = std::string(ICON_FA_INFO_CIRCLE) + " Source Info";
		const static std::string tab_uses = std::string(ICON_FA_INFO_CIRCLE) + " Usage Info";
		const static std::string tab_construction = std::string(ICON_FA_INFO_CIRCLE) + " Construction Info";

		static bool show_window = true;
		static int tab_selected = 0;

		static bengine::entity_t * item_entity;
		static item_t * item_c;
		static item_ammo_t * item_ammo;
		static item_bone_t * item_bone;
		static item_chopping_t * item_chopping;
		static item_digging_t * item_digging;
		static item_drink_t * item_drink;
		static item_food_t * item_food;
		static item_hide_t * item_hide;
		static item_leather_t * item_leather;
		static item_melee_t * item_melee;
		static item_ranged_t * item_ranged;
		static item_skull_t * item_skull;
		static item_spice_t * item_spice;
		static item_carried_t * item_carried;
		static item_stored_t * item_stored;
		static item_farming_t * item_farming;
		static item_quality_t * item_quality;
		static item_wear_t * item_wear;
		static material_def_t * item_mat;
		static item_creator_t * item_creator;
		static item_def_t * item_def;

		static void load_item_details()
		{
			item_entity = bengine::entity(selected_item);
			item_c = item_entity->component<item_t>();
			item_ammo = item_entity->component<item_ammo_t>();
			item_bone = item_entity->component<item_bone_t>();
			item_chopping = item_entity->component<item_chopping_t>();
			item_digging = item_entity->component<item_digging_t>();
			item_drink = item_entity->component<item_drink_t>();
			item_food = item_entity->component<item_food_t>();
			item_hide = item_entity->component<item_hide_t>();
			item_leather = item_entity->component<item_leather_t>();
			item_melee = item_entity->component<item_melee_t>();
			item_ranged = item_entity->component<item_ranged_t>();
			item_skull = item_entity->component<item_skull_t>();
			item_spice = item_entity->component<item_spice_t>();
			item_carried = item_entity->component<item_carried_t>();
			item_stored = item_entity->component<item_stored_t>();
			item_farming = item_entity->component<item_farming_t>();
			item_quality = item_entity->component<item_quality_t>();
			item_wear = item_entity->component<item_wear_t>();
			item_mat = get_material(item_c->material);
			item_creator = item_entity->component<item_creator_t>();
			item_def = get_item_def(item_c->item_tag);
		}

		static fmt::MemoryWriter get_tags()
		{
			fmt::MemoryWriter tags;
			if (item_ammo) tags << "AMMO ";
			if (item_bone) tags << "BONE ";
			if (item_chopping) tags << "CHOPPING ";
			if (item_digging) tags << "DIGGING ";
			if (item_drink) tags << "DRINK ";
			if (item_food) tags << "FOOD ";
			if (item_hide) tags << "HIDE ";
			if (item_leather) tags << "LEATHER ";
			if (item_melee) tags << "MELEE ";
			if (item_ranged) tags << "RANGED ";
			if (item_skull) tags << "SKULL ";
			if (item_spice) tags << "SPICE ";
			if (item_farming) tags << "FARMING ";
			return tags;
		}

		static void render_item_header()
		{
			fmt::MemoryWriter item_name;
			item_name << item_c->item_name;
			if (item_mat) item_name << ", made of " << item_mat->name;
			if (item_quality) item_name << ", of " << item_quality->get_quality_text() << " quality";
			if (item_wear) item_name << ", " << item_wear->get_wear_text() << "(" << +item_wear->wear << "/100) condition";
			item_name << ".";
			std::string item_name_str = item_name.str();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", item_name_str.c_str());

			if (item_creator) {
				ImGui::Text("Created by: %s", item_creator->creator_name.c_str());
			}

			const auto tags = get_tags();
			std::string tag_str = tags.str();
			if (tag_str != "") {
				ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "TAGS: %s", tag_str.c_str());
			}

			if (item_carried) {
				auto carried_by = bengine::entity(item_carried->carried_by);
				if (carried_by) {
					if (carried_by->component<settler_ai_t>()) {
						auto name = carried_by->component<name_t>();
						ImGui::Text("Carried by %s %s", name->first_name.c_str(), name->last_name.c_str());
						ImGui::SameLine();
						const std::string btn_view = std::string(ICON_FA_CAMERA) + " View##" + std::to_string(carried_by->id);
						if (ImGui::Button(btn_view.c_str())) {
							tab_selected = 0;
							game_master_mode = SETTLER;
							units_ui::selected_settler = item_carried->carried_by;
						}
					}
					else {
						auto name = carried_by->component<name_t>();
						ImGui::Text("Carried by %s %s", name->first_name.c_str(), name->last_name.c_str());
						ImGui::SameLine();
						const std::string btn_view = std::string(ICON_FA_CAMERA) + " View##" + std::to_string(carried_by->id);
						if (ImGui::Button(btn_view.c_str())) {
							game_master_mode = SENTIENT_INFO;
							units_ui::current_native = item_carried->carried_by;
						}
					}
				}
			}

			if (item_stored) {
				ImGui::Text("Stored");
				// TODO: More Information
			}
		}

		static void render_summary()
		{
			if (item_def) {
				ImGui::Text("%s", item_def->description.c_str());
			}
			else {
				auto cloth_def = get_clothing_by_tag(item_c->item_tag.c_str());
				if (cloth_def) {
					ImGui::Text("%s", cloth_def->description.c_str());
					ImGui::Text("It provides an armor bonus of %f", cloth_def->armor_class);
					if (item_mat) {
						ImGui::Text("That material provides an additional armor bonus of %f", item_mat->ac_bonus);
						if (item_quality && item_quality->quality > 3) ImGui::Text("It gains an extra +1 for being of high quality.");
						if (item_quality && item_quality->quality > 6) ImGui::Text("It gains an extra +1 for being of masterwork quality.");
					}
				}
				else {
					ImGui::Text("WARNING, definition for %s did not load.", item_c->item_tag.c_str());
				}
			}
		}

		static void render_ammo()
		{
			ImGui::Text("This item is used as ammunition (type: %s).", item_def->ammo.c_str());
			if (item_def) {
				ImGui::Text("It inflicts %dd%d+%d damage.", item_def->damage_n, item_def->damage_d, item_def->damage_mod);
			}
		}

		static void render_melee()
		{
			ImGui::Text("It inflicts %dd%d+%d damage, modified by %s.", item_def->damage_n, item_def->damage_d, item_def->damage_mod, item_def->damage_stat.c_str());
			if (item_quality && item_quality->quality > 3) ImGui::Text("It gains an extra +1 for being of high quality.");
			if (item_quality && item_quality->quality > 6) ImGui::Text("It gains an extra +1 for being of masterwork quality.");
			if (item_mat) {
				ImGui::Text("%s material provides an additional %d damage modifier.", item_mat->name.c_str(), item_mat->damage_bonus);
			}
		}

		static void render_ranged()
		{
			ImGui::Text("It has a range of %d tiles.", item_def->range);
			ImGui::Text("It provides an ammunition damage bonus of %d", item_def->damage_mod);
			ImGui::Text("It's initiative penalty (mitigated by Dexterity) is %d", item_def->initiative_penalty);
			ImGui::Text("It requires ammunition of type: %s", item_def->ammo.c_str());
			if (item_quality && item_quality->quality > 3) ImGui::Text("It gains an extra +1 for being of high quality.");
			if (item_quality && item_quality->quality > 6) ImGui::Text("It gains an extra +1 for being of masterwork quality.");
			if (item_mat) {
				ImGui::Text("%s material provides an additional %d damage modifier.", item_mat->name.c_str(), item_mat->damage_bonus);
			}
		}

		void run(const double &duration_ms) {
			bengine::btabs_t item_tabs{
				{
					bengine::btab_t{ tab_summary, render_summary }
				}
			};
			item_tabs.selected = tab_selected;

			load_item_details();

			if (selected_item == 0 || item_entity == nullptr || item_c == nullptr) {
				game_master_mode = PLAY;
				return;
			}

			bengine::begin_info_window(win_item_info, &show_window);
			render_item_header();

			const std::string tab_bar_name = std::string("##Item#info_bar") + std::to_string(selected_item);


			if (item_ammo) item_tabs.tabs.emplace_back(bengine::btab_t{ tab_ammo, render_ammo });
			if (item_bone) ImGui::Text("This item is an unprocessed bone. Take it to a bonecarver.");
			if (item_chopping) ImGui::Text("This item can be used to cut down trees.");
			if (item_digging) ImGui::Text("This item can be used for mining.");
			if (item_drink) ImGui::Text("This item can be drunk.");
			if (item_food) ImGui::Text("This item can be eaten or cooked.");
			if (item_hide) ImGui::Text("This item is unprocessed hide. Take it to a tanner.");
			if (item_leather) ImGui::Text("This item is processed leather, and can be turned into something useful by a leatherworker.");
			if (item_melee && item_def) item_tabs.tabs.emplace_back(bengine::btab_t{ tab_melee, render_melee });
			if (item_ranged && item_def) item_tabs.tabs.emplace_back(bengine::btab_t{ tab_ranged, render_ranged });
			if (item_skull) ImGui::Text("This is an unprocessed skull, and may be used at a bonecarver.");
			if (item_spice) ImGui::Text("This is a spice, used to improve flavor in cooking.");
			if (item_farming) ImGui::Text("This is used for farming.");

			// Poll the reactions system
			std::vector<std::string> sources;
			std::vector<std::string> uses;
			each_reaction([&sources, &uses](std::string reaction_tag, reaction_t * reaction) {
				// Check for inclusion in an input
				for (const auto input : reaction->inputs) {
					if (input.tag == item_c->item_tag) {
						uses.push_back(std::string("Can be used at a ") + get_building_def(reaction->workshop)->name + std::string(" to ") + reaction->name);
					}
				}

				for (const auto output : reaction->outputs) {
					if (output.first == item_c->item_tag) {
						// Add to list
						sources.push_back(std::string("Can be made at a ") + get_building_def(reaction->workshop)->name);
					}
				}
			});

			// Poll the buildings system
			std::vector<std::string> buildings;
			each_building_def([&buildings](building_def_t *building_def) {
				for (const auto input : building_def->components) {
					if (input.tag == item_c->item_tag) {
						// Add to list
						buildings.push_back(std::string("Can be used to construct a ") + building_def->name);
					}
				}
			});

			if (!uses.empty()) item_tabs.tabs.emplace_back(bengine::btab_t{ tab_uses, [&uses] () { for (const auto &use : uses) ImGui::Text("%s", use.c_str()); } });
			if (!sources.empty()) item_tabs.tabs.emplace_back(bengine::btab_t{ tab_sources, [&sources] () { for (const auto &source : sources) ImGui::Text("%s", source.c_str()); } });
			if (!buildings.empty()) item_tabs.tabs.emplace_back(bengine::btab_t{ tab_construction, [&buildings]() { for (const auto &bld : buildings) ImGui::Text("%s", bld.c_str()); } });


			bengine::render_btab_bar(item_tabs);
			ImGui::End();
			tab_selected = item_tabs.selected;

			if (!show_window)
			{
				game_master_mode = PLAY;
				show_window = true;
				tab_selected = 0;
			}
		}
	}
}
