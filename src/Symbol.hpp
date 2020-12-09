#ifndef SYMBOL_GUARD_HPP
#define SYMBOL_GUARD_HPP

#include "Box2DObject.hpp"
#include "SmartAnimation.hpp"

class Symbol : public jt::Box2DObject {
public:
    Symbol(std::shared_ptr<b2World> world, const b2BodyDef* def);

    std::shared_ptr<jt::SmartAnimation> getSprite();

private:
    std::shared_ptr<jt::SmartAnimation> m_sprite;

    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    void doCreate() override;
};
#endif
