#include <PlugUI/PlugSignature.h>
#include <PlugUI/PlugTheme.h>

namespace PlugUI {

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
PlugSignature::PlugSignature(const juce::String& /*versionStr*/) {
    m_brandLabel.setText("VASCO NUNES", juce::dontSendNotification);
    m_brandLabel.setJustificationType(juce::Justification::centred);
    m_brandLabel.setColour(juce::Label::textColourId,
                           Theme::textMuted.withAlpha(0.65f)); // Confident, readable opacity
    addAndMakeVisible(m_brandLabel);
}

void PlugSignature::setUiScale(float scale) {
    if (juce::exactlyEqual(m_uiScale, scale))
        return;
    m_uiScale = scale;
}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void PlugSignature::resized() {
    auto bounds = getLocalBounds();

    auto brandFont =
        juce::Font(juce::FontOptions(Theme::fontFamily, 15.5f * m_uiScale, juce::Font::plain));
    brandFont.setExtraKerningFactor(0.12f);
    m_brandLabel.setFont(brandFont);

    const int brandH = juce::roundToInt(20.0f * m_uiScale);

    int labelY = bounds.getBottom() - brandH;
    m_brandLabel.setBounds(bounds.getX(), labelY, bounds.getWidth(), brandH);
}

} // namespace PlugUI
