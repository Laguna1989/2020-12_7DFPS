#include "Hud.hpp"
#include "GameBase.hpp"
#include "GameProperties.hpp"
#include "color.hpp"

Hud::Hud() = default;

void Hud::doCreate()
{
    m_scoreHealth = std::make_shared<jt::SmartText>();
    m_scoreHealth->loadFont("assets/font.ttf", 32, getGame()->getRenderTarget());
    m_scoreHealth->setColor(jt::color { 248, 249, 254 });
    m_scoreHealth->update(0.0f);
    m_scoreHealth->SetTextAlign(jt::SmartText::TextAlign::LEFT);
    m_scoreHealth->setPosition({ 20, (GP::GetWindowSize().y() / GP::GetZoom() - 40.0f) });

    m_scoreP2Text = std::make_shared<jt::SmartText>();
    m_scoreP2Text->loadFont("assets/font.ttf", 32, getGame()->getRenderTarget());
    m_scoreP2Text->setColor(jt::color { 248, 249, 254 });
    m_scoreP2Text->update(0.0f);
    m_scoreP2Text->SetTextAlign(jt::SmartText::TextAlign::LEFT);
    m_scoreP2Text->setPosition({ 650 / 2 + 10, 325 });
}

void Hud::doUpdate(float const elapsed)
{
    if (m_health >= 0) {
        m_scoreHealth->setText(std::to_string(static_cast<int>(m_health)));
    }
    if (m_ammo >= 0) {
        m_scoreP2Text->setText(std::to_string(m_ammo));
    }

    m_scoreHealth->update(elapsed);
    m_scoreP2Text->update(elapsed);
}

void Hud::doDraw() const
{
    if (m_health >= 0) {
        m_scoreHealth->draw(getGame()->getRenderTarget());
    }
    if (m_ammo >= 0) {
        m_scoreP2Text->draw(getGame()->getRenderTarget());
    }
}

void Hud::setHealth(float v) { m_health = v; }
void Hud::setAmmo(int v) { m_ammo = v; }
