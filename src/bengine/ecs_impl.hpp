#pragma once

#include "cereal_include.hpp"
#include <cereal/types/memory.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/bitset.hpp>
#include <cereal/types/vector.hpp>
#include <queue>
#include <thread>
#include <mutex>
#include <algorithm>
#include <array>

namespace bengine {

    // Forward declarations
    class ecs_t;
    struct entity_t;
    extern ecs_t default_ecs;

    namespace impl {

        template<class C>
        inline void assign(ecs_t &ecs, entity_t &e, C component) noexcept;

        template <class C>
        inline C * component(ecs_t &ecs, entity_t &e) noexcept;

        inline void unset_component_mask(ecs_t &ecs, const std::size_t id, const std::size_t family_id, const bool delete_if_empty=false) noexcept;
    }

    /* Class for storing profile data */
    struct system_profiling_t {
        double last = 0.0;
        double best = 1000000.0;
        double worst = 0.0;
    };

    namespace impl {

        /*
         * Constant defining the maximum number of components to support. This sizes the bitsets,
         * so we don't want it to be much bigger than needed.
         */
        constexpr std::size_t MAX_COMPONENTS = 256;

        /*
         * If the current component set does not support serialization, this will become true.
         */
        static bool ecs_supports_serialization = true;

        /*
         * Base type for component handles. Exists so that we can have a vector of pointers to
         * derived classes. entity_id is included to allow a quick reference without a static cast.
         * type_counter is used as a static member, referenced from component_t - the handle class.
         */
        struct base_component_t {
            static std::size_t type_counter;
            std::size_t entity_id = 0;
            bool deleted = false;

            template<class Archive>
            void serialize(Archive & archive)
            {
                archive( entity_id, deleted ); // serialize things by passing them to the archive
            }
        };

        /*
         * component_t is a handle class for components. It inherits from base_component, allowing
         * the component store to have vectors of base_component_t *, where each type is a concrete
         * specialized class containing the component data. It does some magic with a static type_counter
         * to ensure that each instance with the same template type will have a unique family_id - this is
         * then used to reference the correct component store.
         */
        template<class C>
        struct component_t : public base_component_t {
            component_t() noexcept {
                data = C{};
                family();
            }
            explicit component_t(C comp) noexcept: data(comp) {
                family();
            }
            std::size_t family_id = 0;
            C data;

            template<class Archive>
            void serialize(Archive & archive)
            {
                archive( cereal::base_class<base_component_t>(this), family_id, data ); // serialize things by passing them to the archive
            }

            inline void family() {
                static auto family_id_tmp = base_component_t::type_counter++;
                family_id = family_id_tmp;
            }
        };

        /*
         * Base class for the component store. Concrete component stores derive from this.
         */
        struct base_component_store {
            virtual void erase_by_entity_id(ecs_t &ecs, const std::size_t &id)=0;
            virtual void really_delete()=0;
            virtual std::size_t size()=0;

            template<class Archive>
            void serialize(Archive & archive)
            {
            }
        };

        /*
         * Component stores are just a vector of type C (the component). They inherit from
         * base_component_store, to allow for a vector of base_component_store*, with each
         * casting to a concrete vector of that type. The types are indexed by the family_id
         * created for a type with component_t<C>. This guarantees that each component type
         * is stored in a big contiguous vector, with only one de-reference required to find
         * the right store.
         */
        template<class C>
        struct component_store_t : public base_component_store {
            std::vector<C> components;

            virtual void erase_by_entity_id(ecs_t &ecs, const std::size_t &id) override final {
                for (auto &item : components) {
                    if (item.entity_id == id) {
                        item.deleted=true;
                        impl::unset_component_mask(ecs, id, item.family_id);
                    }
                }
            }

            virtual void really_delete() override final {
                components.erase(std::remove_if(components.begin(), components.end(),
                                                [] (auto x) { return x.deleted; }),
                                 components.end());
            }

            virtual std::size_t size() override final {
                return components.size();
            }

