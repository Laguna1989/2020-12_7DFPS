#ifndef PLAYER_GUARD_HPP
#define PLAYER_GUARD_HPP

#include "Box2DObject.hpp"
#include "vector.hpp"

class Player : public jt::Box2DObject {
public:
    float angle { 0.0f };
    Player(std::shared_ptr<b2World> world, const b2BodyDef* def);
    void handleInput(float elapsed);

    void setTakeInput(bool take);
    bool getTakeInput();

    bool getShootNow();

private:
    bool m_takeInput { true };
    bool m_shootNow { false };

    void doUpdate(float const elapsed) override;
    void doDraw() const override {};
    void doCreate() override {};
};

#endif
