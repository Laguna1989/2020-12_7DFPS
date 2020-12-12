#ifndef KEY_GUARD_HPP
#define KEY_GUARD_HPP

#include "GameObject.hpp"
#include "SmartAnimation.hpp"
#include "SmartSprite.hpp"

class Key : public jt::GameObject {
public:
    Key(jt::vector2 pos, std::size_t keyID);

    std::shared_ptr<jt::SmartAnimation> getAnim();

    jt::vector2 getPosition() const;
    std::size_t getKeyID() const;

private:
    std::shared_ptr<jt::SmartAnimation> m_anim;
    std::shared_ptr<jt::SmartSprite> m_glow;

    jt::vector2 m_position;
    std::size_t m_keyID;

    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    void doCreate() override;
};
#endif
