#include "PlugUI/PlugSegmentedControl.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

PlugSegmentedControl::PlugSegmentedControl(const std::vector<std::string>& labels) {
    const int numSegments = static_cast<int>(labels.size());
    for (int i = 0; i < numSegments; ++i) {
        auto btn = std::make_unique<juce::ToggleButton>();
        btn->setButtonText(juce::String(labels[static_cast<size_t>(i)]).toUpperCase());
        btn->setClickingTogglesState(true);
        btn->setRadioGroupId(4201);
        btn->setColour(juce::ToggleButton::textColourId, Theme::textMuted);
        btn->getProperties().set("plugSegment", true);

        int edgeFlags = 0;
        if (i > 0)
            edgeFlags |= juce::Button::ConnectedOnLeft;
        if (i < numSegments - 1)
            edgeFlags |= juce::Button::ConnectedOnRight;
        btn->getProperties().set("plugSegmentEdge", edgeFlags);
        btn->setConnectedEdges(edgeFlags);

        const int id = i + 1; // 1-based
        btn->onClick = [this, id] { handleSegmentClick(id); };

        addAndMakeVisible(btn.get());
        m_buttons.push_back(std::move(btn));
    }

    if (!m_buttons.empty())
        setSelectedId(1, juce::dontSendNotification);
}

void PlugSegmentedControl::setSelectedId(int selectedId, juce::NotificationType notification) {
    const int safeId = juce::jlimit(1, static_cast<int>(m_buttons.size()), selectedId);
    for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
        m_buttons[static_cast<size_t>(i)]->setToggleState(i + 1 == safeId,
                                                          juce::dontSendNotification);
    if (notification != juce::dontSendNotification && onSelectionChanged)
        onSelectionChanged(safeId);
}

int PlugSegmentedControl::getSelectedId() const noexcept {
    for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i) {
        if (m_buttons[static_cast<size_t>(i)]->getToggleState())
            return i + 1;
    }
    return 1;
}

void PlugSegmentedControl::handleSegmentClick(int selectedId) {
    setSelectedId(selectedId, juce::dontSendNotification);
    if (onSelectionChanged)
        onSelectionChanged(selectedId);
}

void PlugSegmentedControl::resized() {
    auto b      = getLocalBounds();
    const int n = static_cast<int>(m_buttons.size());
    if (n == 0)
        return;
    const int segmentW = b.getWidth() / n;
    for (int i = 0; i < n; ++i) {
        if (i < n - 1)
            m_buttons[static_cast<size_t>(i)]->setBounds(b.removeFromLeft(segmentW));
        else
            m_buttons[static_cast<size_t>(i)]->setBounds(b); // last gets remainder
    }
}

} // namespace PlugUI
