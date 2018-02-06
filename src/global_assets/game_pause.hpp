#pragma once

enum pause_mode_t { RUNNING, PAUSED, ONE_STEP };

extern pause_mode_t pause_mode;
extern bool major_tick;
extern bool slow_tick;
extern bool day_elapsed;
extern bool hour_elapsed;
