#include "PlugUI/PlugPanel.h"
#include "PlugUI/BackgroundRenderer.h"

namespace PlugUI {

PlugPanel::PlugPanel() { setInterceptsMouseClicks(false, true); }

void PlugPanel::paint(juce::Graphics &g) {
  BackgroundRenderer::paintEnginePanel(g, getLocalBounds());
}

} // namespace PlugUI
