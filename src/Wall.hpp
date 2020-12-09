#ifndef GAME_WALL_HPP_INCLUDEGUARD
#define GAME_WALL_HPP_INCLUDEGUARD

#include "GameObject.hpp"
#include "SmartShape.hpp"
#include <string>

class Wall : public jt::GameObject {
public:
    Wall();

    std::shared_ptr<jt::SmartShape> getShape();

private:
    std::shared_ptr<jt::SmartShape> m_shape;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;

    void doCreate() override;
};

#endif
