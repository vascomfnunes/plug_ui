#include "PlugUI/PlugLabeledSlider.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

PlugLabeledSlider::PlugLabeledSlider() {
    label_.setText("PARAM", juce::dontSendNotification);
    label_.setJustificationType(juce::Justification::centredLeft);
    auto font =
        juce::Font(juce::FontOptions(Theme::fontFamily, Theme::fontSection, juce::Font::plain));
    font.setExtraKerningFactor(0.06f);
    label_.setFont(font);
    label_.setColour(juce::Label::textColourId, Theme::textMuted);

    slider_.setSliderStyle(juce::Slider::LinearHorizontal);
    slider_.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(label_);
    addAndMakeVisible(slider_);
}

void PlugLabeledSlider::setLabelText(const juce::String& text) {
    label_.setText(text.toUpperCase(), juce::dontSendNotification);
}

void PlugLabeledSlider::setSliderStyle(juce::Slider::SliderStyle style) {
    slider_.setSliderStyle(style);
}

void PlugLabeledSlider::resized() {
    auto area           = getLocalBounds();
    const bool hasLabel = label_.getText().isNotEmpty();
    if (hasLabel) {
        const int labelHeight = juce::jmax(12, juce::jmin(16, area.getHeight() / 3));
        label_.setBounds(area.removeFromTop(labelHeight));
        area.removeFromTop(2);
        label_.setVisible(true);
    } else {
        label_.setVisible(false);
    }

    slider_.setBounds(area.reduced(0, 2));
}

} // namespace PlugUI
