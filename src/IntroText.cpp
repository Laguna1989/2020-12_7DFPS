#include "IntroText.hpp"
#include "Game.hpp"
#include "GameProperties.hpp"
#include "GameState.hpp"
#include "TweenAlpha.hpp"
#include "TweenBase.hpp"
#include <fstream>
#include <sstream>

IntroText::IntroText(std::string const& fileName)
{
    std::ifstream in { fileName };
    std::string line;
    while (std::getline(in, line)) {

        m_lines.push_back(line);
    }
}

void IntroText::doCreate()
{
    float const timeOffset = m_lineTime;
    float const fadeInPerc = 0.3f;
    float const stayPerc = 0.6f;
    float const fadeOutPec = 0.1f;
    for (auto i = 0U; i != m_lines.size(); ++i) {
        auto t = std::make_shared<jt::SmartText>();
        t->loadFont("assets/font.ttf", 32, getGame()->getRenderTarget());
        t->setText(m_lines.at(i));
        t->SetTextAlign(jt::SmartText::TextAlign::CENTER);
        t->setPosition(GP::GetWindowSize() * 0.5f);
        t->setShadow(jt::color { 57U, 34U, 69U, 255U }, jt::vector2 { 3, 3 });
        m_texts.push_back(t);

        using ta = jt::TweenAlpha<jt::SmartText>;
        auto tweenAlphaFadeIn = ta::create(t, timeOffset * fadeInPerc, 0U, 255U);
        tweenAlphaFadeIn->setStartDelay(timeOffset * i);
        getGame()->getCurrentSate()->add(tweenAlphaFadeIn);
        std::weak_ptr<jt::TweenBase> wp1 = tweenAlphaFadeIn;
        m_tweens.push_back(wp1);

        auto tweenAlphaFadeOut = ta::create(t, timeOffset * fadeOutPec, 255U, 0U);
        tweenAlphaFadeOut->setStartDelay(timeOffset * i + (fadeInPerc + stayPerc) * timeOffset);
        getGame()->getCurrentSate()->add(tweenAlphaFadeOut);
        std::weak_ptr<jt::TweenBase> wp2 = tweenAlphaFadeOut;
        m_tweens.push_back(wp2);
    }
}
void IntroText::doUpdate(float const elapsed)
{
    for (auto& t : m_texts) {
        t->update(elapsed);
        auto sc = t->getShadowColor();
        sc.a() = t->getColor().a();
        t->setShadowColor(sc);
    }
}
void IntroText::doDraw() const
{
    for (auto const& t : m_texts) {
        t->draw(getGame()->getRenderTarget());
    }
}

void IntroText::setLineTime(float lineTime) { m_lineTime = lineTime; }

bool IntroText::isDone() { return (m_age >= getTotalTime()); }

float IntroText::getTotalTime() { return m_lineTime * m_lines.size(); }

void IntroText::setTextColor(jt::color col)
{
    for (auto& t : m_texts) {
        t->setColor(col);
    }
}
void IntroText::cancel()
{
    for (auto& wp : m_tweens) {
        if (wp.expired()) {
            continue;
        }
        auto const p = wp.lock();
        p->cancel();
    }

    setTextColor(jt::colors::Transparent);
}
