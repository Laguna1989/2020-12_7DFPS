#include "Key.hpp"
#include "Game.hpp"

Key::Key(jt::vector2 pos, std::size_t keyId)
{
    m_position = pos;
    m_keyID = keyId;
}
std::shared_ptr<jt::SmartAnimation> Key::getAnim() { return m_anim; }

jt::vector2 Key::getPosition() const { return m_position; }

void Key::doCreate()
{
    m_anim = std::make_shared<jt::SmartAnimation>();
    m_anim->add("assets/key.png", "idle", { 21, 24 }, { 0 }, 0.25f);
    m_anim->play("idle");

    m_glow = std::make_shared<jt::SmartSprite>();
    m_glow->loadSprite("#g#60#255");
    m_glow->setColor(jt::color { 255, 255, 255, 150 });
}

void Key::doUpdate(float const elapsed)
{
    m_anim->update(elapsed);

    float ofsx = m_anim->getLocalBounds().width() * m_anim->getScale().x() * 0.5f
        - m_glow->getLocalBounds().width() * m_glow->getScale().x() * 0.5f;
    float ofsy = m_anim->getLocalBounds().height() * m_anim->getScale().y() * 0.5f
        - m_glow->getLocalBounds().height() * m_glow->getScale().y() * 0.5f;

    m_glow->setPosition(m_anim->getPosition() + jt::vector2 { ofsx, ofsy });
    m_glow->update(elapsed);
    m_glow->setScale({ m_anim->getScale().x() * 0.75f, m_anim->getScale().y() * 0.75f });
}

void Key::doDraw() const
{
    m_glow->draw(getGame()->getRenderTarget());
    m_anim->draw(getGame()->getRenderTarget());
}

std::size_t Key::getKeyID() const { return m_keyID; }
