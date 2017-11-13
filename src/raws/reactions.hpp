#pragma once

#include "../stdafx.h"

struct reaction_t; // Forward
struct graphviz_t; // Forward

reaction_t * get_reaction_def(const std::string tag);
std::vector<std::string> get_reactions_for_building(const std::string tag);
void each_reaction(const std::function<void(std::string, reaction_t *)> func);

void sanity_check_reactions() noexcept;
void read_reactions() noexcept;
void build_reaction_tree(graphviz_t * tree);