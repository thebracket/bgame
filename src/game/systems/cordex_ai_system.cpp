#include "cordex_ai_system.h"
#include <iostream>
#include <map>
#include "../game.h"
#include "../raws/raws.h"
#include "../tasks/help_wanted.h"
#include "../../engine/geometry.hpp"
#include "../world/universe.hpp"
#include "../world/inventory.hpp"

struct selected_component
{
	int id;
	int x;
	int y;
};

void cordex_ai_system::tick(const double& duration_ms)
{
	handle_build_orders();
	handle_tree_chop_orders();
	handle_reaction_orders();
	handle_dig_orders();
}

void cordex_ai_system::handle_build_orders()
{
	position_component3d * camera_pos = get_camera_position();

	vector<build_order_message> * orders = game_engine->messaging->get_messages_by_type<build_order_message>();
	for (build_order_message &msg : *orders)
	{
		msg.deleted = true; // No repeats

		// Create a place-holder entity for the new structure (just a renderable and a position, obtain the entity ID)
		const int entity_id = raws::create_placeholder_structure_from_raws(
				msg.structure_name, location_t
				{ camera_pos->pos.region, msg.x, msg.y, msg.z });
		game_engine->messaging->add_message<entity_moved_message>(
				entity_moved_message());

		// Create a job
		ai::job_t job;
		job.job_id = ai::get_next_job_id();
		job.current_step = 0;
		job.assigned_to = 0;
		job.type = ai::BUILD_STRUCTURE;

		// Create the sub-tasks; we need to know what components go into the task
		vector<string> components = raws::get_buildable_requirements(
				msg.structure_name);
		vector<available_item_t *> chosen_components;
		for (const string &component : components)
		{
			auto finder = inventory.find(component);
			if (finder == inventory.end())
				return; // Can't do it!
			// Find the closest candidate
			std::map<int, available_item_t *> distance_components;
			for (available_item_t item : finder->second)
			{
				item_component * item_comp = ECS->find_entity_component<item_component>(item.entity_id);
				if (!item_comp->claimed)
				{
					//float distance = std::sqrt( (std::abs(msg.x - item.location.first)*std::abs(msg.x - item.location.first)) + (std::abs(msg.y - item.location.second)*std::abs(msg.y - item.location.second)));
					float distance = geometry::distance3d(msg.x, msg.y, msg.z,
							item.location.x, item.location.y, item.location.z);
					int distance_i = distance;
					distance_components[distance_i] = &item;
				}
			}
			chosen_components.push_back(distance_components.begin()->second);
			item_component * item_comp = ECS->find_entity_component<
					item_component>(
					distance_components.begin()->second->entity_id);
			item_comp->claimed = true;
			game_engine->messaging->add_message<item_changed_message>(
					item_changed_message(item_comp->entity_id));
		}

		// For each required component
		for (available_item_t * component : chosen_components)
		{
			const int16_t component_x = component->location.x;
			const int16_t component_y = component->location.y;
			const uint8_t component_z = component->location.z;
			const int component_id = component->entity_id;

			job.steps.push_back(ai::job_step_t
			{ ai::MOVE_TO, component_x, component_y, component_z, 0, false, "",
					0 });
			job.steps.push_back(ai::job_step_t
			{ ai::PICK_UP_COMPONENT, component_x, component_y, component_z,
					component_id, false, "", 0 });
			job.steps.push_back(ai::job_step_t
			{ ai::MOVE_TO, msg.x, msg.y, msg.z, 0, false, "", 0 });
			job.steps.push_back(ai::job_step_t
			{ ai::DROP_OFF_COMPONENT, msg.x, msg.y, msg.z, component_id, false,
					"", 0 });
		}

		// Add the skill check
		job.steps.push_back(ai::job_step_t
		{ ai::CONSTRUCT_WITH_SKILL, msg.x, msg.y, msg.z, 0, true,
				"Construction", 0, 12 });

		// Once the skill has been passed
		//	For each component
		for (available_item_t * component : chosen_components)
		{
			const int component_id = component->entity_id;
			job.steps.push_back(ai::job_step_t
			{ ai::DESTROY_COMPONENT, msg.x, msg.y, msg.z, component_id, false,
					"", 0 });
		}
		job.steps.push_back(ai::job_step_t
		{ ai::CONVERT_PLACEHOLDER_STRUCTURE, msg.x, msg.y, msg.z, 0, false, "",
				entity_id });

		// Add it to the help-wanted board
		ai::jobs_board[job.job_id] = job;
	}
}

