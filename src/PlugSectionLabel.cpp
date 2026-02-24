#include "PlugUI/PlugSectionLabel.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

PlugSectionLabel::PlugSectionLabel(const juce::String& title)
    : m_title(title) {}

void PlugSectionLabel::paint(juce::Graphics& g) {
    const auto b = getLocalBounds().toFloat();

    const auto headerColour = Theme::textMuted.withAlpha(0.80f);
    g.setColour(headerColour);
    auto titleFont =
        juce::Font(juce::FontOptions(Theme::fontFamily, Theme::fontSection, juce::Font::plain));
    titleFont.setExtraKerningFactor(0.03f);
    g.setFont(titleFont);
    g.drawFittedText(m_title.toUpperCase(), b.toNearestInt(), juce::Justification::centred, 1);
}

} // namespace PlugUI
