#include "PlugUI/PlugSegmentedControl.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

PlugSegmentedControl::PlugSegmentedControl(const std::vector<std::string>& labels) {
    const int numSegments = static_cast<int>(labels.size());
    for (int i = 0; i < numSegments; ++i) {
        auto btn = std::make_unique<juce::ToggleButton>();
        btn->setButtonText(juce::String(labels[static_cast<size_t>(i)]).toUpperCase());
        btn->setClickingTogglesState(false);
        btn->setColour(juce::ToggleButton::textColourId, Theme::textMuted);
        btn->getProperties().set("plugSegment", true);

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
    m_selectedId     = safeId;
    for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
        m_buttons[static_cast<size_t>(i)]->setToggleState(i + 1 == safeId,
                                                          juce::dontSendNotification);
    if (notification != juce::dontSendNotification && onSelectionChanged)
        onSelectionChanged(safeId);
}

int PlugSegmentedControl::getSelectedId() const noexcept {
    return m_selectedId;
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
    // Dense selectors (7-8 segments) need tighter gaps to avoid tiny hit targets.
    int gap = juce::roundToInt(static_cast<float>(b.getHeight()) * 0.15f);
    if (n >= 7)
        gap = juce::jmax(1, juce::roundToInt(static_cast<float>(b.getHeight()) * 0.06f));
    else if (n >= 5)
        gap = juce::jmax(1, juce::roundToInt(static_cast<float>(b.getHeight()) * 0.10f));
    const int totalGaps = (n - 1) * gap;
    const int segmentW  = (b.getWidth() - totalGaps) / n;
    int x               = b.getX();
    for (int i = 0; i < n; ++i) {
        int w = (i == n - 1) ? b.getRight() - x : segmentW;
        m_buttons[static_cast<size_t>(i)]->setBounds(x, b.getY(), w, b.getHeight());
        x += w + gap;
    }
}

} // namespace PlugUI
