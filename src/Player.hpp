#ifndef PLAYER_GUARD_HPP
#define PLAYER_GUARD_HPP

#include "Box2DObject.hpp"
#include "GameProperties.hpp"
#include "vector.hpp"

class Player : public jt::Box2DObject {
public:
    float angle { 0.0f };
    Player(std::shared_ptr<b2World> world, const b2BodyDef* def);
    void handleInput(float elapsed);

    void setTakeInput(bool take);
    bool getTakeInput();

    bool getShootNow();

    void pickUpAmmo();
    void pickUpHealth();

    void TakeDamate(float dmg);
    float getHitPoints() const;
    int getAmmo() const;

private:
    bool m_takeInput { true };
    bool m_shootNow { false };

    float m_hitpoints { GP::PlayerStartingHealth() };
    int m_ammo { GP::PlayerStartingAmmo() };

    void doUpdate(float const elapsed) override;
    void doDraw() const override {};
    void doCreate() override {};
};

#endif
