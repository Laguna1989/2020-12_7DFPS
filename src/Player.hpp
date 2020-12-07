#ifndef PLAYER_GUARD_HPP
#define PLAYER_GUARD_HPP

#include "Box2DObject.hpp"
#include "vector.hpp"

class Player : public jt::Box2DObject {
public:
    Player(std::shared_ptr<b2World> world, const b2BodyDef* def);

    float angle;

private:
    void doUpdate(float const elapsed) override;
    void doDraw() const override {};
    void doCreate() override {};
};

#endif
