﻿#include "Player.hpp"
#include "GameProperties.hpp"
#include "InputManager.hpp"
#include "MathHelper.hpp"
#include <cmath>

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
        position = position + elapsed * 1.0f * dir;
    }
    if (jt::InputManager::pressed(jt::KeyCode::S)) {
        position = position + elapsed * -1.0f * dir;
    }
    // std::cout << position.x() << " " << position.y() << "\n";
}