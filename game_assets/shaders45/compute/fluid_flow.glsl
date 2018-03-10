#version 430

uniform uint offset;

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

layout (std430, binding=6) readonly buffer water_particles
{ 
    uint wp[];
} waterParticles;

layout (std430, binding=5) buffer water_level
{ 
    uint wl[];
} waterLevel;

layout (std430, binding=4) readonly buffer terrain_flags
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

uvec3 idxmap(uint idx) {
    uint z = idx / (256 * 256);
    idx -= (z * 256 * 256);

    uint y = idx / 256;
    idx -= (y * 256);

    uint x = idx;

    return uvec3(x, y, z);
}

bool is_solid(uint idx) {
    return ((terrainFlags.flags[idx] & 1024) == 1024);
}

bool can_stand_here(uint idx) {
    return ((terrainFlags.flags[idx] & 64) == 64);
}

void water_swap(uint idx, uint dest_idx)
{
    uint my_level = waterLevel.wl[idx];
    uint their_level = waterLevel.wl[dest_idx];
    uint amount_to_move = 1;
    waterLevel.wl[idx] = my_level - amount_to_move;
    waterLevel.wl[dest_idx] = their_level + amount_to_move;
}

void main() {
    uint array_index = gl_GlobalInvocationID.x;

    uint idx = waterParticles.wp[array_index];
    uint idx_below = idx - 65536;
    uint idx_west = idx - 1;
    uint idx_east = idx + 1;
    uint idx_north = idx - 256;
    uint idx_south = idx + 256;
    uvec3 pos = idxmap(idx);

    if (is_solid(idx)) {
        // Water shouldn't have ever been here!
        waterLevel.wl[idx] = 0;
    } else {
        if (pos.z > 1 && !is_solid(idx_below) && !can_stand_here(idx) && waterLevel.wl[idx_below] < 10) {
            water_swap(idx, idx_below);
        } else if (pos.x > 1 && !is_solid(idx_west) && waterLevel.wl[idx_west] < 10)
        {
            water_swap(idx, idx_west);
        }
        else if (pos.x < 253 && !is_solid(idx_east) && waterLevel.wl[idx_east] < 10)
        {
            // It can go west
            water_swap(idx, idx_east);
        }
        else if (pos.y > 1 && !is_solid(idx_north) && waterLevel.wl[idx_north] < 10)
        {
            // It can go west
            water_swap(idx, idx_north);
        }
        else if (pos.y < 253 && !is_solid(idx_south) && waterLevel.wl[idx_south] < 10)
        {
            // It can go west
            water_swap(idx, idx_south);
        }
    }
}