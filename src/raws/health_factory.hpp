#pragma once

#include "../components/health.hpp"
#include "../raws/species.hpp"

health_t create_health_component_sentient(const raw_species_t * species, const int base_hp) noexcept;
health_t create_health_component_settler(const std::string &tag, const int base_hp) noexcept;
health_t create_health_component_creature(const std::string &tag) noexcept;