void cordex_ai_system::handle_tree_chop_orders()
{
	vector<chop_order_message> * orders = game_engine->messaging->get_messages_by_type<chop_order_message>();
	for (chop_order_message &msg : *orders)
	{
		msg.deleted = true; // No repeats

		// Create a job
		ai::job_t job;
		job.job_id = ai::get_next_job_id();
		job.current_step = 0;
		job.assigned_to = 0;
		job.type = ai::CHOP_TREE;

		// Sub tasks:
		// Find axe
		auto finder = inventory.find("Fire Axe");
		if (finder == inventory.end())
		{
			std::cout << "Unable to locate an axe.\n";
			return; // Can't do it!
		}
		std::map<int, available_item_t *> distance_components;
		for (available_item_t item : finder->second)
		{
			item_component * item_comp = ECS->find_entity_component<
					item_component>(item.entity_id);
			if (!item_comp->claimed)
			{
				//float distance = std::sqrt( (std::abs(msg.x - item.location.first)*std::abs(msg.x - item.location.first)) + (std::abs(msg.y - item.location.second)*std::abs(msg.y - item.location.second)));
				float distance = geometry::distance3d(msg.x, msg.y, msg.z,
						item.location.x, item.location.y, item.location.z);
				int distance_i = distance;
				distance_components[distance_i] = &item;
			}
		}
		available_item_t * axe = distance_components.begin()->second;
		item_component * item_comp = ECS->find_entity_component<item_component>(
				distance_components.begin()->second->entity_id);
		item_comp->claimed = true;
		game_engine->messaging->add_message<item_changed_message>(
				item_changed_message(item_comp->entity_id));

		// Navigate to axe
		const int16_t component_x = axe->location.x;
		const int16_t component_y = axe->location.y;
		const uint8_t component_z = axe->location.z;
		const int component_id = axe->entity_id;

		job.steps.push_back(
				ai::job_step_t
				{ ai::MOVE_TO, component_x, component_y, component_z, 0, false,
						"", 0 });
		job.steps.push_back(ai::job_step_t
		{ ai::PICK_UP_COMPONENT, component_x, component_y, component_z,
				component_id, false, "", 0 });

		// Navigate to tree
		job.steps.push_back(ai::job_step_t
		{ ai::MOVE_TO, msg.x, msg.y, msg.z, 0, false, "", 0 });

		// Skill roll
		job.steps.push_back(ai::job_step_t
		{ ai::CONSTRUCT_WITH_SKILL, msg.x, msg.y, msg.z, 0, true, "Lumberjack",
				0, 15 });

		// Destroy tree
		job.steps.push_back(ai::job_step_t{ ai::DESTROY_TREE, msg.x, msg.y, msg.z, msg.tree_id, false, "", 0 });

		// Add wood
		job.steps.push_back(ai::job_step_t
		{ ai::CREATE_WOOD, msg.x, msg.y, msg.z, msg.tree_id, false, "", 0 });

		// Drop axe
		job.steps.push_back(ai::job_step_t
		{ ai::DROP_OFF_TOOL, msg.x, msg.y, msg.z, component_id, false, "", 0 });

		// Add it to the help-wanted board
		ai::jobs_board[job.job_id] = job;
	}
}

