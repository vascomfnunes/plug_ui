#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugSectionLabel : public juce::Component {
  public:
    explicit PlugSectionLabel(const juce::String& title);
    void paint(juce::Graphics&) override;

  private:
    juce::String m_title;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugSectionLabel)
};

} // namespace PlugUI
