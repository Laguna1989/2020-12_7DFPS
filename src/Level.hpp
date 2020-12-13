#ifndef LEVEL_GUARD_HPP
#define LEVEL_GUARD_HPP

#include "Box2DObject.hpp"
#include "SmartSprite.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

// fwd decl
class Player;

class LevelWall : public jt::Box2DObject {
public:
    LevelWall(std::shared_ptr<b2World> world, b2BodyDef const* def);
    // for dynamically changing the level
    unsigned int m_tx { 0 };
    unsigned int m_ty { 0 };
    std::size_t m_forceID { 0 };
};

class Level {
public:
    enum class TileType : std::uint8_t { EMPTY, WALL, FORCE };

    void loadLevel(std::string const& fileName, std::shared_ptr<b2World> world);

    TileType getTileTypeAt(unsigned int x, unsigned int y) const;

    jt::vector2u getLevelSizeInTiles() const;

    jt::vector2 getPlayerStartPositionInTiles() const;
    float getPlayerStartAngle() const;
    std::vector<jt::vector2> getEnemyPositions() const;
    jt::vector2 getSymbolPosition() const;
    std::map<std::size_t, jt::vector2> getKeyPositions() const;
    void PopForceField(std::size_t forceFieldID);

    std::vector<jt::vector2> getAmmoPackPositions() const;
    std::vector<jt::vector2> getHealthPackPositions() const;

private:
    std::vector<TileType> m_levelVec {};
    jt::vector2u m_levelSize { 0, 0 };
    jt::vector2u m_playerStart { 0, 0 };
    float m_playerStartAngle { 0.0f };

    std::size_t posToIndex(unsigned int x, unsigned int y) const;
    std::vector<jt::vector2> m_enemyPositions;

    jt::vector2 m_symbolPosition;

    std::vector<jt::vector2> m_ammoPackPositions;
    std::vector<jt::vector2> m_healthPackPositions;

    std::map<std::size_t, jt::vector2> m_keyPositions;
    std::map<std::size_t, std::vector<std::shared_ptr<LevelWall>>> m_ForceFields;
};

#endif
