#include "StateGame.hpp"
#include "FastMath.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"
#include "Hud.hpp"
#include "InputManager.hpp"
#include "MathHelper.hpp"
#include "Renderer.hpp"
#include "SmartShape.hpp"
#include "SmartSprite.hpp"
#include "TweenAlpha.hpp"
#include "color.hpp"
#include <algorithm>
#include <map>

namespace {

static std::string getLevelFileNameFromId(int id)
{
    return "assets/level" + std::to_string(id) + ".png";
}
static std::string getTextFileNameFromId(int id)
{
    return "assets/level" + std::to_string(id) + ".txt";
}
} // namespace

StateGame::StateGame(int levelId)
{
    m_levelID = levelId;
    m_levelFileName = getLevelFileNameFromId(m_levelID);
    m_world = std::make_shared<b2World>(b2Vec2 { 0, 0 });
}

void StateGame::doCreate()
{
    // std::cout << "StateGame::do Create\n";
    float w = static_cast<float>(GP::GetWindowSize().x());
    float h = static_cast<float>(GP::GetWindowSize().y());

    using jt::SmartShape;
    using jt::TweenAlpha;

    m_background = std::make_shared<SmartShape>();
    m_background->makeRect({ w, h });
    m_background->setColor(GP::PaletteBackground());
    m_background->update(0.0f);

    doCreateInternal();

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    m_introText = std::make_shared<IntroText>(getTextFileNameFromId(m_levelID));
    add(m_introText);
    m_introText->setTextColor(jt::color { 176U, 137U, 161U, 255U });

    m_overlay = std::make_shared<SmartShape>();
    m_overlay->makeRect(jt::vector2 { w, h });
    m_overlay->setColor(jt::color { 0, 0, 0 });
    m_overlay->update(0);
    m_tween1 = TweenAlpha<SmartShape>::create(
        m_overlay, m_introText->getTotalTime(), std::uint8_t { 255 }, std::uint8_t { 100 });
    m_tween1->setSkipFrames();
    add(m_tween1);

    m_tween2
        = TweenAlpha<SmartShape>::create(m_overlay, 0.5f, std::uint8_t { 100 }, std::uint8_t { 0 });
    m_tween2->setSkipFrames();
    m_tween2->setStartDelay(m_introText->getTotalTime());
    m_tween2->addCompleteCallback([this]() { m_player->setTakeInput(true); });
    add(m_tween2);

    m_world->SetContactListener(&m_contactListener);
}

