#pragma once

constexpr int REGION_FRACTION_TO_CONSIDER = 64;
constexpr int octaves = 5;
constexpr double persistence = 0.5;
constexpr double frequency = 2.0;

inline uint8_t noise_to_planet_height(const double &n) {
	return (n + 1.0F) * 150.0;
}

constexpr double NOISE_SIZE = 256.0F;

inline double noise_x(const int world_x, const int region_x) {
	const double big_x = (world_x * WORLD_WIDTH) + region_x;
	return (big_x / ((double)WORLD_WIDTH * (double)REGION_WIDTH)) * NOISE_SIZE;
}

inline double noise_y(const int world_y, const int region_y) {
	const double big_y = (world_y * WORLD_HEIGHT) + region_y;
	return (big_y / ((double)WORLD_HEIGHT * (double)REGION_HEIGHT)) * NOISE_SIZE;
}

