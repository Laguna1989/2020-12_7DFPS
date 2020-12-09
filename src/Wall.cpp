#include "Wall.hpp"
#include "Game.hpp"

Wall::Wall() { m_shape = std::make_shared<jt::SmartShape>(); }

std::shared_ptr<jt::SmartShape> Wall::getShape() { return m_shape; }
void Wall::doCreate() { }

void Wall::doUpdate(float const elapsed) { m_shape->update(elapsed); }
void Wall::doDraw() const { m_shape->draw(getGame()->getRenderTarget()); }
