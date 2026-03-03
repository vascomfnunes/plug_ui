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

  const auto base = available ? Theme::buttonOn : Theme::buttonOff;
  juce::ColourGradient grad(base.brighter(0.12f), bounds.getCentreX(), bounds.getY(),
                            base.darker(0.15f), bounds.getCentreX(), bounds.getBottom(), false);
  g.setGradientFill(grad);
  g.fillRoundedRectangle(bounds, 4.0f);

  g.setColour(Theme::borderSubtle.withAlpha(0.9f));
  g.drawRoundedRectangle(bounds, 4.0f, 1.0f);

  g.setColour(available ? Theme::buttonOnText : Theme::buttonOffText);
  auto font = juce::Font(
      juce::FontOptions(Theme::fontFamily, Theme::fontSection + 0.5f, juce::Font::bold));
  font.setExtraKerningFactor(0.06f);
  g.setFont(font);
  g.drawText(text_, getLocalBounds(), juce::Justification::centred, false);
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
