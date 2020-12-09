#ifndef SHOT_HPP_GUARD
#define SHOT_HPP_GUARD

#include "Box2DObject.hpp"
#include "SmartAnimation.hpp"
#include <memory>

class Shot : public jt::Box2DObject {
public:
    Shot(std::shared_ptr<b2World> world, b2BodyDef* def);

    std::shared_ptr<jt::SmartAnimation> getAnim();

    void collide();

private:
    std::shared_ptr<jt::SmartAnimation> m_anim;
    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    void doCreate() override;
};

#endif
