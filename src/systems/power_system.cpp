#include "power_system.hpp"
#include "../messages/messages.hpp"
#include "../game_globals.hpp"
#include <sstream>

void power_system::calculate(int consumption) {
    designations->total_capacity = 0;
    int total_always_on = 0;
    int total_solar = 0;
    each<construct_power_t>([&total_always_on, &total_solar] (entity_t &e, construct_power_t &p) {
        designations->total_capacity += p.storage_capacity;
        total_always_on += p.generation_always;
        total_solar += p.generation_solar;
    });

    designations->current_power += total_always_on;
    if (calendar->hour > 6 && calendar->hour < 19) designations->current_power += total_solar;
    if (designations->current_power > designations->total_capacity) designations->current_power = designations->total_capacity;    
    designations->current_power -= consumption;

    if (has_run) emit(power_changed_message{});
}

void power_system::configure() {
    subscribe_mbox<power_consumed_message>();

    subscribe<tick_message>([this](tick_message &msg) {
        std::queue<power_consumed_message> * messages = mbox<power_consumed_message>();
        int consumption = 0;
        while (!messages->empty()) {
            power_consumed_message e = messages->front();
            messages->pop();
            consumption += e.amount;
        }
        calculate(consumption);              
    });
}

void power_system::update(double time_ms) {
    if (!has_run) {
        has_run = true;
        calculate(0);
    }
    if (designations->current_power < 1) {
        std::cout << "TODO: Everyone just died.\n";
    }
}
