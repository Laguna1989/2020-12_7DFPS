#include "Player.hpp"
#include "Conversions.hpp"
#include "GameProperties.hpp"
#include "InputManager.hpp"
#include "MathHelper.hpp"
#include <cmath>

using namespace jt::MathHelper;
using namespace jt::Conversion;

Player::Player(std::shared_ptr<b2World> world, const b2BodyDef* def)
    : Box2DObject { world, def }
{
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.5f, 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.9f;

    getB2Body()->CreateFixture(&fixtureDef);
}

void Player::doUpdate(float elapsed)
{
    if (m_takeInput) {
        handleInput(elapsed);
    }

    auto v = vec(getB2Body()->GetLinearVelocity());
    // velocity dampening
    v.x() *= GP::PlayerVelocityDamping();
    v.y() *= GP::PlayerVelocityDamping();
    // get rid of denormalized floats
    if (abs(v.x()) < 0.001f) {
        v.x() = 0;
    }
    if (abs(v.y()) < 0.001f) {
        v.y() = 0;
    }

    // clamp velocity
    auto l = jt::MathHelper::length(v);
    if (l >= GP::PlayerMaxSpeed()) {
        v = v * (GP::PlayerMaxSpeed() / l);
    }
    getB2Body()->SetLinearVelocity(vec(v));
}

void Player::handleInput(float elapsed)
{
    bool isSprinting { false };
    if (jt::InputManager::pressed(jt::KeyCode::LShift)
        || jt::InputManager::pressed(jt::KeyCode::RShift)) {
        isSprinting = true;
    }

    float turningSpeedFactor = isSprinting ? GP::PlayerTurningSpeedSprintFactor() : 1.0f;
    if (jt::InputManager::pressed(jt::KeyCode::E)
        || jt::InputManager::pressed(jt::KeyCode::Right)) {
        angle -= elapsed * GP::PlayerTurningSpeed() * turningSpeedFactor;
    }
    if (jt::InputManager::pressed(jt::KeyCode::Q) || jt::InputManager::pressed(jt::KeyCode::Left)) {
        angle += elapsed * GP::PlayerTurningSpeed() * turningSpeedFactor;
    }

    float walkingSpeedFactor = isSprinting ? GP::PlayerWalkingSpeedSprintFactor() : 1.0f;
    jt::vector2 const dir
        = { cos(jt::MathHelper::deg2rad(angle)), -sin(jt::MathHelper::deg2rad(angle)) };
    if (jt::InputManager::pressed(jt::KeyCode::W)) {
        getB2Body()->ApplyForceToCenter(
            vec(dir * GP::PlayerAccelerationFactor() * walkingSpeedFactor), true);
    }
    if (jt::InputManager::pressed(jt::KeyCode::S)) {
        getB2Body()->ApplyForceToCenter(
            vec(-1.0f * dir * GP::PlayerAccelerationFactor() * walkingSpeedFactor), true);
    }

    jt::vector2 const perp = jt::MathHelper::rotateBy(dir, 90);

    if (jt::InputManager::pressed(jt::KeyCode::A)) {
        getB2Body()->ApplyForceToCenter(
            vec(perp * GP::PlayerAccelerationFactor() * walkingSpeedFactor), true);
    }
    if (jt::InputManager::pressed(jt::KeyCode::D)) {
        getB2Body()->ApplyForceToCenter(
            vec(-1.0f * perp * GP::PlayerAccelerationFactor() * walkingSpeedFactor), true);
    }

    if (jt::InputManager::justPressed(jt::KeyCode::Space)) {
        m_shootNow = true;
    }
}

void Player::setTakeInput(bool take) { m_takeInput = take; }
bool Player::getTakeInput() { return m_takeInput; }

bool Player::getShootNow()
{
    bool const t = m_shootNow;
    m_shootNow = false;
    return t;
}
