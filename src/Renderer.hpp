#ifndef RENDERER_HPP_GUARD
#define RENDERER_HPP_GUARD

#include "Level.hpp"
#include "SmartShape.hpp"
#include "vector.hpp"
#include <memory>
#include <vector>

void calculateWallScales(jt::vector2 const playerPos, float const angle,
    std::shared_ptr<Level> const level, std::vector<std::shared_ptr<jt::SmartShape>>& walls);

void calculateSpriteScale(jt::vector2 const playerPos, float const angle,
    jt::vector2 const spriteWorldPos, std::shared_ptr<jt::SmartDrawable> d);

#endif
