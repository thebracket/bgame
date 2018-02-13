#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include "../bengine/ecs_impl.hpp"

struct riding_t {
    std::size_t riding;
	riding_t() = default;
    explicit riding_t(const std::size_t carrier) noexcept : riding(carrier) {}

    
};
