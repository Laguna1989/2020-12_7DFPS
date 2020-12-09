#ifndef GAME_HUD_HPP_INCLUDEGUARD
#define GAME_HUD_HPP_INCLUDEGUARD

#include "GameObject.hpp"
#include "SmartText.hpp"
#include <string>

class Hud : public jt::GameObject {
public:
    Hud();

private:
    float m_health { 100 };
    int m_scoreP2 { -1 };

    jt::SmartText::Sptr m_scoreHealth;
    jt::SmartText::Sptr m_scoreP2Text;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;

    void doCreate() override;
};

#endif
