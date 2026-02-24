#include "PlugUI/PlugKnob.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

void PlugKnob::init(const juce::String& labelText) {
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    slider.setPopupDisplayEnabled(true, false, nullptr);

    label.setText(labelText.toUpperCase(), juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    {
        auto labelFont =
            juce::Font(juce::FontOptions(Theme::fontFamily, Theme::fontLabel, juce::Font::plain));
        labelFont.setExtraKerningFactor(0.04f);
        label.setFont(labelFont);
    }
    label.setColour(juce::Label::textColourId, Theme::textMuted);
}

void PlugKnob::setBounds(juce::Rectangle<int> area, int labelHeight) {
    const auto controlH = juce::jmax(0, area.getHeight() - labelHeight);
    label.setBounds(area.removeFromTop(labelHeight));
    slider.setBounds(area.withHeight(controlH));
}

void PlugKnob::setLabelFont(const juce::Font& f) {
    label.setFont(f);
}

} // namespace PlugUI
