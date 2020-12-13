#include "Enemy.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"

Enemy::Enemy(std::shared_ptr<b2World> world, const b2BodyDef* def)
    : Box2DObject { world, def }
{
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.5f, 0.5f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.9f;

    getB2Body()->CreateFixture(&fixtureDef);
    m_hitpoints = GP::EnemyHitPointsDefault();
}
std::shared_ptr<jt::SmartAnimation> Enemy::getAnimation() { return m_anim; }

void Enemy::doCreate()
{
    m_anim = std::make_shared<jt::SmartAnimation>();
    m_anim->add("assets/enemy.png", "idle", jt::vector2u { 21, 24 }, { 0, 1, 2, 3, 4, 5 }, 0.2f);
    m_anim->play("idle");
    m_anim->setScale({ 2.0f, 2.0f });
}

void Enemy::doUpdate(float const elapsed) { m_anim->update(elapsed); }
void Enemy::doDraw() const { m_anim->draw(getGame()->getRenderTarget()); }

void Enemy::TakeDamage(float damage)
{
    m_hitpoints -= damage;
    m_active = true;
    if (m_hitpoints <= 0.0f) {
        kill();
    }
}

void Enemy::setPlayerPosition(jt::vector2 playerPosition) { m_playerPos = playerPosition; }
