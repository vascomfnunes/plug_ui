#include "PlugUI/PlugLookAndFeel.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

using namespace PlugUI::Theme;

PlugLookAndFeel::PlugLookAndFeel() {
    setColour(juce::Slider::textBoxTextColourId, textValue);
    setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
    setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
    setColour(juce::Slider::rotarySliderFillColourId,
              accent.withMultipliedSaturation(0.70f).withMultipliedBrightness(0.92f));
    setColour(juce::Slider::rotarySliderOutlineColourId, knobArc.withAlpha(0.55f));
    setColour(juce::Label::textColourId, textMuted);
    setColour(juce::ResizableWindow::backgroundColourId, background);
}

juce::Font PlugLookAndFeel::getSliderPopupFont(juce::Slider&) {
    auto f = juce::Font(juce::FontOptions(fontFamily, 11.8f, juce::Font::plain));
    f.setExtraKerningFactor(0.03f);
    return f;
}

juce::Font PlugLookAndFeel::getLabelFont(juce::Label& label) {
    const auto& f = label.getFont();
    if (f.getHeight() > 1.0f)
        return f;
    auto small = juce::Font(juce::FontOptions(fontFamily, fontValue, juce::Font::plain));
    small.setExtraKerningFactor(0.06f);
    return small;
}

juce::Label* PlugLookAndFeel::createSliderTextBox(juce::Slider& slider) {
    auto* label = LookAndFeel_V4::createSliderTextBox(slider);
    label->setVisible(false);
    return label;
}

void PlugLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                       float sliderPos, float rotaryStartAngle,
                                       float rotaryEndAngle, juce::Slider& slider) {
    auto bounds = juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y),
                                         static_cast<float>(width), static_cast<float>(height))
                      .reduced(3.8f);

    const float radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) * 0.5f;
    if (radius <= 0.0f)
        return;

    const float diameter    = radius * 2.0f;
    auto knobBounds         = bounds.withSizeKeepingCentre(diameter, diameter);
    const auto centre       = knobBounds.getCentre();
    const float angle       = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
    const auto accentColour = slider.findColour(juce::Slider::rotarySliderFillColourId)
                                  .withAlpha(slider.isEnabled() ? 0.95f : 0.40f);

    g.setColour(knobFill);
    g.fillEllipse(knobBounds);
    g.setColour(knobArc.withAlpha(0.55f));
    g.drawEllipse(knobBounds, 1.0f);

    auto ringBounds = knobBounds.reduced(radius * 0.24f);
    juce::Path ringPath;
    ringPath.addCentredArc(centre.x, centre.y, ringBounds.getWidth() * 0.5f,
                           ringBounds.getHeight() * 0.5f, 0.0f, rotaryStartAngle, rotaryEndAngle,
                           true);
    g.setColour(knobArc.withAlpha(0.75f));
    g.strokePath(ringPath, juce::PathStrokeType(1.1f));

    juce::Path valueArc;
    valueArc.addCentredArc(centre.x, centre.y, ringBounds.getWidth() * 0.5f,
                           ringBounds.getHeight() * 0.5f, 0.0f, rotaryStartAngle, angle, true);
    g.setColour(accentColour);
    g.strokePath(valueArc, juce::PathStrokeType(1.35f));

    g.setColour(knobIndicator.withAlpha(slider.isEnabled() ? 0.95f : 0.45f));
    juce::Path indicator;
    const float indicatorLength    = ringBounds.getWidth() * 0.44f;
    const float indicatorThickness = 1.4f;
    indicator.addRoundedRectangle(-indicatorThickness * 0.5f, -indicatorLength, indicatorThickness,
                                  indicatorLength, 0.6f);
    g.fillPath(indicator, juce::AffineTransform::rotation(angle).translated(centre.x, centre.y));
}

