#include "PlugUI/PlugCorrelationMeter.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

void PlugCorrelationMeter::setCorrelation(float correlation) noexcept {
    m_correlation = juce::jlimit(-1.0f, 1.0f, correlation);
    repaint();
}

void PlugCorrelationMeter::paint(juce::Graphics& g) {
    using namespace Theme;

    const auto inner = getLocalBounds().toFloat().reduced(1.0f);

    const float barMaxW  = inner.getWidth();
    const float barRight = inner.getRight();
    const float barLeft  = inner.getX();
    const float barY     = inner.getY() + 1.0f;
    const float barH     = inner.getHeight() - 14.0f;

    const bool fullyJustify = getProperties().getWithDefault("fullyJustified", false);

    constexpr int segments      = 64;
    const float segW            = barMaxW / static_cast<float>(segments);
    const float segGap          = 1.0f;
    const float thicknessOffset = getProperties().getWithDefault("thicknessOffset", 0.0f);
    const float segInset        = juce::jmax(0.0f, 1.5f - thicknessOffset);

    const float centerIdx = segments * 0.5f;
    const float targetIdx = (m_correlation + 1.0f) * 0.5f * segments;

    for (int i = 0; i < segments; ++i) {
        const float segX     = barLeft + segW * static_cast<float>(i) + segGap * 0.5f;
        const float segDrawW = segW - segGap;

        bool isFilled = false;
        if (m_correlation > 0.001f) {
            if (i >= centerIdx && i < targetIdx)
                isFilled = true;
        } else if (m_correlation < -0.001f) {
            if (i < centerIdx && i >= targetIdx)
                isFilled = true;
        }

        if (isFilled) {
            juce::Colour segColour =
                (m_correlation >= 0.0f) ? juce::Colour(0xff4b8a61) : juce::Colour(0xffa45e4b);
            g.setColour(segColour.withAlpha(0.85f));
            g.fillRect(segX, barY + segInset, segDrawW, barH - segInset * 2.0f);
        } else {
            g.setColour(textMuted.withAlpha(0.05f));
            g.fillRect(segX, barY + segInset, segDrawW, barH - segInset * 2.0f);
        }
    }

    g.setColour(textMuted.withAlpha(0.25f));
    float zeroX = barLeft + (barMaxW * 0.5f);
    g.drawLine(zeroX, barY, zeroX, barY + barH, 1.5f);

    auto scaleFont = juce::Font(juce::FontOptions(fontFamily, 8.0f, juce::Font::plain));
    scaleFont.setExtraKerningFactor(0.04f);
    g.setFont(scaleFont);
    g.setColour(textMuted.withAlpha(0.77f));
    struct Tick {
        float val;
        const char* text;
    };
    for (const Tick tick : {Tick{-1.0f, "-1"}, Tick{0.0f, "0"}, Tick{1.0f, "+1"}}) {
        const float x = barLeft + barMaxW * ((tick.val + 1.0f) * 0.5f);
        g.drawText(tick.text, juce::Rectangle<float>(x - 20.0f, barY + barH + 2.0f, 40.0f, 10.0f),
                   juce::Justification::centred, false);
    }
}

} // namespace PlugUI