            template<class Archive>
            void serialize(Archive & archive)
            {
                archive( cereal::base_class<base_component_store>(this), components ); // serialize things by passing them to the archive
            }

        };

    } // End impl namespace

    /*
     * All entities are of type entity_t. They should be created with create_entity (below).
     */
    struct entity_t {

        /*
         * Default constructor - use the next available entity id #.
         */
        entity_t() {
            ++entity_t::entity_counter;
            id = entity_t::entity_counter;
        }

        /*
         * Construct with a specified entity #. Moves the next available entity # to this id+1.
         */
        entity_t(const std::size_t ID) {
            entity_t::entity_counter = ID+1;
            id = ID;
        }

        /*
         * Static ID counter - used to ensure that entity IDs are unique. This would need to be atomic
         * in a threaded app.
         */
        static std::size_t entity_counter;

        /*
         * The entities ID number. Used to identify the entity. These should be unique.
         */
        std::size_t id;

        /*
         * Overload == and != to allow entities to be compared for likeness.
         */
        bool operator == (const entity_t &other) const { return other.id == id; }
        bool operator != (const entity_t &other) const { return other.id != id; }

        bool deleted = false;

        /*
         * A bitset storing whether or not an entity has each component type. These are set with the family_id
         * determined in the component_t system above.
         */
        std::bitset<impl::MAX_COMPONENTS> component_mask;

        /*
         * Assign a component to this entity. Determines the family_id of the component type, sets the bitmask to
         * include the component, marks the component as belonging to the entity, and puts it in the appropriate
         * component store.
         */
        template<class C>
        inline entity_t * assign(ecs_t &ecs, C component) noexcept {
            if (deleted)
            {
				std::cout << "Cannot assign to a deleted entity.\n";
				std::terminate();
            }
            impl::assign<C>(ecs, *this, component);
            return this;
        }

        template<class C>
        inline entity_t * assign(C component) {
            return assign<C>(default_ecs, component);
        }

        /*
         * Find a component of the specified type that belongs to the entity.
         */
        template <class C>
        inline C * component(ecs_t &ECS) noexcept {
            return impl::component<C>(ECS, *this);
        }

        template <class C>
        inline C * component() noexcept {
            return component<C>(default_ecs);
        }

        template<class Archive>
        void serialize(Archive & archive)
        {
            archive( component_mask, id, deleted ); // serialize things by passing them to the archive
        }
    };

    /*
 * Class that holds an entity-component-system. This was moved to a class to allow for multiple instances.
 */
    class ecs_t {
    public:
        /*
         * entity(ID) is used to reference an entity. So you can, for example, do:
         * entity(12)->component<position_component>()->x = 3;
         */
        entity_t * entity(const std::size_t id) noexcept;

        /*
         * Creates an entity with a new ID #. Returns a pointer to the entity, to enable
         * call chaining. For example create_entity()->assign(foo)->assign(bar)
         */
        entity_t * create_entity() noexcept;

        /*
         * Creates an entity with a specified ID #. You generally only do this during loading.
         */
        entity_t * create_entity(const std::size_t new_id) noexcept;

        /*
         * Marks an entity (specified by ID#) as deleted.
         */
        inline void delete_entity(const std::size_t id) noexcept {
            const auto e = entity(id);
            if (!e) return;

            e->deleted = true;
            for (auto &store : component_store) {
                if (store) store->erase_by_entity_id(*this, id);
            }
        }

        /*
         * Marks an entity as deleted.
         */
        inline void delete_entity(entity_t &e) noexcept {
            delete_entity(e.id);
        }

        /*
         * Deletes all entities
         */
        inline void delete_all_entities() noexcept  {
			for (auto &it : entity_store) {
                delete_entity(it.first);
            }
        }