void PlugLookAndFeel::drawToggleButton(juce::Graphics& g, juce::ToggleButton& button,
                                       bool shouldDrawButtonAsHighlighted,
                                       bool /*shouldDrawButtonAsDown*/) {
    const auto b = button.getLocalBounds().toFloat();

    const bool isOn      = button.getToggleState();
    const bool isSegment = button.getProperties().contains("plugSegment");
    const bool isPill    = button.getProperties().contains("plugPill");

    if (isSegment) {
        auto r         = b.reduced(0.5f);
        const int edge = button.getProperties().getWithDefault("plugSegmentEdge", 0);
        g.setColour(isOn ? accent : panelBg);
        g.fillRoundedRectangle(r, 2.0f);

        if ((edge & juce::Button::ConnectedOnLeft) != 0 ||
            (edge & juce::Button::ConnectedOnRight) != 0) {
            g.setColour(borderSubtle.withAlpha(0.45f));
            g.drawRect(r.toNearestInt(), 1);
        } else {
            g.setColour(borderSubtle.withAlpha(0.55f));
            g.drawRoundedRectangle(r, 2.0f, 1.0f);
        }

        auto btnFont = juce::Font(juce::FontOptions(
            fontFamily, juce::jmax(fontSegment, b.getHeight() * 0.42f), juce::Font::plain));
        btnFont.setExtraKerningFactor(0.02f);
        g.setFont(btnFont);
        g.setColour(isOn ? juce::Colours::white : textMuted.withAlpha(0.90f));
        g.drawFittedText(button.getButtonText().toUpperCase(), b.toNearestInt(),
                         juce::Justification::centred, 1);
    } else if (isPill) {
        // Small rounded badge style (A, LF, etc.)
        const float corner = b.getHeight() * 0.28f;
        const float bright = shouldDrawButtonAsHighlighted ? 1.05f : 1.0f;

        if (isOn) {
            const auto fillCol = accent.withMultipliedBrightness(0.82f * bright);
            g.setColour(fillCol);
            g.fillRoundedRectangle(b.reduced(0.5f), corner);
        } else {
            g.setColour(buttonOff.withAlpha(0.9f * bright));
            g.fillRoundedRectangle(b.reduced(0.5f), corner);
            g.setColour(borderSubtle.withAlpha(0.85f));
            g.drawRoundedRectangle(b.reduced(0.5f), corner, 1.0f);
        }

        const float fontSize = juce::jmax(7.0f, b.getHeight() * 0.60f);
        auto btnFont = juce::Font(juce::FontOptions(fontFamily, fontSize, juce::Font::plain));
        btnFont.setExtraKerningFactor(0.06f);
        g.setFont(btnFont);
        const auto textCol = isOn ? juce::Colours::white.withAlpha(0.90f * bright)
                                  : textMuted.withAlpha(0.86f * bright);
        g.setColour(textCol);
        g.drawFittedText(button.getButtonText(), b.toNearestInt(), juce::Justification::centred, 1);
    } else {
        // Generic toggle button (on/off).
        const auto bgColour  = isOn ? buttonOn : buttonOff;
        const auto txtColour = isOn ? juce::Colours::white : buttonOffText;

        const float cornerRadius = 2.0f;
        g.setColour(bgColour.withMultipliedBrightness(shouldDrawButtonAsHighlighted ? 1.08f : 1.0f));
        g.fillRoundedRectangle(b.reduced(0.5f), cornerRadius);
        g.setColour(borderSubtle.withAlpha(0.65f));
        g.drawRoundedRectangle(b.reduced(0.5f), cornerRadius, 1.0f);

        auto btnFont = juce::Font(juce::FontOptions(fontFamily, 8.5f, juce::Font::plain));
        btnFont.setExtraKerningFactor(0.04f);
        g.setFont(btnFont);
        g.setColour(txtColour);
        g.drawFittedText(button.getButtonText(), b.toNearestInt(), juce::Justification::centred, 1);
    }
}

void PlugLookAndFeel::drawLabel(juce::Graphics& g, juce::Label& label) {
    g.setColour(label.findColour(juce::Label::textColourId));
    auto font =
        juce::Font(juce::FontOptions(fontFamily, label.getFont().getHeight(), juce::Font::plain));
    font.setExtraKerningFactor(0.04f);
    g.setFont(font);
    auto textArea = label.getBorderSize().subtractedFrom(label.getLocalBounds());
    g.drawText(label.getText(), textArea, label.getJustificationType(), false);
}

void PlugLookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool, int buttonX,
                                   int buttonY, int buttonW, int buttonH, juce::ComboBox& box) {
    const auto bounds = juce::Rectangle<float>(0.5f, 0.5f, static_cast<float>(width) - 1.0f,
                                               static_cast<float>(height) - 1.0f);
    g.setColour(panelBg);
    g.fillRoundedRectangle(bounds, 2.0f);
    g.setColour(borderSubtle.withAlpha(0.65f));
    g.drawRoundedRectangle(bounds, 2.0f, 1.0f);

    const float cx = static_cast<float>(buttonX + buttonW / 2);
    const float cy = static_cast<float>(buttonY + buttonH / 2 + 0.5f);
    juce::Path arrow;
    arrow.startNewSubPath(cx - 4.0f, cy - 2.5f);
    arrow.lineTo(cx + 4.0f, cy - 2.5f);
    arrow.lineTo(cx, cy + 2.5f);
    arrow.closeSubPath();
    g.setColour(textMuted.withAlpha(box.isEnabled() ? 0.95f : 0.45f));
    g.fillPath(arrow);
}

juce::Font PlugLookAndFeel::getComboBoxFont(juce::ComboBox&) {
    auto f = juce::Font(juce::FontOptions(fontFamily, 11.0f, juce::Font::plain));
    f.setExtraKerningFactor(0.02f);
    return f;
}

void PlugLookAndFeel::positionComboBoxText(juce::ComboBox& box, juce::Label& label) {
    constexpr int leftPadding     = 8;
    constexpr int rightPadding    = 24;
    constexpr int verticalPadding = 2;
    label.setBounds(leftPadding, verticalPadding,
                    juce::jmax(0, box.getWidth() - leftPadding - rightPadding),
                    juce::jmax(0, box.getHeight() - (verticalPadding * 2)));
    label.setJustificationType(juce::Justification::centred);
    label.setFont(getComboBoxFont(box));
}

} // namespace PlugUI
