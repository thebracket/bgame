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
    // Using atomic exchange to avoid stepping on other writes, in an attempt to keep the total
    // water amount the same.
    atomicAdd(waterLevel.wl[idx], -amount_to_move);
    atomicAdd(waterLevel.wl[dest_idx], amount_to_move);
    //waterLevel.wl[idx] = my_level - amount_to_move;
    //waterLevel.wl[dest_idx] = their_level + amount_to_move;
}

uint distance_to_fall(uint idx) {
    uint distance = 0;
    bool falling = true;
    while (falling) {
        ++distance;
        idx -= 65536;
        if (idx < 65536 || is_solid(idx) || waterLevel.wl[idx]==10) {
            falling = false;
            --distance;
        }
    }
    return distance;
}

void main() {
    uint array_index = gl_GlobalInvocationID.x;

    uint idx = waterParticles.wp[array_index];
    uint idx_below = idx - 65536;
    uint idx_above = idx + 65536;
    uint idx_west = idx - 1;
    uint idx_east = idx + 1;
    uint idx_north = idx - 256;
    uint idx_south = idx + 256;
    uvec3 pos = idxmap(idx);
    uint my_water = atomicAdd(waterLevel.wl[idx], 0);
    uint water_n = atomicAdd(waterLevel.wl[idx_north], 0);
    uint water_s = atomicAdd(waterLevel.wl[idx_south], 0);
    uint water_e = atomicAdd(waterLevel.wl[idx_east], 0);
    uint water_w = atomicAdd(waterLevel.wl[idx_west], 0);

    if (is_solid(idx)) {
        // Water shouldn't have ever been here!
        waterLevel.wl[idx] = 0;
    } else {
        if (pos.z > 1 && !is_solid(idx_below) && !can_stand_here(idx) && waterLevel.wl[idx_below] < 10) {
            uint distance = distance_to_fall(idx);
            if (distance > 0) water_swap(idx, idx - distance *65536);
        } else if (my_water > 1) {
            // We're not falling, so we need to consider exits.
            uint north = pos.y > 1 && !is_solid(idx_north) ? water_n : 10;
            uint west = pos.x > 1 && !is_solid(idx_west) ? water_w : 10;
            uint south = pos.y < 255 && !is_solid(idx_south) ? water_s : 10;
            uint east = pos.x < 255 && !is_solid(idx_east) ? water_e : 10;

            if (north==10 && west==10 && south==10 && east==10) {
                // The tile is completely full with no available exits
                //if (pos.z < 254 && !is_solid(idx_above) && !can_stand_here(idx_above)) {
                //    water_swap(idx, idx_above);
                //}
            } else {
                // We need to find the smaller of north/west/south/east
                uint smallest = min(north, west);
                smallest = min(smallest, south);
                smallest = min(smallest, east);

                if (north == smallest) {
                    water_swap(idx, idx_north);
                } else if (west == smallest) {
                    water_swap(idx, idx_west);
                } else if (south == smallest) {
                    water_swap(idx, idx_south);
                } else {
                    water_swap(idx, idx_east);
                }
            }

            /*
            if (north < 10 && west < 10 && south < 10 && east < 10) {
                // There's a potential exit
                if (north < west && north < south && north < east) {
                    water_swap(idx, idx_north);
                }
                else if (west < north && west < south && west < east) {
                    water_swap(idx, idx_west);
                }
                else if (south < north && south < west && south < east) {
                    water_swap(idx, idx_south);
                }
                else if (east < north && east < west && east < south) {
                    water_swap(idx, idx_south);
                }
                // If we've got to here, then it's equal so we don't move.
            } // It's all full! This is where we should consider water pressure.
            */

            //if (north != 10) water_swap(idx, idx_north);
        }
    }
}