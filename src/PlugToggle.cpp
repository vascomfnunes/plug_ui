#include "PlugUI/PlugToggle.h"

namespace PlugUI {

PlugToggle::PlugToggle(const juce::String& text) {
    setButtonText(text.toUpperCase());
    setClickingTogglesState(true);
}

} // namespace PlugUI
