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
std::shared_ptr<jt::SmartAnimation> Symbol::getAnim() { return m_anim; }

void Symbol::doCreate()
{
    m_anim = std::make_shared<jt::SmartAnimation>();
    m_anim->add("assets/glyph1.png", "idle", { 21, 24 }, { 0, 0, 1, 1, 2, 3, 4, 5 }, 0.25f);
    m_anim->play("idle");

    m_glow = std::make_shared<jt::SmartSprite>();
    m_glow->loadSprite("#g#60#255");
    m_glow->setColor(jt::color { 255, 255, 255, 250 });
}

void Symbol::doUpdate(float const elapsed)
{
    m_anim->update(elapsed);
    m_glow->setPosition(m_anim->getPosition() + jt::vector2 { -20.0f, -20.0f });
    m_glow->update(elapsed);
    m_glow->setScale({ m_anim->getScale().x() * 0.75f, m_anim->getScale().y() });
}

void Symbol::doDraw() const
{
    m_glow->draw(getGame()->getRenderTarget());
    m_anim->draw(getGame()->getRenderTarget());
}
