#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugVuMeter : public juce::Component {
  public:
    PlugVuMeter() = default;

    /** Sets the current VU level in dB */
    void setLevel(float levelDb) noexcept;

    /** Sets whether the unit is lit (e.g. not bypassed) */
    void setLit(bool isLit) noexcept;

    /** Sets the short label (e.g., "IN" or "OUT") */
    void setLabel(const juce::String& text) noexcept;

    void paint(juce::Graphics& g) override;

  private:
    float m_currentDb{-100.0f};
    bool m_isLit{true};
    juce::String m_label{"VU"};

    static constexpr float minVu      = -20.0f;
    static constexpr float maxVu      = 3.0f;
    static constexpr float startAngle = juce::MathConstants<float>::pi * 1.10f;
    static constexpr float endAngle   = juce::MathConstants<float>::pi * 1.90f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugVuMeter)
};

} // namespace PlugUI
