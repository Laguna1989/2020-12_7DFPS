#ifndef GAME_INTROTEXT_HPP_INCLUDEGUARD
#define GAME_INTROTEXT_HPP_INCLUDEGUARD

#include "GameObject.hpp"
#include "SmartText.hpp"
#include "color.hpp"
#include <string>
#include <vector>

class IntroText : public jt::GameObject {
public:
    IntroText(std::string const& fileName);

    bool isDone();
    void setLineTime(float lineTime);
    float getTotalTime();

    void setTextColor(jt::color col);

private:
    std::vector<std::shared_ptr<jt::SmartText>> m_texts;
    std::vector<std::string> m_lines;
    float m_lineTime { 3.0f };
    jt::color m_textColor;

    void doUpdate(float const elapsed) override;
    void doDraw() const override;
    void doCreate() override;
};

#endif