        /*
         * Marks an entity's component as deleted.
         */
        template<class C>
        inline void delete_component(const std::size_t entity_id, bool delete_entity_if_empty=false) noexcept {
            const auto eptr = entity(entity_id);
            if (!eptr) return;
            const  auto e = *entity(entity_id);
            impl::component_t<C> temp(C{});
            if (!e.component_mask.test(temp.family_id)) return;
            for (impl::component_t<C> &component : static_cast<impl::component_store_t<impl::component_t<C>> *>(component_store[temp.family_id].get())->components) {
                if (component.entity_id == entity_id) {
                    component.deleted = true;
                    unset_component_mask(entity_id, temp.family_id, delete_entity_if_empty);
                }
            }
        }

        /*
         * Finds all entities that have a component of the type specified, and returns a
         * vector of pointers to the entities. It does not check for component deletion.
         */
        template<class C>
        inline std::vector<entity_t *> entities_with_component() noexcept {
            std::vector<entity_t *> result;
            impl::component_t<C> temp(C{});
            for (auto it=entity_store.begin(); it!=entity_store.end(); ++it) {
                if (!it->second.deleted && it->second.component_mask.test(temp.family_id)) {
                    result.push_back(&it->second);
                }
            }
            return result;
        }

        /*
         * all_components takes a component type, and calls the provided function/lambda on
         * every component, alongside it's owning entity. For example,
         * all_components<position>([] (entity_t &e, position &p) {...}) would execute the
         * function body (...) for every entity/component position pair.
         */
        template <class C>
        inline void all_components(typename std::function<void(entity_t &, C &)> func) noexcept {
            impl::component_t<C> temp(C{});
            for (impl::component_t<C> &component : static_cast<impl::component_store_t<impl::component_t<C>> *>(component_store[temp.family_id].get())->components) {
                entity_t e = *entity(component.entity_id);
                if (!e.deleted && !component.deleted) {
                    func(e, component.data);
                }
            }
        }

        /*
         * each, overloaded with a function/lambda that accepts an entity, will call the provided
         * function on _every_ entity in the system.
         */
        void each(std::function<void(entity_t &)> &&func) noexcept;

        /*
         * Variadic each. Use this to call a function for all entities having a discrete set of components. For example,
         * each<position, ai>([] (entity_t &e, position &pos, ai &brain) { ... code ... });
         */
        template <typename... Cs, typename F>
        inline void each(F callback) noexcept {
            std::array<size_t, sizeof...(Cs)> family_ids{ {impl::component_t<Cs>{}.family_id...} };
            for (auto it=entity_store.begin(); it!=entity_store.end(); ++it) {
                if (!it->second.deleted) {
                    auto matches = true;
                    for (const std::size_t &compare : family_ids) {
                        if (!it->second.component_mask.test(compare)) {
                            matches = false;
                            break;
                        }
                    }
                    if (matches) {
                        // Call the functor
                        callback(it->second, *it->second.component<Cs>()...);
                    }
                }
            }
        }

		/*
		* Variadic each, with an additional NOT check for the first type parameter. 
		* Use this to call a function for all entities having a discrete set of components. For example,
		* each_without<notme, position, ai>([] (entity_t &e, position &pos, ai &brain) { ... code ... });
		*/
		template <typename EXCLUDE, typename... Cs, typename F>
		inline void each_without(F callback) noexcept {
			std::array<size_t, sizeof...(Cs)> family_ids{ { impl::component_t<Cs>{}.family_id... } };
			auto excluder = impl::component_t<EXCLUDE>{}.family_id;
			for (auto it = entity_store.begin(); it != entity_store.end(); ++it) {
				if (!it->second.deleted) {
					if (!it->second.component_mask.test(excluder)) {
						bool matches = true;
						for (const std::size_t &compare : family_ids) {
							if (!it->second.component_mask.test(compare)) {
								matches = false;
								break;
							}
						}
						if (matches) {
							// Call the functor
							callback(it->second, *it->second.component<Cs>()...);
						}
					}
				}
			}
		}

