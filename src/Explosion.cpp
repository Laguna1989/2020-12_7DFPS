#include "Explosion.hpp"
#include "GameBase.hpp"

Explosion::Explosion(jt::vector2 pos) { m_pos = pos; }
void Explosion::doCreate()
{
    m_anim = std::make_shared<jt::SmartAnimation>();
    m_anim->add("assets/explision.png", "idle", { 21, 24 }, { 0, 1, 2, 3, 4, 5 }, 0.075f);
    m_anim->play("idle", 0, true);
}
void Explosion::doUpdate(float const elapsed)
{
    m_anim->update(elapsed);
    if (getAge() >= m_anim->getCurrentAnimTotalTime()) {
        kill();
    }
}
void Explosion::doDraw() const { m_anim->draw(getGame()->getRenderTarget()); }
void Explosion::doKill() { }

std::shared_ptr<jt::SmartAnimation> Explosion::getShape() { return m_anim; }
