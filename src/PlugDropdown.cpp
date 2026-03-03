#include "PlugUI/PlugDropdown.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

PlugDropdown::PlugDropdown() {
  label_.setText("SELECT", juce::dontSendNotification);
  label_.setJustificationType(juce::Justification::centredLeft);
  auto font = juce::Font(
      juce::FontOptions(Theme::fontFamily, Theme::fontSection, juce::Font::plain));
  font.setExtraKerningFactor(0.06f);
  label_.setFont(font);
  label_.setColour(juce::Label::textColourId, Theme::textMuted);

  addAndMakeVisible(label_);
  addAndMakeVisible(comboBox_);
}

void PlugDropdown::setLabelText(const juce::String &text) {
  label_.setText(text.toUpperCase(), juce::dontSendNotification);
}

void PlugDropdown::addItem(const juce::String &text, int id) {
  comboBox_.addItem(text, id);
}

void PlugDropdown::clear(juce::NotificationType notification) {
  comboBox_.clear(notification);
}

void PlugDropdown::setSelectedId(int id, juce::NotificationType notification) {
  comboBox_.setSelectedId(id, notification);
}

int PlugDropdown::getSelectedId() const { return comboBox_.getSelectedId(); }

void PlugDropdown::resized() {
  auto area = getLocalBounds();
  const bool hasLabel = label_.getText().isNotEmpty();
  if (hasLabel) {
    const int labelHeight = juce::jmax(12, juce::jmin(16, area.getHeight() / 3));
    label_.setBounds(area.removeFromTop(labelHeight));
    area.removeFromTop(2);
    label_.setVisible(true);
  } else {
    label_.setVisible(false);
  }

  comboBox_.setBounds(area);
}

} // namespace PlugUI
