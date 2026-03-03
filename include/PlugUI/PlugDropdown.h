#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugDropdown : public juce::Component {
public:
  PlugDropdown();

  void setLabelText(const juce::String &text);
  void addItem(const juce::String &text, int id);
  void clear(juce::NotificationType notification = juce::dontSendNotification);
  void setSelectedId(int id,
                     juce::NotificationType notification = juce::dontSendNotification);
  int getSelectedId() const;

  juce::ComboBox &getComboBox() noexcept { return comboBox_; }
  juce::Label &getLabel() noexcept { return label_; }

  void resized() override;

private:
  juce::Label label_;
  juce::ComboBox comboBox_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugDropdown)
};

} // namespace PlugUI
