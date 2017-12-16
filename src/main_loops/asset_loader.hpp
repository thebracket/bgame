#pragma once

namespace splash_screen {
	bool loader_tick();

	extern bool loaded_textures;
	extern bool started_telemetry;
	extern bool sent_telemetry;
	extern bool loaded_worldgen;
	extern bool loaded_chunk;
	extern int loaded_vox_count;

}