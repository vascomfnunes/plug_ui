#pragma once

#include <functional>
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class DragMidiComponent : public juce::Component, public juce::DragAndDropContainer {
  public:
    using MidiProvider = std::function<juce::MidiFile()>;

    DragMidiComponent();
    ~DragMidiComponent() override = default;

    void setText(const juce::String& text);
    void setMidiProvider(MidiProvider provider);
    void setMidiFile(const juce::MidiFile& midiFile);

    void paint(juce::Graphics& g) override;
    void mouseDown(const juce::MouseEvent& e) override;
    void mouseDrag(const juce::MouseEvent& e) override;
    void mouseUp(const juce::MouseEvent& e) override;
    void mouseEnter(const juce::MouseEvent& e) override;
    void mouseExit(const juce::MouseEvent& e) override;

  private:
    void updateCursor();
    bool canDrag() const;
    juce::File writeTempMidiFile() const;

    juce::String text_ = "DRAG MIDI";
    MidiProvider provider_;
    juce::MidiFile staticMidi_;
    bool hasStaticMidi_ = false;
    bool dragStarted_   = false;

    static constexpr int kDragStartDistancePx = 6;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DragMidiComponent)
};

} // namespace PlugUI
