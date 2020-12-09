#include "Level.hpp"
#include "GameProperties.hpp"
#include "Player.hpp"

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
            auto const c = level->getColorAtPixel({ i, j });
            if (c == jt::colors::White) {
                m_levelVec.at(posToIndex(i, j)) = Level::TileType::WALL;
                wallBodyDef.position = b2Vec2 { static_cast<float>(i), static_cast<float>(j) };
                LevelWall const w { world, &wallBodyDef };
            } else {
                if (c.r() == 0 && c.g() == 255) {
                    // player start
                    m_playerStart = jt::vector2u { i, j };
                    m_playerStartAngle = 90.0f * c.b();
                } else if (c.r() == 255 && c.g() == 0 && c.b() == 0) {
                    // enemy position
                    m_enemyPositions.push_back(
                        jt::vector2 { static_cast<float>(i), static_cast<float>(j) });
                } else if (c.r() == 0 && c.g() == 0 && c.b() == 255) {
                    m_symbolPosition = jt::vector2 { static_cast<float>(i), static_cast<float>(j) };
                }
                m_levelVec.at(posToIndex(i, j)) = Level::TileType::EMPTY;
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
