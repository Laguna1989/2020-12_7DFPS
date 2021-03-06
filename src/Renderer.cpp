﻿#include "Renderer.hpp"
#include "FastMath.hpp"
#include "GameProperties.hpp"
#include "MathHelper.hpp"

namespace {
jt::vector2 getH1(float const theta, float px, float py, float /*dx*/, float dy)
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

jt::vector2 getV1(float const theta, float px, float py, float dx, float /*dy*/)
{
    int const x = static_cast<int>(px);
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

int gethttcx(float const /*theta*/, float hnx) { return static_cast<int>(hnx); }
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
int getvttcy(float const /*theta*/, float vny) { return static_cast<int>(vny); }

float getDistanceTransform(float const theta, jt::vector2 const cp)
{
    float z = cp.x() * mycos(theta) - cp.y() * mysin(theta);
    return 0.85f / z;
}

void scaleSprite(std::shared_ptr<jt::SmartDrawable> w, jt::vector2 playerPos, float theta,
    float angle, jt::vector2 hIntersectionPos, jt::vector2 vIntersectionPos, bool doColor)
{
    auto const dh = hIntersectionPos - playerPos;
    auto const dv = vIntersectionPos - playerPos;
    auto const lhs = jt::MathHelper::lengthSquared(dh);
    auto const lvs = jt::MathHelper::lengthSquared(dv);
    float d {};
    float dreal {};

    if (lhs > lvs) {
        d = getDistanceTransform(angle, dv);
        dreal = jt::MathHelper::length(dv);
        if (doColor) {
            if (theta >= 0 && theta < 90) {
                w->setColor(GP::PaletteWallE());
            } else if (theta >= 90 && theta < 180) {
                w->setColor(GP::PaletteWallW());
            } else if (theta > 180 && theta < 270) {
                w->setColor(GP::PaletteWallW());
            } else {
                w->setColor(GP::PaletteWallE());
            }
        }
    } else {
        d = getDistanceTransform(angle, dh);
        dreal = jt::MathHelper::length(dh);
        if (doColor) {
            if (theta >= 0 && theta < 90) {
                w->setColor(GP::PaletteWallN());
            } else if (theta >= 90 && theta < 180) {
                w->setColor(GP::PaletteWallN());
            } else if (theta > 180 && theta < 270) {
                w->setColor(GP::PaletteWallS());
            } else {
                w->setColor(GP::PaletteWallS());
            }
        }
    }

    auto p = w->getPosition();
    p.y() = (1.0f - d) * 0.5f * (GP::GetWindowSize().y() / GP::GetZoom());
    w->setPosition(p);
    w->setScale({ 1.0f, d });
    w->setZDist(dreal);
} // namespace

} // namespace

void calculateWallScales(jt::vector2 const playerPos, float const angle,
    std::shared_ptr<Level> const level, std::vector<std::shared_ptr<Wall>>& walls,
    bool isForceField)
{
    // reset all walls to zero size
    for (auto const& w : walls) {
        w->getShape()->setScale({ 1.0f, 0.0f });
    }

    // player absolut position
    float const px = playerPos.x() + 0.45f;
    float const py = playerPos.y() + 0.45f;

    // player position in full tiles
    int x = static_cast<int>(px);
    int y = static_cast<int>(py);

    // player offset in tile
    float const dx = px - x;
    float const dy = py - y;

    float const angleIncrement = GP::RendererFoVAngle() / GP::GetDivisions();
    float const angleStart = angle - GP::RendererFoVAngle() * 0.5f;

    Level::TileType const expectedTileType
        = (isForceField ? Level::TileType::FORCE : Level::TileType::WALL);

    for (auto i = 0U; i != GP::GetDivisions(); ++i) {
        // ray direction
        float const theta = wrapAngle(angleStart + i * angleIncrement);

        jt::vector2 vIntersectionPos { -500000.0f, -500000.0f };
        jt::vector2 hIntersectionPos { -500000.0f, -500000.0f };

        {
            // horizontal grid intersections
            jt::vector2 hn = getH1(theta, px, py, dx, dy);
            jt::vector2 const hInc = getHInc(theta);
            for (int i = 0; i != GP::RendererMaxDistance(); ++i) {
                int const ttcx = gethttcx(theta, hn.x());
                int const ttcy = gethttcy(theta, hn.y());
                if (ttcx < 0 || ttcy < 0
                    || ttcx >= static_cast<int>(level->getLevelSizeInTiles().x())
                    || ttcy >= static_cast<int>(level->getLevelSizeInTiles().y())) {
                    break;
                }
                if (level->getTileTypeAt(ttcx, ttcy) == expectedTileType) {
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
            for (int i = 0; i != GP::RendererMaxDistance(); ++i) {
                int const ttcx = getvttcx(theta, vn.x());
                int const ttcy = getvttcy(theta, vn.y());
                if (ttcx < 0 || ttcy < 0
                    || ttcx >= static_cast<int>(level->getLevelSizeInTiles().x())
                    || ttcy >= static_cast<int>(level->getLevelSizeInTiles().y())) {
                    break;
                }
                if (level->getTileTypeAt(ttcx, ttcy) == expectedTileType) {
                    vIntersectionPos = vn;
                    break;
                }
                // increase vertical intersection
                vn = vn + vInc;
            }
        }
        auto w = walls.at(i);
        scaleSprite(
            w->getShape(), playerPos, theta, angle, hIntersectionPos, vIntersectionPos, true);
    }
}

void calculateSpriteScale(jt::vector2 const playerPos, float const angle,
    jt::vector2 const spriteWorldPos, std::shared_ptr<jt::SmartDrawable> d, float yOffset)
{
    const jt::vector2 dist = spriteWorldPos - playerPos;

    float const spriteAngle = wrapAngle(jt::MathHelper::rad2deg(std::atan2(-dist.y(), dist.x())));

    float a = wrapAngle(angle);

    if (spriteAngle < a - GP::RendererFoVAngle() / 2.0f - GP::RendererSideToleranceAngle()
        || spriteAngle > a + GP::RendererFoVAngle() / 2.0f + GP::RendererSideToleranceAngle()) {
        d->setScale({ 0.0f, 0.0f });
        return;
    }

    float const spriteAngleOnScreen
        = wrapAngle(spriteAngle - (angle - GP::RendererFoVAngle() / 2.0f));
    float const xp = (1.0f - spriteAngleOnScreen / GP::RendererFoVAngle()) * GP::GetWindowSize().x()
        / GP::GetZoom();

    scaleSprite(d, playerPos, spriteAngle, angle, spriteWorldPos,
        jt::vector2 { -90000.0f, -90000.0f }, false);

    auto const s = d->getScale();
    float newScale = s.y() * GP::RendererSpriteScale();
    d->setScale({ newScale, newScale });
    // std::cout << d->getScale().y() << std::endl;
    auto const p = d->getPosition();
    auto const py = p.y() + 5.0f - (d->getLocalBounds().height()) * (0.5f - 0.5f * newScale);
    d->setZDist(d->getZDist());
    d->setPosition({ xp, py + yOffset });
}
