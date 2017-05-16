#include "particle_system.hpp"
#include "../../messages/messages.hpp"
#include "../../components/smoke_emitter.hpp"
#include "../../main/game_rng.hpp"
#include "../../main/game_region.hpp"

using namespace rltk;

std::vector<particle_t> particles;

void particle_system::configure() {
    system_name = "Particle System";
    subscribe_mbox<emit_particles_message>();
}

void particle_system::update(const double ms) {
    each<smoke_emitter_t, position_t>([] (entity_t &e, smoke_emitter_t &s, position_t &pos) {
        if (rng.roll_dice(1,6)<4) emit_deferred(emit_particles_message{1, pos.x, pos.y, pos.z}); 
    });


    std::queue<emit_particles_message> * emitters = mbox<emit_particles_message>();
	while (!emitters->empty()) {
        emit_particles_message msg = emitters->front();
		emitters->pop();

        switch (msg.type) {
        case PARTICLE_SMOKE : particles.push_back(particle_t{ msg.x, msg.y, msg.z, rltk::vchar{ 176, rltk::colors::GREY, rltk::colors::BLACK }, 1, 20 }); break;
        case PARTICLE_MIASMA : particles.push_back(particle_t{ msg.x, msg.y, msg.z, rltk::vchar{ 176, rltk::colors::GREEN, rltk::colors::BLACK }, 2, 20 }); break;
        case PARTICLE_PROJECTILE : particles.push_back(particle_t{ msg.x, msg.y, msg.z, rltk::vchar{ '*', rltk::colors::GREY, rltk::colors::BLACK }, 3, msg.dx, msg.dy, msg.dz }); break;
        case PARTICLE_BOOM_FUSE : particles.push_back(particle_t{ msg.x, msg.y, msg.z, rltk::vchar{ '!', rltk::colors::RED, rltk::colors::BLACK }, 4, 20}); break;
        case PARTICLE_BOOM : particles.push_back(particle_t{ msg.x, msg.y, msg.z, rltk::vchar{ 176, rltk::lerp(rltk::colors::RED, rltk::colors::YELLOW, (float)rng.roll_dice(1,100)/100.0f), rltk::colors::BLACK}, 5, 20}); break;
        }
	}

    subscribe<slow_tick_message>([this](slow_tick_message &msg) {
        tick = true;
    });

    bool updated = false;
    for (particle_t &p : particles) {
        updated = true;

        if (p.mode < 3 && tick) {
            // Smoke and miasma
            p.lifespan--;
            if (p.lifespan < 0) p.deleteme = true;
            if (p.x < 1 || p.x > REGION_WIDTH-1 || p.y < 1 || p.y > REGION_HEIGHT-1 || p.z < 1 || p.z > REGION_DEPTH-1) {
                p.deleteme = true;
            }
            if (!p.deleteme) {
                if (p.z < REGION_DEPTH - 2 && !current_region->solid[mapidx(p.x, p.y, p.z + 1)]) {
                    ++p.z;
                }
                int direction = rng.roll_dice(1, 6);
                switch (direction) {
                    case 1 :
                        if (p.x > 1 && !current_region->solid[mapidx(p.x - 1, p.y, p.z)]) --p.x;
                        break;
                    case 2 :
                        if (p.x < REGION_WIDTH - 2 && !current_region->solid[mapidx(p.x + 1, p.y, p.z)]) ++p.x;
                        break;
                    case 3 :
                        if (p.y > 1 && !current_region->solid[mapidx(p.x, p.y - 1, p.z)]) --p.y;
                        break;
                    case 4 :
                        if (p.y < REGION_HEIGHT - 2 && !current_region->solid[mapidx(p.x, p.y + 1, p.z)]) ++p.x;
                        break;
                }
            }
        } else if (p.mode == 3) {
            // Projectiles move towards their target
            if (p.x < p.dest_x) ++p.x;
            if (p.x > p.dest_x) --p.x;
            if (p.y < p.dest_y) ++p.y;
            if (p.y > p.dest_y) --p.y;
            if (p.z < p.dest_z) ++p.z;
            if (p.z > p.dest_z) --p.z;
            if (p.x == p.dest_x && p.y == p.dest_y && p.z == p.dest_z) p.deleteme = true;
        } else {
            --p.lifespan;
            if (p.lifespan < 0) p.deleteme = true;
        }
    }
    if (updated) {
        particles.erase(std::remove_if(particles.begin(), particles.end(), [] (particle_t p) { return p.deleteme; }), particles.end());
        emit(renderables_changed_message{});
    }
    tick = false;
}
