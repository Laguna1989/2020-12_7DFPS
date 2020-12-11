#ifndef SHOT_HPP_GUARD
#define SHOT_HPP_GUARD

#include "Box2DObject.hpp"
#include "SmartAnimation.hpp"
#include <memory>

class StateGame;

class Shot : public jt::Box2DObject {
public:
    Shot(std::shared_ptr<b2World> world, b2BodyDef* def);

    std::shared_ptr<jt::SmartAnimation> getAnim();

    void collide();

    void setState(std::weak_ptr<StateGame> state);

private:
    std::shared_ptr<jt::SmartAnimation> m_anim;
    std::weak_ptr<StateGame> m_state {};
    bool m_hasCollided { false };

    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    void doCreate() override;
};

#endif
