#pragma once

#include "ecs_impl.hpp"

namespace bengine {

    /* Public interface to allow existing calls to continue to work */

    inline entity_t * entity(ecs_t &ECS, const std::size_t id) noexcept {
        return ECS.entity(id);
    }

    inline entity_t * entity(const std::size_t id) noexcept {
        return entity(default_ecs, id);
    }

    inline entity_t * create_entity(ecs_t &ecs) noexcept {
        return ecs.create_entity();
    }

    inline entity_t * create_entity() noexcept {
        return create_entity(default_ecs);
    }

    inline entity_t * create_entity(ecs_t &ecs, const std::size_t new_id) noexcept {
        return ecs.create_entity(new_id);
    }

    inline entity_t * create_entity(const std::size_t new_id) noexcept {
        return create_entity(default_ecs, new_id);
    }

    inline void delete_entity(ecs_t &ecs, const std::size_t id) noexcept {
        ecs.delete_entity(id);
    }

    inline void delete_entity(const std::size_t id) noexcept {
        delete_entity(default_ecs, id);
    }

    inline void delete_entity(ecs_t &ecs, entity_t &e) noexcept {
        ecs.delete_entity(e);
    }

    inline void delete_entity(entity_t &e) noexcept {
        delete_entity(default_ecs, e);
    }

    inline void delete_all_entities(ecs_t &ecs) noexcept {
		ecs.delete_all_entities();
    }

    inline void delete_all_entities() noexcept {
        delete_all_entities(default_ecs);
    }

    template<class C>
    inline void delete_component(ecs_t &ecs, const std::size_t entity_id, const bool delete_entity_if_empty=false) noexcept {
		ecs.delete_component<C>(entity_id, delete_entity_if_empty);
    }

    template<class C>
    inline void delete_component(const std::size_t entity_id, bool delete_entity_if_empty=false) noexcept {
        delete_component<C>(default_ecs, entity_id, delete_entity_if_empty);
    }

    template<class C>
    inline std::vector<entity_t *> entities_with_component(ecs_t &ecs) noexcept {
        return ecs.entities_with_component<C>();
    }

    template<class C>
    inline std::vector<entity_t *> entities_with_component() noexcept {
        return entities_with_component<C>(default_ecs);
    }

    template <class C>
    inline void all_components(ecs_t &ecs, typename std::function<void(entity_t &, C &)> func) noexcept {
		ecs.all_components<C>(func);
    }

    template <class C>
    inline void all_components(typename std::function<void(entity_t &, C &)> func) noexcept {
        all_components<C>(default_ecs, func);
    }

    template <typename... Cs, typename F>
    inline void each(ecs_t &ecs, F callback) noexcept {
		ecs.each<Cs...>(callback);
    }

    template <typename... Cs, typename F>
    inline void each(F callback) noexcept {
        each<Cs...>(default_ecs, callback);
    }

	template <typename EXCLUDE, typename... Cs, typename F>
	inline void each_without(ecs_t &ecs, F callback) noexcept {
		ecs.each_without<EXCLUDE, Cs...>(callback);
	}

	template <typename EXCLUDE, typename... Cs, typename F>
	inline void each_without(F callback) noexcept {
		each_without<EXCLUDE, Cs...>(default_ecs, callback);
	}

    template <typename... Cs, typename P, typename F>
    inline void each_if(ecs_t &ecs, P&& predicate, F callback) noexcept {
		ecs.each_if<Cs...>(predicate, callback);
    }

    template <typename... Cs, typename P, typename F>
    inline void each_if(P&& predicate, F callback) noexcept {
        each_if<Cs...>(default_ecs, predicate, callback);
    }

    inline void ecs_garbage_collect(ecs_t &ecs) noexcept {
        ecs.ecs_garbage_collect();
    }

    inline void ecs_garbage_collect() noexcept {
        ecs_garbage_collect(default_ecs);
    }

    inline void ecs_save(ecs_t &ecs, std::unique_ptr<std::ofstream> &lbfile) noexcept {
		ecs.ecs_save(lbfile);
    }

    inline void ecs_save(std::unique_ptr<std::ofstream> &lbfile) noexcept {
        ecs_save(default_ecs, lbfile);
    }

    inline void ecs_load(ecs_t &ecs, std::unique_ptr<std::ifstream> &lbfile) noexcept {
		ecs.ecs_load(lbfile);
    }

    inline void ecs_load(std::unique_ptr<std::ifstream> &lbfile) noexcept {
        ecs_load(default_ecs, lbfile);
    }
}
