#pragma once

// Standing orders
namespace standing_orders {

constexpr uint8_t SO_IDLE_STATIC = 0;
constexpr uint8_t SO_IDLE_WANDER = 1;
constexpr uint8_t SO_IDLE_WANDER_CLOSE = 2;

constexpr uint8_t SO_WILDLIFE_IGNORE = 0;
constexpr uint8_t SO_WILDLIFE_KILL = 1;

constexpr uint8_t SO_UPGRADE_NEVER = 0;
constexpr uint8_t SO_UPGRADE_NEARBY = 1;
constexpr uint8_t SO_UPGRADE_ALWAYS = 2;

}
