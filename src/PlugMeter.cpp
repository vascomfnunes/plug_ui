#include "PlugUI/PlugMeter.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

PlugMeter::PlugMeter(float maxDb)
    : m_maxGrDb(maxDb) {}

void PlugMeter::setGainReductionDb(float grDb) noexcept {
    m_grDb = grDb;
    repaint();
}

void PlugMeter::paint(juce::Graphics& g) {
    using namespace Theme;

    const auto b     = getLocalBounds().toFloat();
    const auto inner = b;

    auto meterFont = juce::Font(juce::FontOptions(fontFamily, 9.4f, juce::Font::plain));
    meterFont.setExtraKerningFactor(0.06f);
    g.setFont(meterFont);
    const float labelW = 28.0f;

    g.setColour(textMuted.withAlpha(0.85f));
    g.drawText("GR", inner.withWidth(labelW), juce::Justification::centred);

    const float barMaxW  = inner.getWidth() - labelW - 36.0f;
    const float barRight = inner.getRight() - 36.0f;
    const float barLeft  = barRight - barMaxW;

    g.setColour(panelBg);
    g.fillRect(barLeft, inner.getY() + 3.0f, barMaxW, inner.getHeight() - 6.0f);

    // Tick marks at -1, -3, -6 dB
    g.setColour(textMuted.withAlpha(0.20f));
    for (const float tickDb : {1.0f, 3.0f, 6.0f}) {
        const float tickNorm = juce::jlimit(0.0f, 1.0f, tickDb / m_maxGrDb);
        const float x        = barRight - barMaxW * tickNorm;
        g.drawVerticalLine(juce::roundToInt(x), inner.getY() + 3.0f, inner.getBottom() - 3.0f);
    }

    // Bar fills from right to left as GR increases.
    const float norm = juce::jlimit(0.0f, 1.0f, m_grDb / m_maxGrDb);
    if (norm > 0.001f) {
        const float barW = barMaxW * norm;
        const float barX = barRight - barW;
        g.setColour(meterFill.withMultipliedBrightness(juce::jmap(norm, 0.0f, 1.0f, 0.75f, 1.0f)));
        g.fillRect(barX, inner.getY() + 3.0f, barW, inner.getHeight() - 6.0f);
    }

    // dB value on the right
    g.setColour(norm > 0.01f ? juce::Colours::white : textMuted);
    const juce::String dbText = (m_grDb > 0.05f) ? "-" + juce::String(m_grDb, 1) + " dB" : "0.0 dB";
    g.drawText(dbText, inner.withLeft(inner.getRight() - 36.0f), juce::Justification::centred);
}

} // namespace PlugUI
