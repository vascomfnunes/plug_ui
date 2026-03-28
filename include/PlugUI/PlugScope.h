#pragma once

#include <array>
#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

/** Scrolling amplitude-history scope.
    Includes pre/post min/max outlines, clip ceiling indication, and clip
   flashes.
*/
class PlugScope : public juce::Component {
  public:
    PlugScope();

    void paint(juce::Graphics& g) override;

    /** Push a single pre-computed min/max column block. */
    void pushColumn(float preMin, float preMax, float postMin, float postMax);

    /** Set the db guides to draw. */
    void setLevels(float thresholdDb, float ceilingDb);

    /** Adds clip flash brightness that gently decays. */
    void triggerClipFlash();

  private:
    static constexpr int kHistoryColumns = 384;

    struct HistoryColumn {
        float preMin  = 0.0f;
        float preMax  = 0.0f;
        float postMin = 0.0f;
        float postMax = 0.0f;
        bool hasData  = false;
    };

    std::array<HistoryColumn, kHistoryColumns> history{};
    int historyWritePos = 0;

    float clipFlashAlpha = 0.0f;
    float m_threshDb     = 0.0f;
    float m_ceilDb       = 0.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugScope)
};

} // namespace PlugUI
