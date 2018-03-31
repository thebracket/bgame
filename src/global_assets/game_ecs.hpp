#pragma once

#include "../bengine/ecs.hpp"
#include <memory>

namespace bengine {

	namespace impl {
		extern my_ecs_t ecs;
	}

	inline bengine::entity_t * entity(const int &id) noexcept
	{
		return impl::ecs.entity(id);
	}

	inline bengine::entity_t * create_entity() noexcept
	{
		return create_entity(&impl::ecs);
	}

	inline void delete_entity(const int &id) noexcept
	{
		impl::ecs.delete_entity(id);
	}

	inline void delete_all_entities() noexcept
	{
		impl::ecs.delete_all_entities();
	}

	template<class C>
	inline void delete_component(const int &id) noexcept
	{
		impl::ecs.delete_component<C>(id);
	}

	template <class C>
	inline std::vector<bengine::entity_t *> entities_with_component() noexcept
	{
		return impl::ecs.entities_with_component<C>();
	}

	template <class...Components, class F>
	inline void each(const F &func) noexcept
	{
		impl::ecs.each<Components...>(func);
	}

	template <class...Components, class P, class F>
	inline void each_if(const P &pred, const F  &func) noexcept
	{
		impl::ecs.each_if<Components...>(pred, func);
	}

	template <class Exclude, class ... Components, class F>
	inline void each_without(const F &func) noexcept
	{
		impl::ecs.each_without<Exclude, Components...>(func);
	}

	template <class Exclude, class Exclude2, class ... Components, class F>
	inline void each_without_both(const F &func) noexcept
	{
		impl::ecs.each_without_both<Exclude, Exclude2, Components...>(func);
	}

	void ecs_save(std::unique_ptr<std::ofstream> &lbfile) noexcept;
	void ecs_load(std::unique_ptr<std::ifstream> &lbfile) noexcept;
}