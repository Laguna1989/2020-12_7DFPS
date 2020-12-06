#ifndef PLAYER_GUARD_HPP
#define PLAYER_GUARD_HPP

#include "GameObject.hpp"
#include "vector.hpp"

class Player : public jt::GameObject {
public:
    jt::vector2 position;
    float angle;

private:
    void doUpdate(float elapsed) override;
};

#endif
