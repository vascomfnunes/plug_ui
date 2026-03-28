#include "PlugUI/PlugSectionLabel.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

PlugSectionLabel::PlugSectionLabel(const juce::String& title)
    : m_title(title) {}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void PlugSectionLabel::paint(juce::Graphics& g) {
    const auto b            = getLocalBounds().toFloat();
    float opacity           = m_title.equalsIgnoreCase("GLUE & BLOOM") ? 0.9f : 0.75f;
    const float customAlpha = getProperties().getWithDefault("customAlpha", 1.0f);
    g.setColour(Theme::textMuted.withMultipliedAlpha(opacity * customAlpha));

    const float uiScale   = b.getHeight() / 24.0f;
    const float fontScale = getProperties().getWithDefault("fontScale", 1.0f);
    auto titleFont        = juce::Font(
        juce::FontOptions(Theme::fontFamily, 16.0f * uiScale * fontScale, juce::Font::bold));
    const float kerning = getProperties().getWithDefault("kerningFactor", 0.04f);
    titleFont.setExtraKerningFactor(kerning);
    g.setFont(titleFont);

    const bool leftAlign = getProperties().getWithDefault("leftAlign", false);
    g.drawFittedText(m_title.toUpperCase(), b.toNearestInt(),
                     leftAlign ? juce::Justification::centredLeft : juce::Justification::centred, 1);
}

} // namespace PlugUI
