#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

/** Thin vertical LED column meter.
    Shows signal level in dBFS with 10 amber/orange/red LED segments.
    Scale: -30 dBFS (bottom) to 0 dBFS (top).
    Intended for flanking the PlugGrMeter to show L/R output levels. */
class PlugLedMeter : public juce::Component {
  public:
    PlugLedMeter() = default;

    /** Set the current level in dBFS (negative = below full scale). */
    void setLevelDb(float levelDb) noexcept;

    void paint(juce::Graphics& g) override;

  private:
    float m_levelDb{-80.0f};

    static constexpr int N_SEGS   = 10;
    static constexpr float MIN_DB = -30.0f;
    static constexpr float MAX_DB = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugLedMeter)
};

} // namespace PlugUI
