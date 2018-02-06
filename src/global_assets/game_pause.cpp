#include "game_pause.hpp"

pause_mode_t pause_mode = PAUSED;
bool major_tick = false;
bool slow_tick = false;
bool day_elapsed = false;
bool hour_elapsed = false;
