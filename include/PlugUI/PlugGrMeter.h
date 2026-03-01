#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

/** Analog-style compressor gain-reduction meter.
 *
 *  Displays 0–15 dB of gain reduction using a needle on a cream-paper
 *  face, matching the visual language of the PlugVuMeter.
 *
 *  Scale orientation (mirrors real hardware compressor meters, e.g. SSL Bus):
 *    - Needle rests at the RIGHT  (0 dB GR = no compression)
 *    - Needle deflects LEFT as GR increases (more compression)
 *  Ticks: major at 0, 2, 4, 6, 8, 10, 15 dB — minor at every other integer.
 */
class PlugGrMeter : public juce::Component {
public:
  PlugGrMeter() = default;

  /** Sets the current gain reduction in dB (positive = more reduction). */
  void setGainReductionDb(float grDb) noexcept;

  void paint(juce::Graphics &g) override;

private:
  float m_grDb{0.0f};

  static constexpr float maxGr      = 15.0f;
  // Arc shared with PlugVuMeter for visual consistency
  static constexpr float startAngle = juce::MathConstants<float>::pi * 1.10f;
  static constexpr float endAngle   = juce::MathConstants<float>::pi * 1.90f;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugGrMeter)
};

} // namespace PlugUI
