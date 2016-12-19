#pragma once

#include <rltk.hpp>

class creature_attacks_system : public rltk::base_system {
public:
    virtual void update(const double duration_ms) override final;
    virtual void configure() override final;
private:
    void creature_attacks();
};