        /*
         * Variadic each_if. Use this to call a function for all entities having a discrete set of components. For example,
         * each<position, ai>([] (entity_t &e, position &pos, ai &brain) { ... code returns true if needs processing ... },
         * [] (entity_t &e, position &pos, ai &brain) { ... code ... });
         */
        template <typename... Cs, typename P, typename F>
        inline void each_if(P&& predicate, F callback) noexcept {
            std::array<size_t, sizeof...(Cs)> family_ids{ {impl::component_t<Cs>{}.family_id...} };
            for (auto it=entity_store.begin(); it!=entity_store.end(); ++it) {
                if (!it->second.deleted) {
                    auto matches = true;
                    for (const std::size_t &compare : family_ids) {
                        if (!it->second.component_mask.test(compare)) {
                            matches = false;
                            break;
                        }
                    }
                    if (matches && predicate(it->second, *it->second.component<Cs>()...)) {
                        // Call the functor
                        callback(it->second, *it->second.component<Cs>()...);
                    }
                }
            }
        }

        /*
         * This should be called periodically to actually erase all entities and components that are marked as deleted.
         */
        inline void ecs_garbage_collect() noexcept {
            std::set<std::size_t> entities_to_delete;

            // Ensure that components are marked as deleted, and list out entities for erasure
			for (auto &it : entity_store) {
                if (it.second.deleted) {
                    for (auto &store : component_store) {
                        if (store) store->erase_by_entity_id(*this, it.second.id);
                    }
                    entities_to_delete.insert(it.second.id);
                }
            }

            // Actually delete entities
            for (const auto &id : entities_to_delete) entity_store.erase(id);

            // Now we erase components
            for (auto &store : component_store) {
                if (store) store->really_delete();
            }
        }

        void ecs_save(std::unique_ptr<std::ofstream> &lbfile) noexcept;

        void ecs_load(std::unique_ptr<std::ifstream> &lbfile) noexcept;

        // The ECS component store
        std::vector<std::unique_ptr<impl::base_component_store>> component_store;

        // The ECS entity store
        std::map<std::size_t, entity_t> entity_store;

        // Helpers
        void unset_component_mask(const std::size_t id, const std::size_t family_id, const bool delete_if_empty) noexcept {
            auto finder = entity_store.find(id);
            if (finder != entity_store.end()) {
                finder->second.component_mask.reset(family_id);
                if (delete_if_empty && finder->second.component_mask.none()) finder->second.deleted = true;
            }
        }

        /*
         * Cereal support for save/load
         */
        template<class Archive>
        void serialize(Archive & archive)
        {
            archive( entity_store, component_store, entity_t::entity_counter, impl::base_component_t::type_counter ); // serialize things by passing them to the archive
        }
    };

    namespace impl {
        template <class C>
        inline void assign(ecs_t &ecs, entity_t &e, C component) noexcept {
            impl::component_t<C> temp(component);
            temp.entity_id = e.id;
            if (ecs.component_store.size() < temp.family_id+1) {
                ecs.component_store.resize(temp.family_id+1);
            }
            if (!ecs.component_store[temp.family_id]) ecs.component_store[temp.family_id] = std::move(std::make_unique<impl::component_store_t<impl::component_t<C>>>());

            static_cast<impl::component_store_t<impl::component_t<C>> *>(ecs.component_store[temp.family_id].get())->components.push_back(temp);
            e.component_mask.set(temp.family_id);
        }

        template <class C>
        inline C * component(ecs_t &ecs, entity_t &e) noexcept {
            C * result = nullptr;
            if (e.deleted) return result;

            impl::component_t<C> temp(C{});
            if (!e.component_mask.test(temp.family_id)) return result;
            for (impl::component_t<C> &component : static_cast<impl::component_store_t<impl::component_t<C>> *>(ecs.component_store[temp.family_id].get())->components) {
                if (component.entity_id == e.id) {
                    result = &component.data;
                    return result;
                }
            }
            return result;
        }

        inline void unset_component_mask(ecs_t &ecs, const std::size_t id, const std::size_t family_id, bool const delete_if_empty) noexcept {
            ecs.unset_component_mask(id, family_id, delete_if_empty);
        }
    }

} // End RLTK namespace

CEREAL_REGISTER_ARCHIVE(bengine::ecs_t)
