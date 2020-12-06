#include "StateGame.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"
#include "Hud.hpp"
#include "InputManager.hpp"
#include "MathHelper.hpp"
#include "SmartShape.hpp"
#include "SmartSprite.hpp"
#include "TweenAlpha.hpp"
#include "color.hpp"
#include <algorithm>

namespace {
float mytan(float a) { return std::tan(jt::MathHelper::deg2rad(a)); }
float mytaninv(float a) { return (1.0f / std::tan(jt::MathHelper::deg2rad(a))); }
float mycos(float a) { return std::cos(jt::MathHelper::deg2rad(a)); }
float mysin(float a) { return std::sin(jt::MathHelper::deg2rad(a)); }
float wrapAngle(float a)
{
    if (a < 0) {
        while (a < 0) {
            a += 360;
        }
    } else if (a >= 360.0f) {
        while (a >= 360) {
            a += 360;
        }
    }
    return a;
}
} // namespace

void StateGame::doCreate()
{
    float w = static_cast<float>(GP::GetWindowSize().x());
    float h = static_cast<float>(GP::GetWindowSize().y());

    using jt::SmartShape;
    using jt::TweenAlpha;

    m_background = std::make_shared<SmartShape>();
    m_background->makeRect({ w, h });
    m_background->setColor(GP::PaletteBackground());
    m_background->update(0.0f);

    doCreateInternal();

    m_overlay = std::make_shared<SmartShape>();
    m_overlay->makeRect(jt::vector2 { w, h });
    m_overlay->setColor(jt::color { 0, 0, 0 });
    m_overlay->update(0);
    auto tw
        = TweenAlpha<SmartShape>::create(m_overlay, 0.5f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tw->setSkipFrames();
    add(tw);

    m_hud = std::make_shared<Hud>();
    add(m_hud);
}

void StateGame::doInternalUpdate(float const elapsed)
{
    m_background->update(elapsed);
    m_floor->update(elapsed);
    m_sky->update(elapsed);
    calculateWallScales();
    for (auto const w : m_walls) {
        w->update(elapsed);
    }
    m_overlay->update(elapsed);
    m_mapPlayer->setPosition(m_player->position * static_cast<float>(GP::MapTileSizeInPixel()));
    m_mapPlayer->update(elapsed);
    m_mapBackground->update(elapsed);
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());
    m_sky->draw(getGame()->getRenderTarget());
    m_floor->draw(getGame()->getRenderTarget());
    drawObjects();
    for (auto const w : m_walls) {
        w->draw(getGame()->getRenderTarget());
    }

    drawMap();
    m_overlay->draw(getGame()->getRenderTarget());
}

void StateGame::doCreateInternal()
{
    m_walls.resize(GP::GetDivisions());
    for (auto i = 0U; i != GP::GetDivisions(); ++i) {
        auto w = std::make_shared<jt::SmartShape>();
        float const binWidth = GP::GetWindowSize().x() / GP::GetZoom() / GP::GetDivisions();
        w->makeRect({ binWidth, GP::GetWindowSize().y() / GP::GetZoom() });
        w->setColor(jt::Random::getRandomColor());
        w->setPosition({ static_cast<float>(i * binWidth), 0.0f });
        m_walls.at(i) = w;
    }
    // reverse for wall segments being rendered from right to left (mathematical positive angle)
    std::reverse(m_walls.begin(), m_walls.end());

    m_player = std::make_shared<Player>();
    add(m_player);

    m_level = std::make_shared<Level>();
    m_level->loadLevel("assets/level1.png");
    m_player->position = m_level->getPlayerStartPositionInTiles();
    m_player->angle = m_level->getPlayerStartAngle();

    m_mapBackground = std::make_shared<jt::SmartShape>();
    m_mapBackground->makeRect(
        { static_cast<float>(m_level->getLevelSizeInTiles().x() * GP::MapTileSizeInPixel()),
            static_cast<float>(m_level->getLevelSizeInTiles().y() * GP::MapTileSizeInPixel()) });
    m_mapBackground->setColor(jt::colors::Cyan);
    m_mapWall = std::make_shared<jt::SmartShape>();
    m_mapWall->makeRect({ static_cast<float>(GP::MapTileSizeInPixel()),
        static_cast<float>(GP::MapTileSizeInPixel()) });
    m_mapWall->setColor(jt::colors::Green);
    m_mapPlayer = std::make_shared<jt::SmartShape>();
    m_mapPlayer->makeRect({ static_cast<float>(GP::MapTileSizeInPixel()),
        static_cast<float>(GP::MapTileSizeInPixel()) });
    m_mapPlayer->setColor(jt::colors::Red);

    m_sky = std::make_shared<jt::SmartShape>();
    m_sky->makeRect(jt::vector2 {
        GP::GetWindowSize().x() / GP::GetZoom(), GP::GetWindowSize().y() / 2.0f / GP::GetZoom() });

    m_sky->setColor(GP::PalletteSky());

    m_floor = std::make_shared<jt::SmartShape>();
    m_floor->makeRect(jt::vector2 {
        GP::GetWindowSize().x() / GP::GetZoom(), GP::GetWindowSize().y() / 2.0f / GP::GetZoom() });
    m_floor->setColor(GP::PalletteFloor());
    m_floor->setPosition({ 0, GP::GetWindowSize().y() * (0.5f / GP::GetZoom()) });
}

