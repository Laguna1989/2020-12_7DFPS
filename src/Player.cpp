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
    dynamicBox.SetAsBox(static_cast<float>(GP::MapTileSizeInPixel() / 2),
        static_cast<float>(GP::MapTileSizeInPixel() / 2));

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.9f;

    getB2Body()->CreateFixture(&fixtureDef);
}

void Player::doUpdate(float elapsed)
{
    if (jt::InputManager::pressed(jt::KeyCode::D)) {
        angle -= elapsed * GP::PlayerTurningSpeed();
    }
    if (jt::InputManager::pressed(jt::KeyCode::A)) {
        angle += elapsed * GP::PlayerTurningSpeed();
    }
    jt::vector2 const dir
        = { cos(jt::MathHelper::deg2rad(angle)), -sin(jt::MathHelper::deg2rad(angle)) };
    if (jt::InputManager::pressed(jt::KeyCode::W)) {
        getB2Body()->ApplyForceToCenter(vec(dir * GP::PlayerAccelerationFactor()), true);
    }
    if (jt::InputManager::pressed(jt::KeyCode::S)) {
        getB2Body()->ApplyForceToCenter(vec(-1.0f * dir * GP::PlayerAccelerationFactor()), true);
    }

    auto v = vec(getB2Body()->GetLinearVelocity());
    v.x() *= GP::PlayerVelocityDamping();
    v.y() *= GP::PlayerVelocityDamping();
    if (abs(v.x()) < 0.001f) {
        v.x() = 0;
    }
    if (abs(v.y()) < 0.001f) {
        v.y() = 0;
    }
    auto l = jt::MathHelper::length(v);
    std::cout << l << std::endl;
    if (l >= GP::PlayerMaxSpeed()) {
        v = v * (GP::PlayerMaxSpeed() / l);
    }
    getB2Body()->SetLinearVelocity(vec(v));
}
