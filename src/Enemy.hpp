#ifndef ENEMY_GUARD_HPP
#define ENEMY_GUARD_HPP

#include "Box2DObject.hpp"
#include "SmartAnimation.hpp"
#include <memory>

class Enemy : public jt::Box2DObject {
public:
    Enemy(std::shared_ptr<b2World> world, const b2BodyDef* def);

    std::shared_ptr<jt::SmartAnimation> getAnimation();

    void TakeDamage(float damage);

    void setPlayerPosition(jt::vector2 playerPosition);

private:
    std::shared_ptr<jt::SmartAnimation> m_anim;
    jt::vector2 m_playerPos;
    bool m_active { false };

    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    void doCreate() override;
    float m_hitpoints;
};

#endif
