#pragma once

constexpr int REGION_WIDTH=512;
constexpr int REGION_HEIGHT=512;
constexpr int REGION_DEPTH=128;
constexpr int REGION_TILES_COUNT = REGION_WIDTH * REGION_HEIGHT * REGION_DEPTH;

constexpr uint8_t CAN_GO_UP = 1;
constexpr uint8_t CAN_GO_DOWN = 2;
constexpr uint8_t CAN_GO_NORTH = 4;
constexpr uint8_t CAN_GO_EAST = 8;
constexpr uint8_t CAN_GO_SOUTH = 16;
constexpr uint8_t CAN_GO_WEST = 32;
constexpr uint8_t CAN_STAND_HERE = 64;
constexpr uint8_t CONSTRUCTION = 128;
