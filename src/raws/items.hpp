#pragma once

#include <string>
#include <functional>

struct item_def_t;
struct stockpile_def_t;

item_def_t * get_item_def(const std::string &tag) noexcept;
stockpile_def_t * get_stockpile_def(const int tag) noexcept;

int get_clothing_stockpile() noexcept;
void set_clothing_stockpile(const int n) noexcept;

void each_stockpile(const std::function<void(stockpile_def_t *)> &func) noexcept;

void read_items() noexcept;
void sanity_check_items() noexcept;
void read_stockpiles() noexcept;
void sanity_check_stockpiles() noexcept;

