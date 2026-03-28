#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <string>
#include <vector>

namespace PlugUI {

/** Flat segmented button strip (e.g. SMOOTH / TAPE / PUNCH).
    Accepts any number of labels via constructor.
    Selected index is 1-based to match JUCE ComboBox IDs. */
class PlugSegmentedControl : public juce::Component {
  public:
    /** Construct with the segment labels. */
    explicit PlugSegmentedControl(const std::vector<std::string>& labels);

    void setSelectedId(int selectedId,
                       juce::NotificationType notification = juce::dontSendNotification);
    int getSelectedId() const noexcept;
    void resized() override;

    /** Called when the user clicks a segment. Argument is 1-based. */
    std::function<void(int)> onSelectionChanged;

  private:
    void handleSegmentClick(int selectedId);
    std::vector<std::unique_ptr<juce::ToggleButton>> m_buttons;
    int m_selectedId{1};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugSegmentedControl)
};

} // namespace PlugUI
