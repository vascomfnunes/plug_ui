#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

/** A fully procedural, clean, modern, minimal background system.
    Provides a main deep charcoal-blue background with subtle tonal shifts,
    center lift, and fine micro-noise. Also provides a subtle darker engine
    panel area to group UI elements without visible hard borders. */
class BackgroundRenderer : public juce::Component {
  public:
    BackgroundRenderer() = default;

    /** Component paint override for drawing the main background.
        This allows BackgroundRenderer to be added as a root back-panel child. */
    void paint(juce::Graphics& g) override;

    /** Paints the main procedural background into the given bounds.
        Uses PlugUI's built-in texture overlay by default. Host plugins can
        pass an explicit texture pointer to override it. */
    static void paintMainBackground(juce::Graphics& g, juce::Rectangle<int> bounds,
                                    const juce::Image* overlayTexture = nullptr,
                                    float overlayOpacity              = 0.18f);

    /** Paints a subtle, borderless, darker engine panel that blends naturally. */
    static void paintEnginePanel(juce::Graphics& g, juce::Rectangle<int> bounds);

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BackgroundRenderer)
};

} // namespace PlugUI
