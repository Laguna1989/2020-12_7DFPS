#include "Shot.hpp"
#include "Game.hpp"
#include "StateGame.hpp"

Shot::Shot(std::shared_ptr<b2World> world, b2BodyDef* def)
    : Box2DObject { world, def }
{
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.15f, 0.15f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 0.2f;
    fixtureDef.isSensor = true;

    getB2Body()->CreateFixture(&fixtureDef);
    getB2Body()->SetUserData(this); // for callbacks
}

void Shot::doCreate()
{
    m_anim = std::make_shared<jt::SmartAnimation>();
    m_anim->add("assets/shot.png", "idle", { 8, 8 }, { 0, 1, 2, 3 }, 0.25f);
    m_anim->play("idle");
}

std::shared_ptr<jt::SmartAnimation> Shot::getAnim() { return m_anim; }

void Shot::doUpdate(float const elapsed)
{
    m_anim->update(elapsed);
    // std::cout << getPosition().x() << " " << getPosition().y() << std::endl;
}
void Shot::doDraw() const { m_anim->draw(getGame()->getRenderTarget()); }

void Shot::collide()
{
    if (!m_hasCollided) {
        m_hasCollided = true;
        if (!m_state.expired()) {
            auto const s = m_state.lock();
            s->SpawnExplosion(getPosition());
        }
        kill();
    }
}

void Shot::setState(std::weak_ptr<StateGame> state) { m_state = state; }
