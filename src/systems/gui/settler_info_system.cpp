#include "stdafx.h"
#include "settler_info_system.hpp"
#include "units_info_system.hpp"
#include "../../bengine/IconsFontAwesome.h"
#include "../../bengine/imgui.h"
#include "../../bengine/imgui_impl_glfw_gl3.h"
#include "../../bengine/imgui_tabs.hpp"
#include "../../components/name.hpp"
#include "../../components/game_stats.hpp"
#include "../../components/species.hpp"
#include "../../components/settler_ai.hpp"
#include "../../components/health.hpp"
#include "../../components/item.hpp"
#include "../../components/item_carried.hpp"
#include "../../global_assets/game_planet.hpp"
#include "../../raws/life_events.hpp"
#include "../../raws/defs/life_event_template.hpp"
#include "../../global_assets/game_mode.hpp"

namespace systems {
	namespace settler_ui {

		static const std::string tab_summary = std::string(ICON_FA_USER) + " Summary";
		static const std::string tab_stats = std::string(ICON_FA_BAR_CHART) + " Stats";
		static const std::string tab_health = std::string(ICON_FA_HEART) + " Health";
		static const std::string tab_inventory = std::string(ICON_FA_SHIELD) + " Inventory";
		static const std::string tab_emotions = std::string(ICON_FA_SNOWFLAKE_O) + " Emotional";
		static const std::string tab_relations = std::string(ICON_FA_HANDSHAKE_O) + " Relationships";
		static const std::string tab_history = std::string(ICON_FA_HISTORY) + " History";
		static const std::string tab_schedule = std::string(ICON_FA_CALENDAR) + " Schedule";
		static const std::string tab_work = std::string(ICON_FA_CHECK) + " Labor";

		static inline void add_attribute_row(short attribute, std::string name, std::string desc) {
			ImGui::Text("%s", name.c_str());
			ImGui::NextColumn();
			ImGui::ProgressBar(static_cast<float>(attribute) / 20.0f);
			ImGui::SameLine();
			ImGui::Text("%d", attribute);
			ImGui::NextColumn();
			const std::string str = desc.c_str();
			ImGui::Text("%s", str.c_str());
			ImGui::NextColumn();
			ImGui::Separator();
		}

		static inline void render_stats(game_stats_t * stats, species_t * species) {
			ImGui::Columns(3, "stats_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Attribute"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Level"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Description"); ImGui::NextColumn();
			ImGui::Separator();

			add_attribute_row(stats->strength, "Strength", species->gender_pronoun() + std::string(" ") + stats->strength_str());
			add_attribute_row(stats->dexterity, "Dexterity", species->gender_pronoun() + std::string(" ") + stats->dexterity_str());
			add_attribute_row(stats->constitution, "Constitution", species->gender_pronoun() + std::string(" ") + stats->constitution_str());
			add_attribute_row(stats->intelligence, "Intelligence", species->gender_pronoun() + std::string(" ") + stats->intelligence_str());
			add_attribute_row(stats->wisdom, "Wisdom", species->gender_pronoun() + std::string(" ") + stats->wisdom_str());
			add_attribute_row(stats->charisma, "Charisma", species->gender_pronoun() + std::string(" ") + stats->charisma_str());
			add_attribute_row(stats->comeliness, "Comeliness", species->gender_pronoun() + std::string(" ") + stats->comeliness_str());
			add_attribute_row(stats->ethics, "Ethics", species->gender_pronoun() + std::string(" ") + stats->ethics_str());

			ImGui::Columns(1);

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Skills");
			ImGui::Columns(3, "skills_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Skill"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Level"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Progress"); ImGui::NextColumn();
			ImGui::Separator();

			for (const auto &skill : stats->skills) {
				ImGui::Text("%s", skill.first);
				ImGui::NextColumn();

				ImGui::ProgressBar(static_cast<float>(skill.second.skill_level) / 10.0f);
				ImGui::NextColumn();

				ImGui::Text("%d", skill.second.experience_gained);
				ImGui::NextColumn();
			}

			ImGui::Columns(1);
		}

		static inline void render_summary(name_t * name, game_stats_t * stats, species_t * species, settler_ai_t * ai, health_t * health) {
			std::stringstream header;
			header << " " << name->first_name << " " << name->last_name << " (" << stats->profession_tag << ") ";
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), header.str().c_str());

