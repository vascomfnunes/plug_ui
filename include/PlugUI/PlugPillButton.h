#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

/** Small rounded-rectangle toggle badge (LF, A, etc.).
    Drawing is handled by PlugLookAndFeel::drawToggleButton, so this
    component only needs to be a ToggleButton with the correct property. */
class PlugPillButton : public juce::ToggleButton {
  public:
    /** @param text  button label (e.g. "A", "LF"). */
    explicit PlugPillButton(const juce::String& text);

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugPillButton)
};

} // namespace PlugUI
