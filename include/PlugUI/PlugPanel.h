#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

/** A translucent panel frame overlay.
    Draws a subtle dark fill with a thin border. Used to visually group
    sections (e.g. meter box, glue box, colour box) in the center engine. */
class PlugPanel : public juce::Component {
public:
  PlugPanel();

  void paint(juce::Graphics &g) override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugPanel)
};

} // namespace PlugUI
