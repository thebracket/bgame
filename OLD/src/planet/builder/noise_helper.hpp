#pragma once

constexpr int REGION_FRACTION_TO_CONSIDER = 64;
constexpr unsigned int octaves = 5;
constexpr float persistence = 0.5F;
constexpr float frequency = 2.0F;

inline uint8_t noise_to_planet_height(const float &n) {
	return static_cast<uint8_t>((n + 1.0F) * 150.0F);
}

constexpr double NOISE_SIZE = 384.0F;

inline float noise_x(const int world_x, const int region_x) {
	const float big_x = (world_x * WORLD_WIDTH) + region_x;
	return (big_x / ((float)WORLD_WIDTH * (float)REGION_WIDTH)) * NOISE_SIZE;
}

inline float noise_y(const int world_y, const int region_y) {
	const float big_y = (world_y * WORLD_HEIGHT) + region_y;
	return (big_y / ((float)WORLD_HEIGHT * (float)REGION_HEIGHT)) * NOISE_SIZE;
}

