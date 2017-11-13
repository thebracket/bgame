#pragma once

#include "../stdafx.h"

struct item_def_t;
struct stockpile_def_t;

item_def_t * get_item_def(const std::string tag);
stockpile_def_t * get_stockpile_def(const int tag);

int get_clothing_stockpile();
void set_clothing_stockpile(const int n);

void each_stockpile(const std::function<void(stockpile_def_t *)> func);

void read_items() noexcept;
void sanity_check_items() noexcept;
void read_stockpiles() noexcept;
void sanity_check_stockpiles() noexcept;

