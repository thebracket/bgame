#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

enum trigger_type { trigger_cage, trigger_stonefall, trigger_blade };

struct entry_trigger_t {
    bool active = true;
    trigger_type type;

    entry_trigger_t() {}
    entry_trigger_t(const trigger_type &t) : type(t) {}
    entry_trigger_t(const trigger_type &t, const bool &enabled) : active(enabled), type(t) {}

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( active, type ); // serialize things by passing them to the archive
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<entry_trigger_t>>)