jt::vector2 getH1(float const theta, float px, float py, float dx, float dy)
{
    int const x = static_cast<int>(px);
    int const y = static_cast<int>(py);
    if (theta >= 0 && theta < 90) {
        return jt::vector2 { (theta == 0) ? x : px + dy * mytaninv(theta), static_cast<float>(y) };
    } else if (theta >= 90 && theta < 180) {
        return jt::vector2 { (theta == 0) ? x : px - (dy)*mytaninv(180 - theta),
            static_cast<float>(y) };
    } else if (theta >= 180 && theta < 270) {
        return jt::vector2 { (theta == 0) ? x : px - (1 - dy) * mytan(270 - theta), y + 1.0f };
    } else {
        return jt::vector2 { (theta == 0) ? x : px + (1 - dy) * mytan(theta - 270), y + 1.0f };
    }
}

jt::vector2 getHInc(float const theta)
{
    if (theta >= 0 && theta < 90) {
        return jt::vector2 { (theta == 0) ? 0 : mytaninv(theta), -1.0f };
    } else if (theta >= 90 && theta < 180) {
        return jt::vector2 { (theta == 0) ? 0 : mytaninv(theta), -1.0f };
    } else if (theta >= 180 && theta < 270) {
        return jt::vector2 { (theta == 0) ? 0 : -mytan(270 - theta), 1.0f };
    } else {
        return jt::vector2 { (theta == 0) ? 0 : mytan(theta - 270), 1.0f };
    }
}

jt::vector2 getV1(float const theta, float px, float py, float dx, float dy)
{
    int const x = static_cast<int>(px);
    int const y = static_cast<int>(py);
    if (theta >= 0 && theta < 90) {
        return jt::vector2 { x + 1.0f, py - (1 - dx) * mytan(theta) };
    } else if (theta >= 90 && theta < 180) {
        return jt::vector2 { static_cast<float>(x), py - (dx)*mytan(180 - theta) };
    } else if (theta >= 180 && theta < 270) {
        return jt::vector2 { static_cast<float>(x), py + (dx)*mytan(theta - 180) };
    } else {
        return jt::vector2 { x + 1.0f, py + (1 - dx) * mytan(360 - theta) };
    }
}

jt::vector2 getVInc(float const theta)
{
    if (theta >= 0 && theta < 90) {
        return jt::vector2 { 1, -mytan(theta) };
    } else if (theta >= 90 && theta < 180) {
        return jt::vector2 { -1.0f, -mytan(180 - theta) };
    } else if (theta >= 180 && theta < 270) {
        return jt::vector2 { -1.0f, mytan(theta - 180) };
    } else {
        return jt::vector2 { 1.0f, mytan(360 - theta) };
    }
}

int gethttcx(float const theta, float hnx) { return static_cast<int>(hnx); }
int gethttcy(float const theta, float hny)
{
    if (theta >= 0 && theta < 180) {
        return static_cast<int>(hny) - 1;
    } else {
        return static_cast<int>(hny);
    }
}

int getvttcx(float const theta, float vnx)
{
    if (theta >= 90 && theta < 270) {
        return static_cast<int>(vnx) - 1;
    } else {
        return static_cast<int>(vnx);
    }
}
int getvttcy(float const theta, float vny) { return static_cast<int>(vny); }

