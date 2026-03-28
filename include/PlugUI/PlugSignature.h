#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI {

class PlugSignature : public juce::Component {
  public:
    PlugSignature(const juce::String& versionStr);

    void setUiScale(float scale);
    void resized() override;

  private:
    juce::Label m_brandLabel;
    float m_uiScale{1.0f};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlugSignature)
};

} // namespace PlugUI
