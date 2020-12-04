#ifndef PLAYER_GUARD_HPP
#define PLAYER_GUARD_HPP

#include "GameObject.hpp"
#include "vector.hpp"

struct Player : public jt::GameObject {
    jt::vector2 position;
    float angle;

    void doUpdate(float elapsed) override;
};

#endif