void StateGame::doCreateInternal()
{
    // std::cout << "StateGame::do CreateInternal 1\n";
    m_walls.resize(GP::GetDivisions());
    for (auto i = 0U; i != GP::GetDivisions(); ++i) {
        auto w = std::make_shared<Wall>();
        float const binWidth = GP::GetWindowSize().x() / GP::GetZoom() / GP::GetDivisions();
        w->getShape()->makeRect({ binWidth, GP::GetWindowSize().y() / GP::GetZoom() });
        w->getShape()->setColor(jt::Random::getRandomColor());
        w->getShape()->setPosition({ static_cast<float>(i * binWidth), 0.0f });
        m_walls.at(i) = w;
        add(w);
    }
    // reverse for wall segments being rendered from right to left (mathematical positive angle)
    std::reverse(m_walls.begin(), m_walls.end());

    // std::cout << "StateGame::do CreateInternal 2\n";
    m_level = std::make_shared<Level>();
    m_level->loadLevel(m_levelFileName, m_world);

    // std::cout << "StateGame::do CreateInternal 2.5\n";
    b2BodyDef playerBodyDef;
    playerBodyDef.type = b2_dynamicBody;
    playerBodyDef.fixedRotation = true;
    playerBodyDef.linearDamping = 2.0f;
    playerBodyDef.position.Set(
        m_level->getPlayerStartPositionInTiles().x(), m_level->getPlayerStartPositionInTiles().y());

    m_player = std::make_shared<Player>(m_world, &playerBodyDef);
    m_player->angle = m_level->getPlayerStartAngle();
    m_player->setTakeInput(false);
    add(m_player);

    // std::cout << "StateGame::do CreateInternal 3\n";
    m_mapBackground = std::make_shared<jt::SmartShape>();

    m_mapBackground->makeRect(
        { static_cast<float>(m_level->getLevelSizeInTiles().x() * GP::MapTileSizeInPixel()),
            static_cast<float>(m_level->getLevelSizeInTiles().y() * GP::MapTileSizeInPixel()) });
    m_mapBackground->setColor(jt::colors::Cyan);
    // std::cout << "StateGame::do CreateInternal 3.5\n";
    m_mapWall = std::make_shared<jt::SmartShape>();
    m_mapWall->makeRect({ static_cast<float>(GP::MapTileSizeInPixel()),
        static_cast<float>(GP::MapTileSizeInPixel()) });
    m_mapWall->setColor(jt::colors::Green);
    // std::cout << "StateGame::do CreateInternal 3.75\n";
    m_mapPlayer = std::make_shared<jt::SmartShape>();
    m_mapPlayer->makeRect({ static_cast<float>(GP::MapTileSizeInPixel()),
        static_cast<float>(GP::MapTileSizeInPixel()) });
    m_mapPlayer->setColor(jt::colors::Red);

    // std::cout << "StateGame::do CreateInternal 4\n";
    m_sky = std::make_shared<jt::SmartShape>();
    m_sky->makeRect(jt::vector2 {
        GP::GetWindowSize().x() / GP::GetZoom(), GP::GetWindowSize().y() / 2.0f / GP::GetZoom() });

    m_sky->setColor(GP::PalletteSky());

    m_floor = std::make_shared<jt::SmartShape>();
    m_floor->makeRect(jt::vector2 {
        GP::GetWindowSize().x() / GP::GetZoom(), GP::GetWindowSize().y() / 2.0f / GP::GetZoom() });
    m_floor->setColor(GP::PalletteFloor());
    m_floor->setPosition({ 0, GP::GetWindowSize().y() * (0.5f / GP::GetZoom()) });

    for (auto const& enemyPosition : m_level->getEnemyPositions()) {
        playerBodyDef.position = jt::Conversion::vec(enemyPosition);
        auto e = std::make_shared<Enemy>(m_world, &playerBodyDef);
        add(e);
        /*   e->getAnimation()->setPosition({ 100, 150 });*/
        m_enemies.push_back(e);
    }

    // std::cout << "StateGame::do CreateInternal 5\n";

    m_hud = std::make_shared<Hud>();

    b2BodyDef symbolBodyDef;
    symbolBodyDef.type = b2_staticBody;
    symbolBodyDef.fixedRotation = true;
    symbolBodyDef.position = jt::Conversion::vec(m_level->getSymbolPosition());
    m_symbol = std::make_shared<Symbol>(m_world, &symbolBodyDef);
    add(m_symbol);

    m_shots = std::make_shared<jt::ObjectGroup<Shot>>();
    add(m_shots);
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (jt::InputManager::justPressed(jt::KeyCode::Escape)) {
        m_introText->update(5000);
        m_tween1->cancel();
        m_tween2->cancel();
        m_introText->cancel();
        m_player->setTakeInput(true);
        m_overlay->setColor(jt::colors::Transparent);
    }

    if (!m_ending) {
        // std::cout << "StateGame::do InternalUpdate\n";
        m_background->update(elapsed);
        m_floor->update(elapsed);
        m_sky->update(elapsed);

        int32 velocityIterations = 6;
        int32 positionIterations = 2;
        m_world->Step(elapsed, velocityIterations, positionIterations);

        calculateWallScales();
        for (auto const& w : m_walls) {
            w->update(elapsed);
        }

        for (auto const& e : m_enemies) {
            ::calculateSpriteScale(
                m_player->getPosition(), m_player->angle, e->getPosition(), e->getAnimation());
            e->update(elapsed);
        }

        ::calculateSpriteScale(m_player->getPosition(), m_player->angle, m_symbol->getPosition(),
            m_symbol->getSprite());
        m_symbol->update(elapsed);

        for (auto& s : *m_shots) {
            if (s.expired()) {
                continue;
            }
            auto const sp = s.lock();
            ::calculateSpriteScale(
                m_player->getPosition(), m_player->angle, sp->getPosition(), sp->getAnim(), 30.0f);
        }

        m_overlay->update(elapsed);
        m_mapPlayer->setPosition(
            m_player->getPosition() * static_cast<float>(GP::MapTileSizeInPixel()));
        m_mapPlayer->update(elapsed);
        m_mapBackground->update(elapsed);

        jt::vector2 const vectorPlayerSymbol = m_symbol->getPosition() - m_player->getPosition();
        float distancePlayerSymbolSquared = jt::MathHelper::lengthSquared(vectorPlayerSymbol);

        if (distancePlayerSymbolSquared
            < GP::SymbolTriggerDistance() * GP::SymbolTriggerDistance()) {
            m_ending = true;
            getGame()->switchState(std::make_shared<StateGame>(m_levelID + 1));
        }

        if (m_player->getShootNow()) {
            SpawnShot();
        }
    }
}

