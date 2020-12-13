#include "Level.hpp"
#include "GameProperties.hpp"
#include "Player.hpp"
#include <iostream>

LevelWall ::LevelWall(std::shared_ptr<b2World> world, b2BodyDef const* def)
    : Box2DObject { world, def }
{
    b2PolygonShape box;
    box.SetAsBox(0.25f, 0.25f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;

    getB2Body()->CreateFixture(&fixtureDef);
}

void Level::loadLevel(std::string const& fileName, std::shared_ptr<b2World> world)
{
    auto level = std::make_shared<jt::SmartSprite>();
    level->loadSprite(fileName);
    m_levelVec.clear();
    m_levelSize = jt::vector2u { static_cast<unsigned int>(level->getLocalBounds().width()),
        static_cast<unsigned int>(level->getLocalBounds().height()) };
    m_levelVec.resize(m_levelSize.x() * m_levelSize.y());

    b2BodyDef wallBodyDef;
    wallBodyDef.type = b2_staticBody;
    wallBodyDef.fixedRotation = true;

    for (unsigned int j = 0; j != m_levelSize.y(); ++j) {
        for (unsigned int i = 0; i != m_levelSize.x(); ++i) {
            // get color at tile position i, j
            auto const c = level->getColorAtPixel({ i, j });

            if (c == jt::colors::White) {
                m_levelVec.at(posToIndex(i, j)) = Level::TileType::WALL;
                wallBodyDef.position = b2Vec2 { static_cast<float>(i), static_cast<float>(j) };
                LevelWall const w { world, &wallBodyDef };
            } else if (c.r() == 255U && c.b() == 255) {
                m_levelVec.at(posToIndex(i, j)) = Level::TileType::FORCE;
                wallBodyDef.position = b2Vec2 { static_cast<float>(i), static_cast<float>(j) };
                auto const w = std::make_shared<LevelWall>(world, &wallBodyDef);
                w->m_forceID = c.g();
                w->m_tx = i;
                w->m_ty = j;
                m_ForceFields[c.g()].push_back(w);

            } else {
                m_levelVec.at(posToIndex(i, j)) = Level::TileType::EMPTY;

                if (c.r() == 0 && c.g() == 255) {
                    // player start
                    m_playerStart = jt::vector2u { i, j };
                    m_playerStartAngle = 90.0f * c.b();
                } else if (c.r() == 255 && c.g() == 0 && c.b() == 0) {
                    // enemy position
                    m_enemyPositions.push_back(
                        jt::vector2 { static_cast<float>(i), static_cast<float>(j) });
                } else if (c.r() == 0 && c.g() == 0 && c.b() == 255) {
                    // symbol
                    m_symbolPosition = jt::vector2 { static_cast<float>(i), static_cast<float>(j) };
                } else if (c.r() == 255 && c.g() == 255) {
                    // key
                    m_keyPositions[c.b()]
                        = jt::vector2 { static_cast<float>(i), static_cast<float>(j) };
                } else if (c.r() == 100 && c.g() == 100 && c.b() == 255) {
                    // map item
                } else if (c.r() == 50 && c.g() == 255 && c.b() == 50) {
                    m_ammoPackPositions.push_back(
                        jt::vector2 { static_cast<float>(i), static_cast<float>(j) });
                    // ammunition
                } else if (c.r() == 255 && c.g() == 50 && c.b() == 50) {
                    // healthpack
                    m_healthPackPositions.push_back(
                        jt::vector2 { static_cast<float>(i), static_cast<float>(j) });
                }
            }
        }
    }
}

Level::TileType Level::getTileTypeAt(unsigned int x, unsigned int y) const
{
    return m_levelVec.at(posToIndex(x, y));
}

jt::vector2 Level::getPlayerStartPositionInTiles() const
{
    return jt::vector2 { static_cast<float>(m_playerStart.x()),
        static_cast<float>(m_playerStart.y()) };
}
float Level::getPlayerStartAngle() const { return m_playerStartAngle; }
jt::vector2u Level::getLevelSizeInTiles() const { return m_levelSize; }

std::size_t Level::posToIndex(unsigned int x, unsigned int y) const
{
    return x + y * m_levelSize.x();
}

std::vector<jt::vector2> Level::getEnemyPositions() const { return m_enemyPositions; };

jt::vector2 Level::getSymbolPosition() const { return m_symbolPosition; }

std::map<std::size_t, jt::vector2> Level::getKeyPositions() const { return m_keyPositions; }

void Level::PopForceField(std::size_t forceFieldID)
{
    for (auto const& w : m_ForceFields.at(forceFieldID)) {
        if (w->m_forceID != forceFieldID) {
            std::cout << "Warning: level data does not match!";
        }
        auto const x = w->m_tx;
        auto const y = w->m_ty;
        auto const idx = posToIndex(x, y);
        if (m_levelVec.at(idx) != Level::TileType::FORCE) {
            std::cout << "Warning: level data does not match!";
        }
        m_levelVec.at(idx) = Level::TileType::EMPTY;
        w->destroy(); // get rid of collider
    }
    // clean entry in force field list
    m_ForceFields.at(forceFieldID) = {};
}

std::vector<jt::vector2> Level::getAmmoPackPositions() const { return m_ammoPackPositions; }
std::vector<jt::vector2> Level::getHealthPackPositions() const { return m_healthPackPositions; }
