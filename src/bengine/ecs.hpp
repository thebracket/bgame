#pragma once

#include <vector>
#include <tuple>
#include <map>
#include <bitset>
#include <array>
#include <type_traits>
#include <memory>
#include "../components/all_components.hpp"
#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/memory.hpp>
#include "ecs_helper.hpp"

namespace bengine
{

	class entity_t;

	template<class ... Components>
	class ecs_t
	{
	public:

		ecs_t() noexcept
		{
			setup_index(storage, std::index_sequence_for<Components...>{});
		}		

		entity_t * entity(const int &id) noexcept
		{
			entity_t * result = nullptr;
			const auto finder = entities.find(id);
			if (finder == entities.end()) return result;
			//if (finder->second->is_deleted) return result;
			result = finder->second.get();
			return result;
		}

		void delete_entity(const int &entity_id) noexcept
		{
			const auto finder = entities.find(entity_id);
			if (finder != entities.end())
			{
				//finder->second.is_deleted = true;
				const auto bfinder = component_mask.find(entity_id);
				if (bfinder != component_mask.end())
				{
					bfinder->second.reset();
				}
				delete_components_for_entity(storage, std::index_sequence_for<Components...>{}, entity_id);
				entities.erase(entity_id);
			}
		}

		void delete_all_entities() noexcept
		{
			for (auto & e : entities)
			{
				delete_entity(e.first);
			}
		}

		template <class ComponentToDelete>
		void delete_component(const int &id) noexcept
		{
			const auto family_id = get_component_family_id<ComponentToDelete>();

			const auto entity_finder = entities.find(id);
			if (entity_finder != entities.end())
			{
				// The entity exists, so clear the mask setting
				const auto bitset_finder = component_mask.find(id);
				if (bitset_finder != component_mask.end())
				{
					bitset_finder->second.reset(family_id);
				}

				// Now flag the component as deleted
				std::get<std::pair<size_t, std::map<int, ComponentToDelete>>>(storage).second.erase(id);
			}
		}		

		template <class Component>
		constexpr size_t get_component_family_id() const noexcept
		{
			static_assert(!contains<Component>(), "ECS type unregistered.");
			return std::get<std::pair<size_t, std::map<int, Component>>>(storage).first;
		}

		template <class Component>
		std::vector<entity_t *> entities_with_component() noexcept
		{
			std::vector<entity_t *> result;
			auto family_id = get_component_family_id<Component>();
			for (const auto &bitset : component_mask)
			{
				if (bitset.second.test(family_id)) result.emplace_back(entity(bitset.first));
			}

			return result;
		}

		template <class Component>
		Component * entity_component(const int &id) noexcept
		{
			const auto finder = std::get<std::pair<size_t, std::map<int, Component>>>(storage).second.find(id);
			if (finder == std::get<std::pair<size_t, std::map<int, Component>>>(storage).second.end())
			{
				return nullptr;
			} else
			{
				return &finder->second;
			}
		}

		template <class ... ComponentsToIterate>
		bool entity_has_all_of(const int &entity_id, const std::array<size_t, sizeof...(ComponentsToIterate)> &family_ids)
		{
			const auto bitset_finder = component_mask.find(entity_id);
			if (bitset_finder != component_mask.end()) {

				for (const auto &bs : family_ids)
				{
					if (!bitset_finder->second.test(bs)) {
						return false;
					}
				}

				return true;
			}
			return false;
		}

		template <class ComponentToIgnore>
		bool entity_does_not_have(const int &entity_id)
		{
			const auto bitset_finder = component_mask.find(entity_id);
			if (bitset_finder != component_mask.end()) {

				const auto family_id = get_component_family_id<ComponentToIgnore>();
				if (!bitset_finder->second.test(family_id)) {
					return true;
				}
				return false;
			}
			return false;
		}

		template <class ... ComponentsToIterate>
		std::vector<std::tuple<entity_t *, ComponentsToIterate *...>> materialize_view()
		{
			std::vector<std::tuple<entity_t *, ComponentsToIterate *...>> result;
			std::array<size_t, sizeof...(ComponentsToIterate)> to_test{ get_component_family_id<ComponentsToIterate>()... };
			for (auto &entity : entities)
			{
				const auto entity_id = entity.first;
				if (entity_has_all_of<ComponentsToIterate...>(entity.second, to_test))
				{
					result.emplace_back(
						std::make_tuple(&entity.second, entity_component<ComponentsToIterate>(entity_id)...)
					);
				}
			}
			return result;
		}

		template <class ... ComponentsToIterate>
		std::vector<std::tuple<entity_t *, ComponentsToIterate *...>> materialize_view_predicate(const std::function<bool(entity_t &,ComponentsToIterate &...)> predicate)
		{
			std::vector<std::tuple<entity_t *, ComponentsToIterate *...>> result;
			std::array<size_t, sizeof...(ComponentsToIterate)> to_test{ get_component_family_id<ComponentsToIterate>()... };
			for (auto &entity : entities)
			{
				const auto entity_id = entity.first;
				if (entity_has_all_of<ComponentsToIterate...>(entity.first, to_test) && predicate(*entity.second, *entity_component<ComponentsToIterate>(entity_id)...))
				{
					result.emplace_back(
						std::make_tuple(entity.second.get(), entity_component<ComponentsToIterate>(entity_id)...)
					);
				}
			}
			return result;
		}

