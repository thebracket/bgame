#pragma once

#include <vector>
#include <tuple>
#include <algorithm>
#include <map>
#include <bitset>
#include <array>
#include <type_traits>
#include "../components/all_components.hpp"
#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/tuple.hpp>

namespace bengine
{

	class entity_t;

	static size_t type_counter = 0;

	template<typename T, typename... Ts>
	constexpr bool contains()
	{
		return std::disjunction_v<std::is_same<T, Ts>...>;
	}

	template<class Component>
	struct component_holder
	{
		bool is_deleted = false;
		int entity_id = 0;
		Component c;

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(is_deleted, entity_id, c); // serialize things by passing them to the archive
		}
	};

	template<class ... Components>
	class ecs_t
	{
	public:
		template<typename ...Ts, size_t... I>
		void setup_index(std::tuple<std::pair<size_t, std::vector<Ts>>...> &tuple, std::index_sequence<I...>) noexcept
		{
			(void)(std::initializer_list<int> {
				(std::get<I>(tuple).first = I, 0)...
			});
		}

		template<typename ...Ts, size_t... I>
		void delete_components_for_entity(std::tuple<std::pair<size_t, std::vector<Ts>>...> &tuple, std::index_sequence<I...>, const int &entity_id) noexcept
		{
			(void)(std::initializer_list<int> {
				(std::for_each(std::get<I>(tuple).second.begin(), std::get<I>(tuple).second.end(), [&entity_id](auto &c) { if (c.entity_id == entity_id) c.is_deleted = true; }), 0)...
			});
		}

		ecs_t() noexcept
		{
			setup_index(storage, std::index_sequence_for<Components...>{});
		}


		entity_t * create_entity() noexcept
		{
			const auto new_id = entity_counter++;
			entities.insert(std::make_pair(new_id, entity_t{ new_id, false, this }));
			return &entities[new_id];
		}

		entity_t * entity(const int &id) noexcept
		{
			entity_t * result = nullptr;
			auto finder = entities.find(id);
			if (finder == entities.end()) return result;
			if (finder->second.is_deleted) return result;
			result = &(finder->second);
			return result;
		}

		void delete_entity(const int &entity_id) noexcept
		{
			const auto finder = entities.find(entity_id);
			if (finder != entities.end())
			{
				finder->second.is_deleted = true;
				const auto bfinder = component_mask.find(entity_id);
				if (bfinder != component_mask.end())
				{
					bfinder->second.reset();
				}
				delete_components_for_entity(storage, std::index_sequence_for<Components...>{}, entity_id);
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
				std::for_each(
					std::get < std::pair<size_t, std::vector<component_holder<ComponentToDelete>>>>(storage).second.begin(),
					std::get < std::pair<size_t, std::vector<component_holder<ComponentToDelete>>>>(storage).second.end(),
					[&id](auto &ch) { if (ch.entity_id == id) ch.is_deleted = true; }
				);
			}
		}		

		template <class Component>
		constexpr size_t get_component_family_id() const noexcept
		{
			static_assert(!contains<Component>(), "ECS type unregistered.");
			return std::get<std::pair<size_t, std::vector<component_holder<Component>>>>(storage).first;
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

		template <class Component, class Function>
		void all_components(const Function && f) noexcept
		{
			std::for_each(
				std::get<std::pair<size_t, std::vector<component_holder<Component>>>>(storage).second.begin(),
				std::get<std::pair<size_t, std::vector<component_holder<Component>>>>(storage).second.end(),
				[&f](auto & ch) { f(ch.c); }
			);
		}

		template <class Component>
		Component * entity_component(const int &id) noexcept
		{
			for (auto &ch : std::get<std::pair<size_t, std::vector<component_holder<Component>>>>(storage).second)
			{
				if (!ch.is_deleted && ch.entity_id == id) return &ch.c;
			}
			return nullptr;
		}

		template <class ... ComponentsToIterate>
		bool entity_has_all_of(const entity_t &entity, const std::array<size_t, sizeof...(ComponentsToIterate)> &family_ids)
		{
			const auto bitset_finder = component_mask.find(entity.id);
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
		bool entity_does_not_have(const entity_t &entity)
		{
			const auto bitset_finder = component_mask.find(entity.id);
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
				const auto entity_id = entity.second.id;
				if (!entity.second.is_deleted)
				{
					if (entity_has_all_of<ComponentsToIterate...>(entity.second, to_test))
					{
						result.emplace_back(
							std::make_tuple(&entity.second, entity_component<ComponentsToIterate>(entity_id)...)
						);
					}
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
				const auto entity_id = entity.second.id;
				if (!entity.second.is_deleted)
				{
					if (entity_has_all_of<ComponentsToIterate...>(entity.second, to_test) && predicate(entity.second, *entity_component<ComponentsToIterate>(entity_id)...))
					{
						result.emplace_back(
							std::make_tuple(&entity.second, entity_component<ComponentsToIterate>(entity_id)...)
						);
					}
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
				const auto entity_id = entity.second.id;
				if (!entity.second.is_deleted)
				{
					if (entity_does_not_have<ComponentToIgnore>(entity.second) && entity_has_all_of<ComponentsToIterate...>(entity.second, to_test))
					{
						// It matches!
						result.emplace_back(
							std::make_tuple(&entity.second, entity_component<ComponentsToIterate>(entity_id)...)
						);
					}
				}
			}
			return result;
		}

		template <class ... ComponentsToIterate, typename Function>
		void each(const Function &callback) noexcept
		{
			auto view = materialize_view<ComponentsToIterate...>();
			std::for_each(view.begin(), view.end(), [&callback](auto &tuple)
			{
				callback(*std::get<entity_t*>(tuple), *std::get<ComponentsToIterate *>(tuple)...);
			});
		}

		template <class ... ComponentsToIterate, typename Predicate, typename Function>
		void each_if(const Predicate & predicate, const Function &callback) noexcept
		{
			auto view = materialize_view_predicate<ComponentsToIterate...>(predicate);
			std::for_each(view.begin(), view.end(), [&callback, &view](auto &tuple)
			{
				callback(*std::get<entity_t*>(tuple), *std::get<ComponentsToIterate *>(tuple)...);
			});
		}

		template <class ComponentToIgnore, class ... ComponentsToIterate, typename Function>
		void each_without(const Function &callback) noexcept
		{
			auto view = materialize_view_excluding<ComponentToIgnore, ComponentsToIterate...>();
			std::for_each(view.begin(), view.end(), [&callback, &view](auto &tuple)
			{
				callback(*std::get<entity_t*>(tuple), *std::get<ComponentsToIterate *>(tuple)...);
			});
		}

		template<typename ...Ts, size_t... I>
		void garbage_collect_impl(std::tuple<std::pair<size_t, std::vector<Ts>>...> &tuple, std::index_sequence<I...>) noexcept
		{
			(void)(std::initializer_list<int> {
				(std::get<I>(tuple).second.erase(
					std::remove_if(std::get<I>(tuple).second.begin(), std::get<I>(tuple).second.end(), [](const auto &ch) { return ch.is_deleted; }),
					std::get<I>(tuple).second.end()
				)
					, 0)...
			});
		}

		void garbage_collect() noexcept
		{
			garbage_collect_impl(storage, std::index_sequence_for<Components...>{});
		}

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(entity_counter, entities, storage, component_mask); // serialize things by passing them to the archive
		}

		int entity_counter = 0;
		std::map<int, entity_t> entities;
		std::tuple<std::pair<size_t, std::vector<component_holder<Components>>>...> storage;
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

			component_holder<Component> ch{ false, id, component };
			auto * target_vector = &std::get<std::pair<size_t, std::vector<component_holder<Component>>>>(ecs->storage).second;
			target_vector->emplace_back(ch);
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

}