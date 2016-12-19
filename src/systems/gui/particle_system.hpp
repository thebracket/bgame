#pragma once

#include <rltk.hpp>
#include <vector>

constexpr uint8_t PARTICLE_SMOKE = 1;
constexpr uint8_t PARTICLE_MIASMA = 2;
constexpr uint8_t PARTICLE_PROJECTILE = 3;

struct particle_t {
    particle_t() {}
    particle_t(const int &X, const int &Y, const int &Z, const rltk::vchar &g, const uint8_t m, const int &life) :
        x(X), y(Y), z(Z), glyph(g), lifespan(life), mode(m), dest_x(0), dest_y(0), dest_z(0) {}
    particle_t(const int &X, const int &Y, const int &Z, const rltk::vchar &g, const uint8_t m, const int &dx, const int &dy, const int &dz) :
        x(X), y(Y), z(Z), glyph(g), lifespan(0), mode(m), dest_x(dx), dest_y(dy), dest_z(dz) {}
    int x,y,z;
    rltk::vchar glyph;
    int lifespan;
    uint8_t mode;
    int dest_x, dest_y, dest_z;
    bool deleteme = false;
};

extern std::vector<particle_t> particles;

class particle_system : public rltk::base_system {
public:
	virtual void update(const double duration_ms) override final;
	virtual void configure() override final;
private:
    bool tick = false;
};
