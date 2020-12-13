#include "AmmoPack.hpp"
#include "Game.hpp"

AmmoPack::AmmoPack(jt::vector2 pos) { m_position = pos; }

std::shared_ptr<jt::SmartAnimation> AmmoPack::getAnim() { return m_anim; }

jt::vector2 AmmoPack::getPosition() const { return m_position; }

void AmmoPack::doCreate()
{
    m_anim = std::make_shared<jt::SmartAnimation>();
    m_anim->add("assets/ammopack.png", "idle", { 21, 24 }, { 0 }, 0.25f);
    m_anim->play("idle");

    m_glow = std::make_shared<jt::SmartSprite>();
    m_glow->loadSprite("#g#60#255");
    m_glow->setColor(jt::color { 255, 255, 255, 250 });
}

void AmmoPack::doUpdate(float const elapsed)
{
    m_anim->update(elapsed);
    m_glow->setPosition(m_anim->getPosition() + jt::vector2 { -20.0f, -20.0f });
    m_glow->update(elapsed);
    m_glow->setScale({ m_anim->getScale().x() * 0.75f, m_anim->getScale().y() });
}

void AmmoPack::doDraw() const { m_anim->draw(getGame()->getRenderTarget()); }
