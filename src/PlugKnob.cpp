#include "PlugUI/PlugKnob.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

void PlugKnob::init(const juce::String& labelText) {
    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    slider.setPopupDisplayEnabled(true, true, nullptr);
    slider.setPaintingIsUnclipped(true);

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
    const int tighten = juce::roundToInt(static_cast<float>(labelHeight) * 0.4f);
    label.setBounds(area.removeFromTop(labelHeight).translated(0, tighten));

    // Force square bounds to reduce padding, pulling the tooltip closer to the
    // visual knob. The bounds are shrunk to wrap the visual knob exactly,
    // and LookAndFeel compensates by drawing the knob full-size and unclipped.
    const auto controlH = juce::jmax(0, area.getHeight());
    auto sliderArea     = area.withHeight(controlH).translated(0, -tighten / 2);
    const int fullSq    = juce::jmin(sliderArea.getWidth(), sliderArea.getHeight());
    const int sqSize    = juce::roundToInt(static_cast<float>(fullSq) * 0.76f);
    slider.setBounds(sliderArea.withSizeKeepingCentre(sqSize, sqSize));
}

void PlugKnob::setLabelFont(const juce::Font& f) {
    label.setFont(f);
}

} // namespace PlugUI
