#pragma once

#include <functional>
#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugIntegerSelector : public juce::Component {
  public:
    PlugIntegerSelector();

    void setLabelText(const juce::String& text);
    void setRange(int minimum, int maximum, int step = 1);
    void setValue(int value, juce::NotificationType notification = juce::sendNotificationSync);
    int getValue() const noexcept { return value_; }

    std::function<void(int)> onValueChanged;

    void resized() override;

  private:
    void changeBy(int delta);
    void refreshValueLabel();

    juce::Label label_;
    juce::TextButton minusButton_{"-"};
    juce::Label valueLabel_;
    juce::TextButton plusButton_{"+"};

    int min_   = 1;
    int max_   = 32;
    int step_  = 1;
    int value_ = 1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugIntegerSelector)
};

} // namespace PlugUI
