#include "PlugUI/DragMidiComponent.h"
#include "PlugUI/PlugTheme.h"

#include <juce_core/juce_core.h>

namespace PlugUI {

DragMidiComponent::DragMidiComponent() { updateCursor(); }

void DragMidiComponent::setText(const juce::String &text) {
  text_ = text.toUpperCase();
  repaint();
}

void DragMidiComponent::setMidiProvider(MidiProvider provider) {
  provider_ = std::move(provider);
  hasStaticMidi_ = false;
  updateCursor();
}

void DragMidiComponent::setMidiFile(const juce::MidiFile &midiFile) {
  staticMidi_ = midiFile;
  hasStaticMidi_ = true;
  provider_ = {};
  updateCursor();
}

void DragMidiComponent::paint(juce::Graphics &g) {
  auto bounds = getLocalBounds().toFloat().reduced(0.5f);
  const bool available = canDrag();
  const bool hovered = isMouseOverOrDragging();

  const auto base = available ? Theme::buttonOff.brighter(hovered ? 0.03f : 0.01f)
                              : Theme::buttonOff.darker(0.10f);
  g.setColour(base.withAlpha(0.88f));
  g.fillRoundedRectangle(bounds, 2.0f);

  g.setColour(Theme::borderSubtle.withAlpha(hovered ? 0.76f : 0.60f));
  g.drawRoundedRectangle(bounds, 2.0f, 0.9f);

  auto content = bounds.toNearestInt().reduced(7, 0);
  auto iconArea = juce::Rectangle<float>(static_cast<float>(content.getX()),
                                         bounds.getCentreY() - 4.0f, 10.0f, 8.0f);
  juce::Path midiIcon;
  midiIcon.addRoundedRectangle(iconArea, 1.6f);
  const float pinY = iconArea.getCentreY();
  midiIcon.startNewSubPath(iconArea.getX() + 2.0f, pinY);
  midiIcon.lineTo(iconArea.getRight() - 2.0f, pinY);
  for (int i = 0; i < 3; ++i) {
    const float pinX = iconArea.getX() + 2.0f + static_cast<float>(i) * 3.0f;
    midiIcon.addEllipse(pinX - 0.55f, iconArea.getY() + 1.4f, 1.1f, 1.1f);
    midiIcon.addEllipse(pinX - 0.55f, iconArea.getBottom() - 2.5f, 1.1f, 1.1f);
  }

  g.setColour(available ? Theme::accentDim.withAlpha(0.85f) : Theme::accentDim.withAlpha(0.45f));
  g.strokePath(midiIcon, juce::PathStrokeType(0.8f));

  g.setColour(available ? Theme::textMuted.brighter(0.16f)
                        : Theme::textMuted.withAlpha(0.6f));
  auto font = juce::Font(
      juce::FontOptions(Theme::fontFamily, Theme::fontSection + 0.35f, juce::Font::plain));
  font.setExtraKerningFactor(0.06f);
  g.setFont(font);
  auto textArea = content;
  textArea.removeFromLeft(14);
  g.drawText(text_, textArea, juce::Justification::centredLeft, false);
}

void DragMidiComponent::mouseDown(const juce::MouseEvent &e) {
  juce::ignoreUnused(e);
  dragStarted_ = false;
}

void DragMidiComponent::mouseDrag(const juce::MouseEvent &e) {
  if (dragStarted_ || !e.mods.isLeftButtonDown() || e.getDistanceFromDragStart() < kDragStartDistancePx)
    return;
  if (!canDrag())
    return;

  const auto file = writeTempMidiFile();
  if (!file.existsAsFile())
    return;

  juce::StringArray files;
  files.add(file.getFullPathName());
  dragStarted_ = true;
  juce::DragAndDropContainer::performExternalDragDropOfFiles(files, false, this);
}

void DragMidiComponent::mouseUp(const juce::MouseEvent &e) {
  juce::ignoreUnused(e);
  dragStarted_ = false;
}

void DragMidiComponent::mouseEnter(const juce::MouseEvent &e) {
  juce::ignoreUnused(e);
  updateCursor();
}

void DragMidiComponent::mouseExit(const juce::MouseEvent &e) {
  juce::ignoreUnused(e);
  setMouseCursor(juce::MouseCursor::NormalCursor);
}

void DragMidiComponent::updateCursor() {
  setMouseCursor(canDrag() ? juce::MouseCursor::PointingHandCursor
                           : juce::MouseCursor::NormalCursor);
}

bool DragMidiComponent::canDrag() const {
  return hasStaticMidi_ || static_cast<bool>(provider_);
}

juce::File DragMidiComponent::writeTempMidiFile() const {
  juce::MidiFile file;
  if (provider_)
    file = provider_();
  else if (hasStaticMidi_)
    file = staticMidi_;

  if (file.getNumTracks() == 0)
    return {};

  file.setTicksPerQuarterNote(960);

  const auto out = juce::File::getSpecialLocation(juce::File::tempDirectory)
                       .getChildFile("krei-midi")
                       .getNonexistentChildFile("krei", ".mid", false);

  if (out.getParentDirectory().createDirectory().failed())
    return {};

  juce::FileOutputStream stream(out);
  if (!stream.openedOk())
    return {};
  file.writeTo(stream);
  stream.flush();
  return out;
}

} // namespace PlugUI
