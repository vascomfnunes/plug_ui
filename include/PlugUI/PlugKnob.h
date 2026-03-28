#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

/** A labelled rotary knob with a label above. */
struct PlugKnob {
    juce::Slider slider;
    juce::Label label;

    /** Configure the slider style, text box, and label appearance.
        Label text is normalized to UPPERCASE to enforce PlugUI knob-label style.
        Call once after construction. */
    void init(const juce::String& labelText);

    /** Place the knob inside the given rectangle (label on top). */
    void setBounds(juce::Rectangle<int> area, int labelHeight);

    /** Set the font for the label (e.g. after UI-scale changes). */
    void setLabelFont(const juce::Font& f);
};

} // namespace PlugUI
