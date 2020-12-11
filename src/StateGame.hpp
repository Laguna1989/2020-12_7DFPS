#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "ContactListener.hpp"
#include "Enemy.hpp"
#include "GameState.hpp"
#include "Hud.hpp"
#include "IntroText.hpp"
#include "Level.hpp"
#include "ObjectGroup.hpp"
#include "Player.hpp"
#include "Shot.hpp"
#include "SmartShape.hpp"
#include "Symbol.hpp"
#include "TweenAlpha.hpp"
#include "Wall.hpp"
#include <iostream>
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class SmartText;
class SmartSprite;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    StateGame(int levelID);

    void SpawnShot();

private:
    std::shared_ptr<jt::SmartShape> m_background;
    std::shared_ptr<jt::SmartShape> m_overlay;

    std::vector<std::shared_ptr<Wall>> m_walls;
    std::shared_ptr<Player> m_player;
    mutable std::shared_ptr<jt::SmartShape> m_sky;
    mutable std::shared_ptr<jt::SmartShape> m_floor;
    std::shared_ptr<Hud> m_hud;

    std::vector<std::shared_ptr<Enemy>> m_enemies;
    std::shared_ptr<Symbol> m_symbol;
    std::shared_ptr<jt::ObjectGroup<Shot>> m_shots;

    std::shared_ptr<jt::SmartShape> m_mapBackground;
    mutable std::shared_ptr<jt::SmartShape> m_mapWall;
    std::shared_ptr<jt::SmartShape> m_mapPlayer;

    std::shared_ptr<b2World> m_world { nullptr };

    std::shared_ptr<IntroText> m_introText;

    std::shared_ptr<Level> m_level;
    std::string m_levelFileName;
    int m_levelID;

    bool m_ending { false };

    MyContactListener m_contactListener;

    std::shared_ptr<jt::TweenBase> m_tween1;
    std::shared_ptr<jt::TweenBase> m_tween2;

    bool m_drawMiniMap { true };

    void doCreate() override;

    virtual void doCreateInternal();

    virtual void doInternalUpdate(float const elapsed) override;
    virtual void doInternalDraw() const override;

    void calculateWallScales();
    void drawMap() const;
};

#endif
