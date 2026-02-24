#include "PlugUI/PlugPillButton.h"

namespace PlugUI {

PlugPillButton::PlugPillButton(const juce::String& text) {
    setButtonText(text);
    setClickingTogglesState(true);
    getProperties().set("plugPill", true);
}

} // namespace PlugUI
