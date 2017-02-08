#pragma once

#include <rltk.hpp>

class trigger_system : public rltk::base_system {
public:
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
    bool dirty = true;
    std::unordered_map<int, std::size_t> triggers;
    std::size_t trigger_id = 0;
    int selected_existing_link = 0;
    int new_link = 0;

    void manage_triggers();
    void edit_triggers();
    void pull_levers();
    void entry_trigger_firing();
    void pulled_levers();
};