void cordex_ai_system::handle_reaction_orders()
{
	vector<reaction_order_message> * orders = game_engine->messaging->get_messages_by_type<reaction_order_message>();
	for (reaction_order_message &msg : *orders)
	{
		msg.deleted = true; // No dupes!

		// Create a job
		ai::job_t job;
		job.job_id = ai::get_next_job_id();
		job.current_step = 0;
		job.assigned_to = 0;
		job.type = ai::PERFORM_REACTION;

		// Find the workshop
		position_component3d * workshop_pos = ECS->find_entity_component<
				position_component3d>(msg.workshop_id);

		// Build component vector
		vector<string> components;
		vector<string> results;
		for (std::pair<string, int> &tmp : msg.requirements)
		{
			for (int i = 0; i < tmp.second; ++i)
				components.push_back(tmp.first);
		}
		for (std::pair<string, int> &tmp : msg.creates)
		{
			for (int i = 0; i < tmp.second; ++i)
				results.push_back(tmp.first);
		}

		// Choose components
		vector<available_item_t *> chosen_components;
		for (const string &component : components)
		{
			auto finder = inventory.find(component);
			if (finder == inventory.end())
				return; // Can't do it!
			// Find the closest candidate
			std::map<int, available_item_t *> distance_components;
			for (available_item_t item : finder->second)
			{
				item_component * item_comp = ECS->find_entity_component<
						item_component>(item.entity_id);
				if (!item_comp->claimed)
				{
					//float distance = std::sqrt( (std::abs(workshop_pos->x - item.location.first)*std::abs(workshop_pos->x - item.location.first)) + (std::abs(workshop_pos->y - item.location.second)*std::abs(workshop_pos->y - item.location.second)));
					float distance = geometry::distance3d(workshop_pos->pos.x,
							workshop_pos->pos.y, workshop_pos->pos.z,
							item.location.x, item.location.y, item.location.z);
					int distance_i = distance;
					distance_components[distance_i] = &item;
				}
			}
			chosen_components.push_back(distance_components.begin()->second);
			item_component * item_comp = ECS->find_entity_component<
					item_component>(
					distance_components.begin()->second->entity_id);
			item_comp->claimed = true;
			game_engine->messaging->add_message<item_changed_message>(
					item_changed_message(item_comp->entity_id));
		}

		// For each required component
		for (available_item_t * component : chosen_components)
		{
			const int16_t component_x = component->location.x;
			const int16_t component_y = component->location.y;
			const uint8_t component_z = component->location.z;
			const int component_id = component->entity_id;

			job.steps.push_back(ai::job_step_t
			{ ai::MOVE_TO, component_x, component_y, component_z, 0, false, "",
					0 });
			job.steps.push_back(ai::job_step_t
			{ ai::PICK_UP_COMPONENT, component_x, component_y, component_z,
					component_id, false, "", 0 });
			job.steps.push_back(ai::job_step_t
			{ ai::MOVE_TO, workshop_pos->pos.x, workshop_pos->pos.y,
					workshop_pos->pos.z, 0, false, "", 0 });
			job.steps.push_back(ai::job_step_t
			{ ai::DROP_OFF_COMPONENT, workshop_pos->pos.x, workshop_pos->pos.y,
					workshop_pos->pos.z, component_id, false, "", 0 });
		}

		// Add the skill check
		// TODO: We need to track a skill here
		job.steps.push_back(ai::job_step_t
		{ ai::CONSTRUCT_WITH_SKILL, workshop_pos->pos.x, workshop_pos->pos.y,
				workshop_pos->pos.z, 0, true, "Construction", 0, 12 });

		// Power drain
		job.steps.push_back(
				ai::job_step_t
				{ ai::CONSUME_POWER, workshop_pos->pos.x, workshop_pos->pos.y,
						workshop_pos->pos.z, msg.power_drain, true,
						"Construction", 0, 0 });

		// Destroy the components
		for (available_item_t * component : chosen_components)
		{
			const int component_id = component->entity_id;
			job.steps.push_back(ai::job_step_t
			{ ai::DESTROY_COMPONENT, workshop_pos->pos.x, workshop_pos->pos.y,
					workshop_pos->pos.z, component_id, false, "", 0 });
		}

		// Create the result
		for (const string &new_component : results)
		{
			job.steps.push_back(ai::job_step_t
			{ ai::CREATE_ITEM, workshop_pos->pos.x, workshop_pos->pos.y,
					workshop_pos->pos.z, 0, false, new_component, 0 });
		}

		// Add it to the help-wanted board
		ai::jobs_board[job.job_id] = job;
	}
}

