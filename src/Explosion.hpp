#ifndef EXPLOSION_GUARD_HPP
#define EXPLOSION_GUARD_HPP

#include "GameObject.hpp"
#include "SmartShape.hpp"

class Explosion : public jt::GameObject {
public:
    Explosion(jt::vector2 pos);

    std::shared_ptr<jt::SmartShape> getShape();
    jt::vector2 getPosition() { return m_pos; }

private:
    jt::vector2 m_pos;

    std::shared_ptr<jt::SmartShape> m_shape;

    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doCreate() override;
    void doKill() override;
};
#endif