		template <class ComponentToIgnore, class ... ComponentsToIterate>
		std::vector<std::tuple<entity_t *, ComponentsToIterate *...>> materialize_view_excluding()
		{
			std::vector<std::tuple<entity_t *, ComponentsToIterate *...>> result;
			std::array<size_t, sizeof...(ComponentsToIterate)> to_test{ get_component_family_id<ComponentsToIterate>()... };
			for (auto &entity : entities)
			{
				const auto entity_id = entity.first;
				if (entity_does_not_have<ComponentToIgnore>(entity.first) && entity_has_all_of<ComponentsToIterate...>(*entity.second, to_test))
				{
					// It matches!
					result.emplace_back(
						std::make_tuple(entity.second.get(), entity_component<ComponentsToIterate>(entity_id)...)
					);
				}
			}
			return result;
		}

		template <class ... ComponentsToIterate, typename Function>
		void each(const Function &callback) noexcept
		{
			std::array<size_t, sizeof...(ComponentsToIterate)> to_test{ get_component_family_id<ComponentsToIterate>()... };
			for (auto &entity : entities)
			{
				const auto entity_id = entity.first;
				if (entity_has_all_of<ComponentsToIterate...>(entity.first, to_test))
				{
					callback(*entity.second, *entity_component<ComponentsToIterate>(entity_id)...);
				}
			}
		}

		template <class ... ComponentsToIterate, typename Predicate, typename Function>
		void each_if(const Predicate & predicate, const Function &callback) noexcept
		{
			std::array<size_t, sizeof...(ComponentsToIterate)> to_test{ get_component_family_id<ComponentsToIterate>()... };
			for (auto &entity : entities)
			{
				const auto entity_id = entity.first;
				if (entity_has_all_of<ComponentsToIterate...>(entity.first, to_test) && predicate(*entity.second, *entity_component<ComponentsToIterate>(entity_id)...))
				{
					callback(*entity.second, *entity_component<ComponentsToIterate>(entity_id)...);
				}
			}
		}

		template <class ComponentToIgnore, class ... ComponentsToIterate, typename Function>
		void each_without(const Function &callback) noexcept
		{
			std::array<size_t, sizeof...(ComponentsToIterate)> to_test{ get_component_family_id<ComponentsToIterate>()... };
			for (auto &entity : entities)
			{
				const auto entity_id = entity.first;
				if (entity_does_not_have<ComponentToIgnore>(entity.first) && entity_has_all_of<ComponentsToIterate...>(entity.first, to_test))
				{
					// It matches!
					callback(*entity.second, *entity_component<ComponentsToIterate>(entity_id)...);
				}
			}
		}

		template <class ComponentToIgnore, class ComponentToIgnore2, class ... ComponentsToIterate, typename Function>
		void each_without_both(const Function &callback) noexcept
		{
			std::array<size_t, sizeof...(ComponentsToIterate)> to_test{ get_component_family_id<ComponentsToIterate>()... };
			for (auto &entity : entities)
			{
				const auto entity_id = entity.first;
				if (entity_does_not_have<ComponentToIgnore>(entity_id) && entity_does_not_have<ComponentToIgnore2>(entity_id) && entity_has_all_of<ComponentsToIterate...>(entity_id, to_test))
				{
					// It matches!
					callback(*entity.second, *entity_component<ComponentsToIterate>(entity_id)...);
				}
			}
		}

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(entity_counter, entities, storage, component_mask); // serialize things by passing them to the archive
		}

		int entity_counter = 0;
		std::map<int, std::unique_ptr<entity_t>> entities;
		std::tuple<std::pair<size_t, std::map<int, Components>>...> storage;
		std::map<int, std::bitset<sizeof...(Components)>> component_mask;
	};

	class entity_t
	{
	public:
		int id = 0;
		bool is_deleted = false;
		impl::my_ecs_t * ecs = nullptr;

		template <class Component>
		entity_t * assign(Component && component) noexcept
		{
			if (is_deleted)
			{
				std::cout << "You cannot assign to a deleted entity.\n";
				std::terminate();
			}

			auto * target_map = &std::get<std::pair<size_t, std::map<int, Component>>>(ecs->storage).second;
			target_map->insert(std::make_pair(id, component));
			const auto family_id = ecs->get_component_family_id<Component>();
			ecs->component_mask[id].set(family_id);
			return this;
		}

		template <class Component>
		Component * component() noexcept
		{
			return ecs->entity_component<Component>(id);
		}

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(id, is_deleted); // serialize things by passing them to the archive
		}
	};

	inline entity_t * create_entity(impl::my_ecs_t * ecs) noexcept
	{
		const auto new_id = ecs->entity_counter++;
		ecs->entities.insert(std::make_pair(new_id, std::make_unique<entity_t>()));
		ecs->entities[new_id]->id = new_id;
		ecs->entities[new_id]->is_deleted = false;
		ecs->entities[new_id]->ecs = ecs;
		return ecs->entities[new_id].get();
	}

}