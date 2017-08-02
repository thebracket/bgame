#pragma once

#include <rltk.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

using namespace rltk;

struct claimed_t {

    claimed_t() {}
    claimed_t(const std::size_t &id) : claimed_by(id) {}
    std::size_t claimed_by = 0;

    template<class Archive>
    void serialize(Archive & archive)
    {
        archive( claimed_by );
    }
};

CEREAL_REGISTER_TYPE(rltk::impl::component_store_t<rltk::impl::component_t<claimed_t>>)
