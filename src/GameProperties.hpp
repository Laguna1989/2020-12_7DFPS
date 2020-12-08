#ifndef GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD
#define GAME_GAMEPROPERTIES_HPP_INCLUDEGUARD

#include "color.hpp"
#include "vector.hpp"
#include <cstdint>
#include <string>

class GP {
public:
    GP() = delete;

    static jt::color PaletteBackground() { return jt::color { 14, 16, 20 }; }
    static jt::color PaletteFontFront() { return jt::color { 248, 249, 254 }; }
    static jt::color PaletteFontShadow(std::uint8_t a = 255)
    {
        return jt::color { 26, 29, 36, a };
    };
    static jt::color PaletteSelector() { return jt::color { 221, 228, 236 }; }
    static jt::color PaletteHighlight() { return jt::color { 163, 183, 205 }; }

    static jt::color PaletteColor1() { return jt::color { 38, 70, 83 }; }
    static jt::color PaletteColor2() { return jt::color { 42, 157, 143 }; }
    static jt::color PaletteColor3() { return jt::color { 233, 196, 106 }; }
    static jt::color PaletteColor4() { return jt::color { 244, 162, 97 }; }
    static jt::color PaletteColor5() { return jt::color { 231, 111, 81 }; }

    static jt::color PaletteFlashGreen() { return jt::color { 51, 143, 63 }; }
    static jt::color PaletteFlashRed() { return jt::color { 143, 33, 33 }; }

    static jt::color PaletteWallN() { return jt::color { 145, 125, 101, 255 }; }
    static jt::color PaletteWallS() { return jt::color { 181, 167, 116, 255 }; }
    static jt::color PaletteWallW() { return jt::color { 129, 144, 108, 255 }; }
    static jt::color PaletteWallE() { return jt::color { 92, 99, 94, 255 }; }

    static jt::color PalletteFloor() { return jt::color { 239, 126, 69, 255 }; }
    static jt::color PalletteSky() { return jt::color { 166, 221, 227, 255 }; }

    static std::string GameName() { return "7DFPS"; }
    static jt::vector2 GetWindowSize() { return jt::vector2 { 800, 600 }; }
    static float GetZoom() { return 1.0f; }
    static std::size_t GetDivisions()
    {
        return static_cast<std::size_t>(ceil(GetWindowSize().x() / GetZoom())) / 10;
    }
    static float GetFoVAngle() { return 45.0f; }
    static float SpriteScale() { return 16.0f; }

    static float PlayerTurningSpeed() { return 45.0f; }
    static float PlayerAccelerationFactor() { return 115.0f; }
    static float PlayerMaxSpeed() { return 0.25f; }
    static float PlayerVelocityDamping() { return 0.90f; }

    static int MapTileSizeInPixel() { return 4; }
};

#endif
