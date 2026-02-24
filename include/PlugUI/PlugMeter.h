#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

/** Horizontal gain-reduction meter. An amber bar grows from the right edge
    inward proportional to GR (0–maxDb). A "GR" label sits on the left;
    dB value on the right. */
class PlugMeter : public juce::Component {
  public:
    /** @param maxDb  maximum displayed reduction in dB (default 18). */
    explicit PlugMeter(float maxDb = 18.0f);

    /** Set the current gain reduction value (positive = dB of reduction). */
    void setGainReductionDb(float grDb) noexcept;

    void paint(juce::Graphics& g) override;

  private:
    float m_grDb{0.0f};
    float m_maxGrDb{18.0f};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugMeter)
};

} // namespace PlugUI
