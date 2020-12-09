#include "Symbol.hpp"
#include "Game.hpp"

Symbol::Symbol(std::shared_ptr<b2World> world, const b2BodyDef* def)
    : Box2DObject { world, def }
{
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.1f, 0.1f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.9f;

    getB2Body()->CreateFixture(&fixtureDef);
}
std::shared_ptr<jt::SmartAnimation> Symbol::getSprite() { return m_sprite; }

void Symbol::doCreate()
{
    m_sprite = std::make_shared<jt::SmartAnimation>();
    m_sprite->add("assets/glyph1.png", "idle", { 21, 24 }, { 0, 0, 1, 1, 2, 3, 4, 5 }, 0.25f);
    m_sprite->play("idle");
}

void Symbol::doUpdate(float const elapsed) { m_sprite->update(elapsed); }
void Symbol::doDraw() const { m_sprite->draw(getGame()->getRenderTarget()); }