void StateGame::doInternalDraw() const
{
    // std::cout << "StateGame::do Internal Draw\n";
    m_background->draw(getGame()->getRenderTarget());
    m_sky->draw(getGame()->getRenderTarget());
    m_floor->draw(getGame()->getRenderTarget());
    // drawObjects();

    std::map<float, std::vector<std::shared_ptr<jt::GameObject>>> zMap;

    for (auto const& w : m_walls) {
        zMap[-w->getShape()->getZDist()].push_back(w);
    }
    for (auto const& e : m_enemies) {
        zMap[-e->getAnimation()->getZDist()].push_back(e);
    }
    zMap[-m_symbol->getSprite()->getZDist()].push_back(m_symbol);
    for (auto& s : *m_shots) {
        if (s.expired()) {
            continue;
        }
        auto sp = s.lock();
        zMap[-sp->getAnim()->getZDist()].push_back(sp);
    }

    for (auto const& kvp : zMap) {
        for (auto p : kvp.second) {
            p->draw();
        }
    }

    drawMap();
    m_overlay->draw(getGame()->getRenderTarget());
    m_introText->draw();
}

void StateGame::calculateWallScales()
{
    // std::cout << "StateGame::calculateWallScales\n";
    ::calculateWallScales(m_player->getPosition(), m_player->angle, m_level, m_walls);
}

void StateGame::drawMap() const
{
    // std::cout << "StateGame::drawMap\n";
    m_mapBackground->draw(getGame()->getRenderTarget());

    m_mapPlayer->draw(getGame()->getRenderTarget());

    for (size_t i = 0U; i != m_level->getLevelSizeInTiles().x(); ++i) {
        for (size_t j = 0U; j != m_level->getLevelSizeInTiles().y(); ++j) {
            if (m_level->getTileTypeAt(static_cast<unsigned int>(i), static_cast<unsigned int>(j))
                == Level::TileType::WALL) {
                m_mapWall->setPosition(
                    { 1.0f * i * GP::MapTileSizeInPixel(), 1.0f * j * GP::MapTileSizeInPixel() });
                m_mapWall->update(0.0f);
                m_mapWall->draw(getGame()->getRenderTarget());
            }
        }
    }
}

void StateGame::SpawnShot()
{
    b2BodyDef shotBodyDef;
    shotBodyDef.type = b2_dynamicBody;
    shotBodyDef.fixedRotation = true;
    shotBodyDef.linearDamping = 0.0f;
    float const a = m_player->angle;
    jt::vector2 const dir { mycos(a), -mysin(a) };
    // std::cout << dir.x() << " " << dir.y() << std::endl;
    shotBodyDef.position.Set(
        m_player->getPosition().x() + dir.x() + 0.0f, m_player->getPosition().y() + dir.y() + 0.0f);
    auto s = std::make_shared<Shot>(m_world, &shotBodyDef);
    s->setVelocity(dir * GP::ShotSpeed());
    add(s);
    s->update(1.0f / 60.0f);
    m_shots->push_back(s);
}
