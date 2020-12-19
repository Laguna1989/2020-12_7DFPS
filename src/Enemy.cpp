#include "Enemy.hpp"
#include "Conversions.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"
#include "MathHelper.hpp"

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

bool Enemy::canAttack() const { return (m_attackTimer <= 0.0f && m_playerInRange); }
void Enemy::doAttack() { m_attackTimer = GP::EnemyAttackTimer(); }

void Enemy::doUpdate(float const elapsed)
{
    using namespace jt::Conversion;
    m_anim->update(elapsed);

    m_attackTimer -= elapsed;

    auto dist = m_playerPos - getPosition();
    auto const l = jt::MathHelper::lengthSquared(dist);
    if (!m_active) {
        if (l <= GP::EnemyTriggerDistance() * GP::EnemyTriggerDistance()) {
            m_active = true;
        }
    }
    if (m_active) {

        m_playerInRange = (l <= GP::EnemyAttackRadius() * GP::EnemyAttackRadius());
        jt::MathHelper::normalizeMe(dist);
        getB2Body()->ApplyForceToCenter(
            vec(dist * GP::PlayerAccelerationFactor() * GP::EnemyVelocityFactor()), true);
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
        if (l >= GP::PlayerMaxSpeed() * GP::EnemyVelocityFactor()) {
            v = v * (GP::PlayerMaxSpeed() * GP::EnemyVelocityFactor() / l);
        }
        getB2Body()->SetLinearVelocity(vec(v));
    }
}

void Enemy::doDraw() const { m_anim->draw(getGame()->getRenderTarget()); }

void Enemy::TakeDamage(float damage)
{
    m_hitpoints -= damage;
    m_active = true;
    if (m_hitpoints <= 0.0f) {
        kill();
    }
    m_anim->flash(1.25f, { 102U, 51U, 60U, 255U });
    m_attackTimer = 0.5f;
}

void Enemy::setPlayerPosition(jt::vector2 playerPosition) { m_playerPos = playerPosition; }
