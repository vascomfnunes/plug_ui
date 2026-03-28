#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugCorrelationMeter : public juce::Component, public juce::SettableTooltipClient {
  public:
    PlugCorrelationMeter() = default;

    /** Sets the stereo correlation value from -1.0 to 1.0 */
    void setCorrelation(float correlation) noexcept;

    void paint(juce::Graphics& g) override;

  private:
    float m_correlation{0.0f};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugCorrelationMeter)
};

} // namespace PlugUI
