#include "Explosion.hpp"
#include "GameBase.hpp"

Explosion::Explosion(jt::vector2 pos) { m_pos = pos; }
void Explosion::doCreate()
{
    m_shape = std::make_shared<jt::SmartShape>();
    m_shape->makeRect({ 21, 24 });
    m_shape->setColor(jt::colors::Blue);
}
void Explosion::doUpdate(float const elapsed) { m_shape->update(elapsed); }
void Explosion::doDraw() const { m_shape->draw(getGame()->getRenderTarget()); }
void Explosion::doKill() { }

std::shared_ptr<jt::SmartShape> Explosion::getShape() { return m_shape; }