void scaleWall(std::shared_ptr<jt::SmartShape> w, jt::vector2 playerPos, float theta, float angle,
    jt::vector2 hIntersectionPos, jt::vector2 vIntersectionPos)
{
    auto const dh = hIntersectionPos - playerPos;
    auto const dv = vIntersectionPos - playerPos;
    auto const lhs = jt::MathHelper::lengthSquared(dh);
    auto const lvs = jt::MathHelper::lengthSquared(dv);
    float d;
    if (theta >= 0 && theta < 90) {
        if (lhs > lvs) {
            d = 0.85f / (abs(dv.x()) * mycos(angle) + abs(dv.y()) * mysin(angle));
            w->setColor(GP::PaletteWallE());

        } else {
            d = 0.85f / (abs(dh.x()) * mycos(angle) + abs(dh.y()) * mysin(angle));
            w->setColor(GP::PaletteWallN());
        }
    } else if (theta >= 90 && theta < 180) {
        if (lhs > lvs) {
            d = 0.85f / (abs(dv.x() * mycos(angle)) + abs(dv.y() * mysin(angle)));
            w->setColor(GP::PaletteWallW());

        } else {
            d = 0.85f / (abs(dh.x() * mycos(angle)) + abs(dh.y() * mysin(angle)));
            w->setColor(GP::PaletteWallN());
        }
    } else if (theta > 180 && theta < 270) {
        if (lhs > lvs) {
            d = 0.85f / (abs(dv.x() * mycos(angle)) + abs(dv.y() * mysin(angle)));
            w->setColor(GP::PaletteWallW());

        } else {
            d = 0.85f / (abs(dh.x() * mycos(angle)) + abs(dh.y() * mysin(angle)));
            w->setColor(GP::PaletteWallS());
        }
    } else {
        if (lhs > lvs) {
            d = 0.85f / (abs(dv.x() * mycos(angle)) + abs(dv.y() * mysin(angle)));
            w->setColor(GP::PaletteWallE());

        } else {
            d = 0.85f / (abs(dh.x() * mycos(angle)) + abs(dh.y() * mysin(angle)));
            w->setColor(GP::PaletteWallS());
        }
    }
    auto p = w->getPosition();
    p.y() = (1.0f - d) * 0.5f * (GP::GetWindowSize().y() / GP::GetZoom());
    w->setPosition(p);
    w->setScale({ 1.0f, d });
}

void StateGame::calculateWallScales()
{
    // player absolut position
    float const px = m_player->position.x();
    float const py = m_player->position.y();

    // player position in full tiles
    int x = static_cast<int>(px);
    int y = static_cast<int>(py);

    // player offset in tile
    float const dx = px - x;
    float const dy = py - y;

    float const angleIncrement = GP::GetFoVAngle() / GP::GetDivisions();
    float const angleStart = m_player->angle - GP::GetFoVAngle() * 0.5f;

    for (auto i = 0U; i != GP::GetDivisions(); ++i) {
        // ray direction
        float const theta = wrapAngle(angleStart + i * angleIncrement);
        auto w = m_walls.at(i);

        jt::vector2 vIntersectionPos {};
        jt::vector2 hIntersectionPos {};

        {
            // horizontal grid intersections
            jt::vector2 hn = getH1(theta, px, py, dx, dy);
            jt::vector2 const hInc = getHInc(theta);
            for (int i = 0; i != 50; ++i) {
                int const ttcx = gethttcx(theta, hn.x());
                int const ttcy = gethttcy(theta, hn.y());
                if (ttcx < 0 || ttcy < 0 || ttcx >= m_level->getLevelSizeInTiles().x()
                    || ttcy >= m_level->getLevelSizeInTiles().y()) {
                    break;
                }
                if (m_level->getTileTypeAt(ttcx, ttcy) == Level::TileType::WALL) {
                    hIntersectionPos = hn;
                    break;
                }
                // increase horizontal intersection
                hn = hn + hInc;
            }
        }
        {
            // vertical grid intersections
            jt::vector2 vn = getV1(theta, px, py, dx, dy);
            jt::vector2 const vInc = getVInc(theta);
            for (int i = 0; i != 50; ++i) {
                int const ttcx = getvttcx(theta, vn.x());
                int const ttcy = getvttcy(theta, vn.y());
                if (ttcx < 0 || ttcy < 0 || ttcx >= m_level->getLevelSizeInTiles().x()
                    || ttcy >= m_level->getLevelSizeInTiles().y()) {
                    break;
                }
                if (m_level->getTileTypeAt(ttcx, ttcy) == Level::TileType::WALL) {
                    vIntersectionPos = vn;
                    break;
                }
                // increase vertical intersection
                vn = vn + vInc;
            }
        }

        scaleWall(
            w, m_player->position, theta, m_player->angle, hIntersectionPos, vIntersectionPos);
    }
}

void StateGame::drawMap() const
{
    m_mapBackground->draw(getGame()->getRenderTarget());

    m_mapPlayer->draw(getGame()->getRenderTarget());

    for (size_t i = 0U; i != m_level->getLevelSizeInTiles().x(); ++i) {
        for (size_t j = 0U; j != m_level->getLevelSizeInTiles().y(); ++j) {
            if (m_level->getTileTypeAt(i, j) == Level::TileType::WALL) {
                m_mapWall->setPosition(
                    { 1.0f * i * GP::MapTileSizeInPixel(), 1.0f * j * GP::MapTileSizeInPixel() });
                m_mapWall->update(0.0f);
                m_mapWall->draw(getGame()->getRenderTarget());
            }
        }
    }
}
