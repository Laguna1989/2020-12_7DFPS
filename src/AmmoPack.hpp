#ifndef AMMOPACK_GUARD_HPP
#define AMMOPACK_GUARD_HPP

#include "GameObject.hpp"
#include "SmartAnimation.hpp"
#include "SmartSprite.hpp"

class AmmoPack : public jt::GameObject {
public:
    AmmoPack(jt::vector2 pos);

    std::shared_ptr<jt::SmartAnimation> getAnim();

    jt::vector2 getPosition() const;

private:
    std::shared_ptr<jt::SmartAnimation> m_anim;
    std::shared_ptr<jt::SmartSprite> m_glow;

    jt::vector2 m_position;

    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    void doCreate() override;
};
#endif