void cordex_ai_system::handle_dig_orders()
{
	vector<dig_order_message> * orders = game_engine->messaging->get_messages_by_type<dig_order_message>();

	for (dig_order_message &msg : *orders)
	{
		msg.deleted = true; // No repeats

		// Create a job
		ai::job_t job;
		job.job_id = ai::get_next_job_id();
		job.current_step = 0;
		job.assigned_to = 0;
		job.type = ai::DIG;

		// Sub tasks:
		// Find axe
		auto finder = inventory.find("Pickaxe");
		if (finder == inventory.end())
		{
			std::cout << "Unable to locate a pickaxe.\n";
			return; // Can't do it!
		}
		std::map<int, available_item_t *> distance_components;
		for (available_item_t item : finder->second)
		{
			item_component * item_comp = ECS->find_entity_component<
					item_component>(item.entity_id);
			if (!item_comp->claimed)
			{
				//float distance = std::sqrt( (std::abs(msg.x - item.location.first)*std::abs(msg.x - item.location.first)) + (std::abs(msg.y - item.location.second)*std::abs(msg.y - item.location.second)));
				float distance = geometry::distance3d(msg.x, msg.y, msg.z, item.location.x, item.location.y, item.location.z);
				int distance_i = distance;
				distance_components[distance_i] = &item;
			}
		}
		available_item_t * axe = distance_components.begin()->second;
		item_component * item_comp = ECS->find_entity_component<item_component>(distance_components.begin()->second->entity_id);
		item_comp->claimed = true;
		game_engine->messaging->add_message<item_changed_message>(item_changed_message(item_comp->entity_id));

		// Navigate to axe
		const int16_t component_x = axe->location.x;
		const int16_t component_y = axe->location.y;
		const uint8_t component_z = axe->location.z;
		const int component_id = axe->entity_id;

		job.steps.push_back(ai::job_step_t{ ai::MOVE_TO, component_x, component_y, component_z, 0, false, "", 0 });
		job.steps.push_back(ai::job_step_t{ ai::PICK_UP_COMPONENT, component_x, component_y, component_z, component_id, false, "", 0 });

		// Navigate to dig site
		job.steps.push_back(ai::job_step_t{ ai::MOVE_TO, msg.x, msg.y, msg.z, 0, false, "", 0 });

		// Skill roll
		job.steps.push_back(ai::job_step_t{ ai::CONSTRUCT_WITH_SKILL, msg.x, msg.y, msg.z, 0, true, "Mining", 0, 15 });

		// Clear dug-out area
		switch (msg.dig_type) {
		case dig_types::MINE : job.steps.push_back(ai::job_step_t{ ai::DIG_TILE, msg.x, msg.y, msg.z, 0, false, "", 0 }); break;
		case dig_types::CHANNEL : job.steps.push_back(ai::job_step_t{ ai::CHANNEL_TILE, msg.x, msg.y, msg.z, 0, false, "", 0 }); break;
		case dig_types::RAMP : job.steps.push_back(ai::job_step_t{ ai::DIG_RAMP, msg.x, msg.y, msg.z, 0, false, "", 0 }); break;
		case dig_types::DOWNSTAIRS : job.steps.push_back(ai::job_step_t{ ai::DIG_TILE_DOWNSTAIRS, msg.x, msg.y, msg.z, 0, false, "", 0 }); break;
		case dig_types::UPSTAIRS : job.steps.push_back(ai::job_step_t{ ai::DIG_TILE_UPSTAIRS, msg.x, msg.y, msg.z, 0, false, "", 0 }); break;
		case dig_types::UPDOWNSTAIRS : job.steps.push_back(ai::job_step_t{ ai::DIG_TILE_UPDOWNSTAIRS, msg.x, msg.y, msg.z, 0, false, "", 0 }); break;
		}

		// Drop axe
		job.steps.push_back(ai::job_step_t{ ai::DROP_OFF_TOOL, msg.x, msg.y, msg.z, component_id, false, "", 0 });

		// Add it to the help-wanted board
		ai::jobs_board[job.job_id] = job;
	}
}
