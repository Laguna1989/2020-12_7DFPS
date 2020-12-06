#include "Level.hpp"
#include "Player.hpp"

void Level::loadLevel(std::string const& fileName)
{
    auto level = std::make_shared<jt::SmartSprite>();
    level->loadSprite(fileName);
    m_levelVec.clear();
    m_levelSize = jt::vector2u { static_cast<unsigned int>(level->getLocalBounds().width()),
        static_cast<unsigned int>(level->getLocalBounds().height()) };
    m_levelVec.resize(m_levelSize.x() * m_levelSize.y());
    for (unsigned int j = 0; j != m_levelSize.y(); ++j) {
        for (unsigned int i = 0; i != m_levelSize.x(); ++i) {
            auto const c = level->getColorAtPixel({ i, j });
            if (c == jt::colors::White) {
                m_levelVec.at(posToIndex(i, j)) = Level::TileType::WALL;
            } else {
                if (c.r() == 0 && c.g() == 255) {
                    m_playerStart = jt::vector2u { i, j };
                    m_playerStartAngle = 90.0f * c.b();
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
