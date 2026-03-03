#include "PlugUI/PlugIntegerSelector.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

PlugIntegerSelector::PlugIntegerSelector() {
  label_.setJustificationType(juce::Justification::centredLeft);
  label_.setText("BARS", juce::dontSendNotification);
  auto labelFont = juce::Font(
      juce::FontOptions(Theme::fontFamily, Theme::fontSection, juce::Font::plain));
  labelFont.setExtraKerningFactor(0.06f);
  label_.setFont(labelFont);
  label_.setColour(juce::Label::textColourId, Theme::textMuted);

  valueLabel_.setJustificationType(juce::Justification::centred);
  auto valueFont = juce::Font(
      juce::FontOptions(Theme::fontFamily, Theme::fontLabel, juce::Font::plain));
  valueFont.setExtraKerningFactor(0.04f);
  valueLabel_.setFont(valueFont);
  valueLabel_.setColour(juce::Label::textColourId, Theme::textPrimary);

  minusButton_.onClick = [this] { changeBy(-step_); };
  plusButton_.onClick = [this] { changeBy(step_); };
  minusButton_.setColour(juce::TextButton::buttonColourId, Theme::panelBg);
  minusButton_.setColour(juce::TextButton::textColourOffId, Theme::textPrimary.withAlpha(0.92f));
  plusButton_.setColour(juce::TextButton::buttonColourId, Theme::panelBg);
  plusButton_.setColour(juce::TextButton::textColourOffId, Theme::textPrimary.withAlpha(0.92f));

  addAndMakeVisible(label_);
  addAndMakeVisible(minusButton_);
  addAndMakeVisible(valueLabel_);
  addAndMakeVisible(plusButton_);

  refreshValueLabel();
}

void PlugIntegerSelector::setLabelText(const juce::String &text) {
  label_.setText(text.toUpperCase(), juce::dontSendNotification);
}

void PlugIntegerSelector::setRange(int minimum, int maximum, int step) {
  min_ = minimum;
  max_ = juce::jmax(minimum, maximum);
  step_ = juce::jmax(1, step);
  setValue(value_, juce::dontSendNotification);
}

void PlugIntegerSelector::setValue(int value, juce::NotificationType notification) {
  const int clamped = juce::jlimit(min_, max_, value);
  if (clamped == value_)
    return;
  value_ = clamped;
  refreshValueLabel();
  if (notification != juce::dontSendNotification && onValueChanged)
    onValueChanged(value_);
}

void PlugIntegerSelector::changeBy(int delta) {
  setValue(value_ + delta, juce::sendNotificationSync);
}

void PlugIntegerSelector::refreshValueLabel() {
  valueLabel_.setText(juce::String(value_), juce::dontSendNotification);
}

void PlugIntegerSelector::resized() {
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

  const int h = area.getHeight();
  const int buttonW = juce::jmin(h, area.getWidth() / 3);
  auto row = area;
  const auto minusBounds = row.removeFromLeft(buttonW).withSizeKeepingCentre(buttonW, h);
  const auto plusBounds = row.removeFromRight(buttonW).withSizeKeepingCentre(buttonW, h);
  minusButton_.setBounds(minusBounds);
  plusButton_.setBounds(plusBounds);
  valueLabel_.setBounds(row.reduced(2, 0));
}

} // namespace PlugUI