			std::stringstream header2;
			header2 << species->gender_str() << ", " << species->sexuality_str() << ", " << stats->age << " years old. " << species->height_feet() << ", " << species->weight_lbs();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), header2.str().c_str());

			std::stringstream profile;
			profile << species->gender_pronoun() << std::string(" has ") << species->ethnicity() << std::string(" colored skin. ");
			profile << species->gender_pronoun() << std::string(" has ") << species->hair_color_str() << std::string(" hair, ") << species->hair_style_str() << ".";
			ImGui::Text("%s", profile.str().c_str());


			ImGui::Text("Hit Points: "); ImGui::SameLine();
			ImGui::ProgressBar(static_cast<float>(health->current_hitpoints) / static_cast<float>(health->max_hitpoints));

			ImGui::Text("%s: %s", "Current Status:", ai->job_status.c_str());
		}

		static inline void render_health(health_t * health) {
			ImGui::Text("Hit Points: "); ImGui::SameLine();
			ImGui::ProgressBar(static_cast<float>(health->current_hitpoints) / static_cast<float>(health->max_hitpoints));

			ImGui::Columns(2, "health_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Part"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "State"); ImGui::NextColumn();
			ImGui::Separator();

			for (const health_part_t &part : health->parts) {
				std::string part_state = "OK";
				if (part.current_hitpoints < 1 && part.current_hitpoints > -10) part_state = "IMPAIRED";
				if (part.current_hitpoints < -9 && part.current_hitpoints > -20) part_state = "BROKEN";
				if (part.current_hitpoints < -19) part_state = "GONE";

				ImGui::Text("%s", part.part);
				ImGui::NextColumn();

				ImGui::Text("%s", part_state);
				ImGui::NextColumn();

				ImGui::Separator();
			}

			ImGui::Columns(1);
		}

		static inline void render_inventory(std::size_t selected_settler) {
			using namespace bengine;

			ImGui::Columns(2, "inventory_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Item"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Location"); ImGui::NextColumn();
			ImGui::Separator();

			each_if<item_t, item_carried_t>(
				[&selected_settler](entity_t &e, item_t &item, item_carried_t &carried) {
				return carried.carried_by == selected_settler;
			},
				[](entity_t &e, item_t &item, item_carried_t &carried) {
				ImGui::Text("%s", item.item_name.c_str());
				ImGui::NextColumn();

				std::string ci = item_loc_name(carried.location);
				ImGui::Text("%s", ci.c_str());
				ImGui::NextColumn();

				ImGui::Separator();
			});
		}

		static inline void render_history(std::size_t selected_settler) {
			ImGui::Columns(2, "history_grid");
			ImGui::Separator();

			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Year"); ImGui::NextColumn();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s", "Event"); ImGui::NextColumn();
			ImGui::Separator();

			for (const life_event_t &le : planet.history.settler_life_events[selected_settler]) {
				auto finder = get_life_event(le.type);
				if (finder != nullptr) {
					const std::string year = std::to_string(le.year);
					std::string event = finder->description + std::string(" ");

					ImGui::Text("%s", year);
					ImGui::NextColumn();

					ImGui::Text("%s", event.c_str());
					ImGui::NextColumn();

					ImGui::Separator();
				}
				else {
					std::cout << "Warning: " << le.type << " life event not found\n";
				}
			}
		}

		void run(const double &duration_ms) {
			using namespace bengine;
			using namespace systems::units_ui;
			auto name = entity(selected_settler)->component<name_t>();
			auto stats = entity(selected_settler)->component<game_stats_t>();
			auto species = entity(selected_settler)->component<species_t>();
			auto ai = entity(selected_settler)->component<settler_ai_t>();
			auto health = entity(selected_settler)->component<health_t>();

			std::stringstream header;
			header << std::string(ICON_FA_USER) << " " << name->first_name << " " << name->last_name << " (" << stats->profession_tag << ") ";

			ImGui::Begin(header.str().c_str(), nullptr, ImGuiWindowFlags_NoCollapse);

			if (ImGui::Button("Close")) {
				game_master_mode = PLAY;
			}

			ImGui::BeginTabBar("##Settler#info_bar");
			ImGui::DrawTabsBackground();

			if (ImGui::AddTab(tab_summary.c_str())) {
				render_summary(name, stats, species, ai, health);
			}
			if (ImGui::AddTab(tab_stats.c_str())) {
				render_stats(stats, species);
			}
			if (ImGui::AddTab(tab_health.c_str())) {
				render_health(health);
			}
			if (ImGui::AddTab(tab_schedule.c_str())) {

			}
			if (ImGui::AddTab(tab_work.c_str())) {

			}
			if (ImGui::AddTab(tab_inventory.c_str())) {
				render_inventory(selected_settler);
			}
			if (ImGui::AddTab(tab_emotions.c_str())) {

			}
			if (ImGui::AddTab(tab_relations.c_str())) {

			}
			if (ImGui::AddTab(tab_history.c_str())) {
				render_history(selected_settler);
			}

			ImGui::EndTabBar();
									
			ImGui::End();
		}
	}
}