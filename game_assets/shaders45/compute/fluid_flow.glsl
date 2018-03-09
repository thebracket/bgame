#version 430

uniform uint offset;

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout (std430, binding=5) buffer water_level
{ 
    uint wl[];
} waterLevel;

layout (std430, binding=4) buffer terrain_flags
{ 
    uint flags[];
} terrainFlags;

uint mapidx(uvec3 position) {
    uint xi = uint(position.x);
    uint yi = uint(position.y);
    uint zi = uint(position.z);

    return (zi * 65536) + (yi * 256) + xi;
}

uint mapidx(uint xi, uint yi, uint zi) {
    return (zi * 65536) + (yi * 256) + xi;
}

bool is_solid(uint idx) {
    return ((terrainFlags.flags[idx] & 1024) == 1024);
}

void main() {
	uvec3 position = gl_GlobalInvocationID.xyz;
    uvec3 pos = position;
    pos.z = offset;
	uint idx = mapidx(pos);
    bool below = false;
    if (idx > 65536) below = true;
    uint idx_below = idx - 65536;

    if (!is_solid(idx)) {
        if (waterLevel.wl[idx] > 0) {
            if (below && !is_solid(idx_below) && waterLevel.wl[idx_below] < 10) {
                uint landing_zone = pos.z-1;
                bool hit_ground = false;
                while (!hit_ground) {
                    if (landing_zone == 0) hit_ground = true;
                    if ((terrainFlags.flags[mapidx(uvec3(pos.x, pos.y, landing_zone))] & 1024)>0) hit_ground = true;
                    if (!hit_ground) --landing_zone;
                }

                --waterLevel.wl[idx];
                ++waterLevel.wl[mapidx(uvec3(pos.x, pos.y, landing_zone))];
            }
        }
    }

    /*
	if (waterLevel.wl[idx] > 0 && waterLevel.wl[idx_below] < 10 && (terrainFlags.flags[idx_below] & 1024) == 0) {
        waterLevel.wl[idx]--;
        waterLevel.wl[idx_below]++;
	}
    */
}