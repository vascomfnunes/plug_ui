#include "PlugUI/PlugHeader.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

using namespace Theme;

PlugHeader::PlugHeader(const juce::String& title, const juce::String& /*version*/,
                       const juce::String& subtitle)
    : m_title(title),
      m_subtitle(subtitle) {}

void PlugHeader::setUiScale(float scale) {
    m_uiScale = scale;
}

void PlugHeader::setColRatio(float ratio) {
    m_colRatio = ratio;
}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void PlugHeader::paint(juce::Graphics& g) {
    const auto header = getLocalBounds();

    // Use ratio (default 0.22f) for leftStrip/rightStrip allocation
    const float colRatio = m_colRatio;
    const int colW       = juce::roundToInt(static_cast<float>(header.getWidth()) * colRatio);
    auto leftCol         = header.withWidth(colW);
    auto rightCol = header.withLeft(header.getRight() - colW).withWidth(colW).translated(0, 2);

    // ── Title (bold, accent gold, left aligned in left col) ───────────────────
    auto titleFont = juce::Font(
        juce::FontOptions(Theme::fontFamily, 82.0f * m_uiScale * 0.44f, juce::Font::bold));
    titleFont.setExtraKerningFactor(0.10f);

    g.setFont(titleFont);
    g.setColour(Theme::accent);
    g.drawText(m_title.toUpperCase(), leftCol.reduced(2, 0), juce::Justification::centredLeft);

    // ── Subtitle (centered in right col, muted) ──────────────────────────
    auto subtitleFont =
        juce::Font(juce::FontOptions(fontFamily, 20.0f * m_uiScale, juce::Font::plain));
    subtitleFont.setExtraKerningFactor(0.06f);
    g.setFont(subtitleFont);
    g.setColour(textMuted.withAlpha(0.95f));
    g.drawText(m_subtitle, rightCol.reduced(2, 0), juce::Justification::centredRight);
}

} // namespace PlugUI
