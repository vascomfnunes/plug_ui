#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugActionButton : public juce::TextButton {
public:
  explicit PlugActionButton(const juce::String &text = {});

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugActionButton)
};

} // namespace PlugUI
