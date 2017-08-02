#pragma once

struct emit_particles_message : public rltk::base_message_t {
    emit_particles_message() {}
    emit_particles_message(const uint8_t &T, const int &X, const int &Y, const int &Z) : type(T), x(X), y(Y), z(Z), dx(0), dy(0), dz(0) {}
    emit_particles_message(const uint8_t &T, const int &X, const int &Y, const int &Z, const int &DX, const int &DY, const int &DZ) : type(T), x(X), y(Y), z(Z), dx(DX), dy(DY), dz(DZ) {}
    uint8_t type;
    int x,y,z;
    int dx,dy,dz;
};