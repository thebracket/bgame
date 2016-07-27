#include "power_system.hpp"
#include "../messages/messages.hpp"
#include "../game_globals.hpp"
#include <sstream>

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

        int total_capacity = 0;
        int total_always_on = 0;
        int total_solar = 0;
        each<construct_power_t>([&total_capacity, &total_always_on, &total_solar] (entity_t &e, construct_power_t &p) {
            total_capacity += p.storage_capacity;
            total_always_on += p.generation_always;
            total_solar += p.generation_solar;
        });

        designations->current_power += total_always_on;
        if (calendar->hour > 6 && calendar->hour < 19) designations->current_power += total_solar;
        if (designations->current_power > total_capacity) designations->current_power = total_capacity;    
        designations->current_power -= consumption;
    });
}

void power_system::update(double time_ms) {
    if (designations->current_power < 1) {
        std::cout << "TODO: Everyone just died.\n";
    }

    int total_capacity = 0;
    int total_always_on = 0;
    int total_solar = 0;
    each<construct_power_t>([&total_capacity, &total_always_on, &total_solar] (entity_t &e, construct_power_t &p) {
        total_capacity += p.storage_capacity;
        total_always_on += p.generation_always;
        total_solar += p.generation_solar;
    });

    float power_pct = (float)designations->current_power / (float) total_capacity;

    if (power_pct < 0.5) {
        designations->alert_color = lerp(rltk::colors::RED, rltk::colors::ORANGE, power_pct*2.0F);
    } else {
        designations->alert_color = lerp(rltk::colors::ORANGE, rltk::colors::GREEN, (power_pct-0.5F)*2.0F);
    }

    std::stringstream ss;
    ss << "Power: " << designations->current_power << "/" << total_capacity;
    term(2)->print(20,0,ss.str(), designations->alert_color);
}
