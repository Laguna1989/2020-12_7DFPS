#ifndef ENEMY_GUARD_HPP
#define ENEMY_GUARD_HPP

#include "Box2DObject.hpp"
#include "SmartAnimation.hpp"
#include <memory>

class Enemy : public jt::Box2DObject {
public:
    Enemy(std::shared_ptr<b2World> world, const b2BodyDef* def);

    std::shared_ptr<jt::SmartAnimation> getAnimation();

private:
    std::shared_ptr<jt::SmartAnimation> m_anim;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    void doCreate() override;
};

#endif
