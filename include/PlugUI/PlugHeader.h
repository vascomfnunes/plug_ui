#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

namespace PlugUI {

/** Reusable plugin header strip.
    Paints: title text (accent gold, left-column) and subtitle text (muted,
   right-column). */
class PlugHeader : public juce::Component {
public:
  /** Construct with display strings.
      @param title     e.g. "PLUGIN_NAME"
      @param version   reserved for future use
      @param subtitle  e.g. "PLUGIN DESCRIPTION" */
  PlugHeader(const juce::String &title, const juce::String &version,
             const juce::String &subtitle);

  void paint(juce::Graphics &g) override;

  /** Set the UI scale factor (affects font sizes). */
  void setUiScale(float scale);

  /** Override the width parameter portion assigned for the Left/Right strips.
   * (Fallback is 0.22f matching Nubo) */
  void setColRatio(float ratio);

private:
  float m_colRatio{0.22f};
  juce::String m_title;
  juce::String m_subtitle;
  float m_uiScale{1.0f};
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugHeader)
};

} // namespace PlugUI
