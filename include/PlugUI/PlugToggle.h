#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugToggle : public juce::ToggleButton {
  public:
    explicit PlugToggle(const juce::String& text = {});

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugToggle)
};

} // namespace PlugUI
