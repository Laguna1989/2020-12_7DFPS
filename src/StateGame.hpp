#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "GameState.hpp"
#include "Player.hpp"
#include <iostream>
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class SmartShape;
class SmartSprite;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    StateGame() = default;

protected:
    std::shared_ptr<Hud> m_hud;

private:
    std::shared_ptr<jt::SmartShape> m_background;
    std::shared_ptr<jt::SmartShape> m_overlay;

    std::vector<std::shared_ptr<jt::SmartShape>> m_walls;
    std::shared_ptr<Player> m_player;
    std::shared_ptr<jt::SmartShape> m_mapBackground;
    mutable std::shared_ptr<jt::SmartShape> m_mapWall;
    std::shared_ptr<jt::SmartShape> m_mapPlayer;

    mutable std::shared_ptr<jt::SmartShape> m_sky;
    mutable std::shared_ptr<jt::SmartShape> m_floor;

    void doCreate() override;

    virtual void doCreateInternal();

    virtual void doInternalUpdate(float const elapsed) override;
    virtual void doInternalDraw() const override;

    void calculateWallScales();
    void drawMap() const;
};

#endif
