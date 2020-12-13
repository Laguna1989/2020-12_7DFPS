#ifndef GAME_HUD_HPP_INCLUDEGUARD
#define GAME_HUD_HPP_INCLUDEGUARD

#include "GameObject.hpp"
#include "GameProperties.hpp"
#include "SmartText.hpp"
#include <string>

class Hud : public jt::GameObject {
public:
    Hud();

    void setHealth(float v);
    void setAmmo(int v);

private:
    float m_health { GP::PlayerStartingHealth() };
    int m_ammo { GP::PlayerStartingAmmo() };

    jt::SmartText::Sptr m_scoreHealth;
    jt::SmartText::Sptr m_scoreP2Text;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;

    void doCreate() override;
};

#endif
