#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugLabeledSlider : public juce::Component {
  public:
    PlugLabeledSlider();

    void setLabelText(const juce::String& text);
    void setSliderStyle(juce::Slider::SliderStyle style);

    juce::Slider& getSlider() noexcept { return slider_; }
    juce::Label& getLabel() noexcept { return label_; }

    void resized() override;

  private:
    juce::Label label_;
    juce::Slider slider_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugLabeledSlider)
};

} // namespace PlugUI
