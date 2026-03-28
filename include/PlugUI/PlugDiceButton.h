#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugDiceButton : public juce::Button, private juce::Timer {
  public:
    PlugDiceButton();

    void paintButton(juce::Graphics&, bool hovered, bool pressed) override;
    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;
    void clicked() override;

    void setFaceValue(int value);
    int getFaceValue() const noexcept { return faceValue_; }

  private:
    void timerCallback() override;

    int faceValue_     = 5;
    float hoverAmount_ = 0.0f;
    float clickAmount_ = 0.0f;
    bool hoverTarget_  = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugDiceButton)
};

} // namespace PlugUI
