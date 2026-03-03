#include "PlugUI/PlugActionButton.h"

namespace PlugUI {

PlugActionButton::PlugActionButton(const juce::String &text) {
  setButtonText(text.toUpperCase());
}

} // namespace PlugUI
