#include "FastMath.hpp"
#include "MathHelper.hpp"

#include <cmath>
float mytan(float a) { return std::tan(jt::MathHelper::deg2rad(a)); }
float mytaninv(float a) { return (1.0f / std::tan(jt::MathHelper::deg2rad(a))); }
float mycos(float a) { return std::cos(jt::MathHelper::deg2rad(a)); }
float mysin(float a) { return std::sin(jt::MathHelper::deg2rad(a)); }
float wrapAngle(float a)
{
    if (a < 0) {
        while (a < 0) {
            a += 360;
        }
    } else if (a > 360.0f) {
        while (a > 360) {
            a -= 360;
        }
    }
    return a;
}
