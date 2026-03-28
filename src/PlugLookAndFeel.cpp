#include "PlugUI/PlugLookAndFeel.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

using namespace PlugUI::Theme;

PlugLookAndFeel::PlugLookAndFeel() {
  setColour(juce::Slider::textBoxTextColourId, textValue);
  setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::transparentBlack);
  setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);
  setColour(juce::Slider::rotarySliderFillColourId, accent);
  setColour(juce::Slider::rotarySliderOutlineColourId, knobArc);
  setColour(juce::Label::textColourId, textMuted);
  setColour(juce::ResizableWindow::backgroundColourId, background);
}

juce::Font PlugLookAndFeel::getSliderPopupFont(juce::Slider &) {
  auto f = juce::Font(juce::FontOptions(fontFamily, 13.5f, juce::Font::plain));
  f.setExtraKerningFactor(0.03f);
  return f;
}

int PlugLookAndFeel::getSliderPopupPlacement(juce::Slider &) {
  // Allow all directions; JUCE picks whichever fits — no directional preference
  // forces the bubble to stay tight against the knob in any orientation.
  return juce::BubbleComponent::above | juce::BubbleComponent::below |
         juce::BubbleComponent::left | juce::BubbleComponent::right;
}

void PlugLookAndFeel::drawBubble(juce::Graphics &g, juce::BubbleComponent &,
                                 const juce::Point<float> &tip,
                                 const juce::Rectangle<float> &body) {
  // Near-black, slightly warm, semi-transparent — matches the dark glass
  // aesthetic of the meter and knob faces.
  juce::Path p;
  p.addBubble(body.reduced(0.5f),
              body.getUnion(juce::Rectangle<float>(tip.x, tip.y, 1.0f, 1.0f)), tip, 4.0f,
              5.5f);

  g.setColour(juce::Colour(0xFF080A0C).withAlpha(0.91f));
  g.fillPath(p);
  g.setColour(juce::Colour(0xFF2E2C28).withAlpha(0.65f));
  g.strokePath(p, juce::PathStrokeType(0.8f));
}

juce::Font PlugLookAndFeel::getLabelFont(juce::Label &label) {
  const auto &f = label.getFont();
  if (f.getHeight() > 1.0f)
    return f;
  auto small = juce::Font(juce::FontOptions(fontFamily, fontValue, juce::Font::plain));
  small.setExtraKerningFactor(0.06f);
  return small;
}

juce::Label *PlugLookAndFeel::createSliderTextBox(juce::Slider &slider) {
  auto *label = LookAndFeel_V4::createSliderTextBox(slider);
  label->setVisible(false);
  return label;
}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void PlugLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                                       int height, float sliderPos,
                                       float rotaryStartAngle, float rotaryEndAngle,
                                       juce::Slider &slider) {
  auto fullBounds =
      juce::Rectangle<float>(static_cast<float>(x), static_cast<float>(y),
                             static_cast<float>(width), static_cast<float>(height));

  // The knob fills 100% of the bounds so tooltips are tight against it.
  const float available = juce::jmin(fullBounds.getWidth(), fullBounds.getHeight());
  const float radius = available * 0.50f; // knob radius
  if (radius <= 0.0f)
    return;

  const float diameter = radius * 2.0f;
  auto knobBounds = fullBounds.withSizeKeepingCentre(diameter, diameter);
  const auto centre = knobBounds.getCentre();
  const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
  const bool enabled = slider.isEnabled();

  // ── Rim width ────────────────────────────────────────────────────────────
  const float rimWidth = juce::jmax(knobRimMin, radius * knobRimFraction);

  // ── Soft drop shadow (radial gradient, fades well before edge) ───────────
  {
    const float shadowR = available * 0.618f;
    auto shadowBounds = fullBounds.withSizeKeepingCentre(shadowR * 2.0f, shadowR * 2.0f)
                            .translated(0.0f, radius * 0.11f);
    juce::ColourGradient shadowGrad(juce::Colours::black.withAlpha(0.56f), centre.x,
                                    centre.y + radius * 0.11f,
                                    juce::Colours::transparentBlack, centre.x + shadowR,
                                    centre.y + radius * 0.11f, true);
    shadowGrad.addColour(0.52, juce::Colours::black.withAlpha(0.38f));
    shadowGrad.addColour(0.72, juce::Colours::black.withAlpha(0.16f));
    shadowGrad.addColour(0.90, juce::Colours::black.withAlpha(0.02f));
    g.setGradientFill(shadowGrad);
    g.fillEllipse(shadowBounds);
  }

  // ── Wide dark rim / shelf / bezel ────────────────────────────────────────
  {
    juce::ColourGradient rimGrad(knobRimLight.brighter(0.08f), centre.x,
                                 knobBounds.getY(), knobRimDark.darker(0.10f), centre.x,
                                 knobBounds.getBottom(), false);
    g.setGradientFill(rimGrad);
    g.fillEllipse(knobBounds);

    {
      juce::Path topArc;
      topArc.addCentredArc(centre.x, centre.y, radius - 0.5f, radius - 0.5f, 0.0f,
                           -juce::MathConstants<float>::pi * 0.85f,
                           juce::MathConstants<float>::pi * 0.85f, true);
      g.setColour(juce::Colours::white.withAlpha(0.05f));
      g.strokePath(topArc, juce::PathStrokeType(0.7f));
    }

    g.setColour(juce::Colours::white.withAlpha(0.06f));
    g.drawEllipse(knobBounds.reduced(0.7f), 0.8f);
    g.setColour(juce::Colours::black.withAlpha(0.22f));
    g.drawEllipse(knobBounds.reduced(1.2f), 0.8f);
  }

  // ── Soft inner shadow (rim → body transition) ────────────────────────────
  auto bodyBounds = knobBounds.reduced(rimWidth);
  {
    g.setColour(juce::Colours::black.withAlpha(0.20f));
    g.drawEllipse(bodyBounds.expanded(0.3f), 1.0f);
    g.setColour(juce::Colours::black.withAlpha(0.10f));
    g.drawEllipse(bodyBounds.expanded(0.8f), 0.7f);
  }

  // ── Directional shadow on bezel (bottom-right) ────────────────────────────
  {
    const float bodyR = bodyBounds.getWidth() * 0.5f;
    const float shadowOff = rimWidth * 0.45f;
    juce::ColourGradient dirShadow(
        juce::Colours::transparentBlack, centre.x + shadowOff * 0.3f,
        centre.y + shadowOff * 0.3f, juce::Colours::black.withAlpha(0.34f),
        centre.x + shadowOff * 0.3f + bodyR + rimWidth * 0.6f,
        centre.y + shadowOff * 0.3f, true);
    dirShadow.addColour(0.60, juce::Colours::transparentBlack);
    dirShadow.addColour(0.72, juce::Colours::black.withAlpha(0.22f));
    g.setGradientFill(dirShadow);
    g.fillEllipse(knobBounds);
  }

  // ── Knob body ────────────────────────────────────────────────────────────
  {
    const float bodyR = bodyBounds.getWidth() * 0.5f;
    juce::ColourGradient bodyGrad(knobFillCentre.brighter(0.12f), centre.x,
                                  bodyBounds.getY(), knobFill.darker(0.30f), centre.x,
                                  bodyBounds.getBottom(), false);
    bodyGrad.addColour(0.55, knobFill);
    g.setGradientFill(bodyGrad);
    g.fillEllipse(bodyBounds);

    juce::ColourGradient lowerShadow(juce::Colours::transparentBlack, centre.x, centre.y,
                                     juce::Colours::black.withAlpha(0.42f), centre.x,
                                     bodyBounds.getBottom(), false);
    g.setGradientFill(lowerShadow);
    g.fillEllipse(bodyBounds);

    juce::Path specular;
    specular.addCentredArc(centre.x, centre.y, bodyR * 0.62f, bodyR * 0.62f, 0.0f,
                           -juce::MathConstants<float>::pi * 0.55f,
                           juce::MathConstants<float>::pi * 0.55f, true);
    juce::ColourGradient specGrad(juce::Colours::white.withAlpha(0.16f), centre.x,
                                  bodyBounds.getY() + bodyR * 0.22f,
                                  juce::Colours::transparentWhite, centre.x,
                                  bodyBounds.getY() + bodyR * 0.60f, false);
    g.setGradientFill(specGrad);
    g.strokePath(specular, juce::PathStrokeType(bodyR * 0.13f));
  }

  // ── White pointer / indicator line ───────────────────────────────────────
  {
    const float bodyR = bodyBounds.getWidth() * 0.5f;
    const float pointerLength = bodyR * 0.78f;
    const float pointerThickness = juce::jmax(1.3f, radius * 0.028f);

    g.setColour(knobIndicator.withAlpha(enabled ? 0.92f : 0.40f));
    juce::Path indicator;
    indicator.addRoundedRectangle(-pointerThickness * 0.5f, -pointerLength,
                                  pointerThickness, pointerLength,
                                  pointerThickness * 0.35f);
    g.fillPath(indicator,
               juce::AffineTransform::rotation(angle).translated(centre.x, centre.y));
  }

  // ── Highlight ring (optional) ────────────────────────────────────────────
  if (slider.getProperties().getWithDefault("ring", false)) {
    g.setColour(accent.withMultipliedAlpha(0.15f));
    g.drawEllipse(knobBounds.expanded(3.0f), 1.5f);
  }
}

void PlugLookAndFeel::drawLinearSlider(juce::Graphics &g, int x, int y, int width,
                                       int height, float sliderPos, float, float,
                                       juce::Slider::SliderStyle, juce::Slider &) {
  const float trackThick = 3.0f;
  const float thumbR = 4.5f;
  const float midY = static_cast<float>(y) + static_cast<float>(height) * 0.5f;
  const float left = static_cast<float>(x) + thumbR;
  const float right = static_cast<float>(x + width) - thumbR;

  g.setColour(borderSubtle.withAlpha(0.85f));
  g.fillRoundedRectangle(left, midY - trackThick * 0.5f, juce::jmax(0.0f, right - left),
                         trackThick, trackThick * 0.5f);

  const float fillW = juce::jmax(0.0f, sliderPos - left);
  g.setColour(accentBright.withAlpha(0.92f));
  g.fillRoundedRectangle(left, midY - trackThick * 0.5f, fillW, trackThick,
                         trackThick * 0.5f);

  g.setColour(knobFillCentre);
  g.fillEllipse(sliderPos - thumbR, midY - thumbR, thumbR * 2.0f, thumbR * 2.0f);
  g.setColour(accentBright);
  g.drawEllipse(sliderPos - thumbR, midY - thumbR, thumbR * 2.0f, thumbR * 2.0f, 1.0f);
}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void PlugLookAndFeel::drawToggleButton(juce::Graphics &g, juce::ToggleButton &button,
                                       bool shouldDrawButtonAsHighlighted,
                                       bool /*shouldDrawButtonAsDown*/) {
  const auto b = button.getLocalBounds().toFloat();
  const auto r = b.reduced(0.5f);
  auto paintLedGlow = [&g](juce::Rectangle<float> bounds, float corner,
                           juce::Colour ledBase, float haloAlpha, float innerAlpha) {
    // Outer soft bloom to simulate light bleeding from a lit cap.
    auto glowBounds = bounds.expanded(3.0f, 2.0f);
    juce::ColourGradient halo(ledBase.withAlpha(haloAlpha), glowBounds.getCentreX(),
                              glowBounds.getCentreY(), juce::Colours::transparentBlack,
                              glowBounds.getRight(), glowBounds.getBottom(), true);
    halo.addColour(0.50, ledBase.withAlpha(haloAlpha * 0.45f));
    halo.addColour(0.82, juce::Colours::transparentBlack);
    g.setGradientFill(halo);
    g.fillRoundedRectangle(glowBounds, corner + 2.0f);

    // Inner bright spot to give the "emitter" feel.
    juce::ColourGradient inner(juce::Colours::white.withAlpha(innerAlpha),
                               bounds.getCentreX(), bounds.getY() + 1.2f,
                               juce::Colours::transparentWhite, bounds.getCentreX(),
                               bounds.getBottom() - 1.0f, false);
    g.setGradientFill(inner);
    g.fillRoundedRectangle(bounds.reduced(1.0f), juce::jmax(1.2f, corner - 1.0f));
  };
  auto paintUnlitCap = [&g](juce::Rectangle<float> bounds, float corner) {
    // OFF state: faint smoked cap to imply no internal illumination.
    juce::ColourGradient smoke(juce::Colour(0xff000000).withAlpha(0.10f),
                               bounds.getCentreX(), bounds.getY() + 1.0f,
                               juce::Colour(0xff000000).withAlpha(0.20f),
                               bounds.getCentreX(), bounds.getBottom(), false);
    g.setGradientFill(smoke);
    g.fillRoundedRectangle(bounds.reduced(0.8f), corner);

    // Slight inner falloff so off state feels less emissive.
    juce::ColourGradient innerDim(juce::Colours::transparentBlack, bounds.getCentreX(),
                                  bounds.getY() + 1.0f,
                                  juce::Colours::black.withAlpha(0.12f),
                                  bounds.getCentreX(), bounds.getBottom() - 1.0f, false);
    g.setGradientFill(innerDim);
    g.fillRoundedRectangle(bounds.reduced(1.5f), juce::jmax(1.1f, corner - 1.2f));
  };

  const bool isOn = button.getToggleState();
  const bool isSegment = button.getProperties().contains("plugSegment");
  const bool isPill = button.getProperties().contains("plugPill");
  const float fontScale =
      static_cast<float>(button.getProperties().getWithDefault("plugFontScale", 1.0f));

  if (isSegment) {
    // ── Segmented control segment ────────────────────────────────────────
    const float corner = 2.2f;

    const juce::Colour base = isOn ? accent : buttonOff.brighter(0.08f);
    if (isOn)
      paintLedGlow(r, corner, accentBright, 0.16f, 0.08f);
    else
      paintUnlitCap(r, corner);

    // Slight vertical gradient to mimic hardware cap depth.
    juce::ColourGradient fill(base.brighter(isOn ? 0.16f : 0.10f), r.getCentreX(),
                              r.getY(), base.darker(isOn ? 0.18f : 0.14f), r.getCentreX(),
                              r.getBottom(), false);
    g.setGradientFill(fill);
    g.fillRoundedRectangle(r, corner);

    // Bevel cues: fine top light and lower shade.
    g.setColour(juce::Colours::white.withAlpha(isOn ? 0.22f : 0.12f));
    g.drawLine(r.getX() + 1.2f, r.getY() + 1.0f, r.getRight() - 1.2f, r.getY() + 1.0f,
               0.8f);
    g.setColour(juce::Colours::black.withAlpha(isOn ? 0.18f : 0.28f));
    g.drawLine(r.getX() + 1.0f, r.getBottom() - 1.0f, r.getRight() - 1.0f,
               r.getBottom() - 1.0f, 0.9f);

    g.setColour(juce::Colours::white.withAlpha(isOn ? 0.22f : 0.10f));
    g.drawRoundedRectangle(r, corner, 0.8f);

    // Subtle divider lines between segments
    const int edge = button.getProperties().getWithDefault("plugSegmentEdge", 0);
    if ((edge & juce::Button::ConnectedOnRight) != 0) {
      g.setColour(borderSubtle.withAlpha(0.50f));
      g.drawLine(r.getRight(), r.getY() + 2.0f, r.getRight(), r.getBottom() - 2.0f, 0.8f);
    }

    // Text — active: dark for contrast, inactive: muted off-white
    auto btnFont = juce::Font(juce::FontOptions(
        fontFamily, juce::jmax(fontSegment, b.getHeight() * 0.42f) * fontScale,
        juce::Font::plain));
    btnFont.setExtraKerningFactor(0.02f);
    g.setFont(btnFont);
    g.setColour(isOn ? buttonOnText : buttonOffText);
    g.drawFittedText(button.getButtonText().toUpperCase(), b.toNearestInt(),
                     juce::Justification::centred, 1);
  } else if (isPill) {
    // ── Pill button (A, LF, etc.) ────────────────────────────────────────
    const float corner = b.getHeight() * 0.28f;
    const juce::Colour activeBase = button.getProperties().contains("plugPillRed")
                                        ? juce::Colour(0xffca5547)
                                        : accent;
    const juce::Colour base = isOn ? activeBase : buttonOff;
    if (isOn)
      paintLedGlow(r, corner, activeBase.brighter(0.22f), 0.20f, 0.10f);
    else
      paintUnlitCap(r, corner);

    juce::ColourGradient fill(base.brighter(isOn ? 0.18f : 0.10f), r.getCentreX(),
                              r.getY() + 0.5f, base.darker(isOn ? 0.22f : 0.16f),
                              r.getCentreX(), r.getBottom() - 0.5f, false);
    g.setGradientFill(fill);
    g.fillRoundedRectangle(r, corner);

    // Subtle inset at bottom to increase "pressed hardware cap" feel.
    g.setColour(juce::Colours::black.withAlpha(isOn ? 0.20f : 0.30f));
    g.drawLine(r.getX() + 1.2f, r.getBottom() - 1.0f, r.getRight() - 1.2f,
               r.getBottom() - 1.0f, 0.9f);
    g.setColour(juce::Colours::white.withAlpha(isOn ? 0.24f : 0.12f));
    g.drawLine(r.getX() + 1.2f, r.getY() + 1.0f, r.getRight() - 1.2f, r.getY() + 1.0f,
               0.8f);
    g.setColour(borderSubtle.withAlpha(isOn ? 0.48f : 0.70f));
    g.drawRoundedRectangle(r, corner, 0.8f);

    const float fontSize = juce::jmax(7.0f, b.getHeight() * 0.56f * fontScale);
    auto btnFont = juce::Font(juce::FontOptions(fontFamily, fontSize, juce::Font::plain));
    btnFont.setExtraKerningFactor(0.06f);
    g.setFont(btnFont);
    // Active: dark text for contrast; Inactive: muted off-white
    g.setColour(isOn ? buttonOnText : buttonOffText);
    g.drawFittedText(button.getButtonText(), b.toNearestInt(),
                     juce::Justification::centred, 1);
  } else {
    // ── Generic toggle button (on/off) ───────────────────────────────────
    const auto bgColour = isOn ? buttonOn : buttonOff;
    const auto txtColour = isOn ? buttonOnText : buttonOffText;

    const float cornerRadius = 2.0f;
    if (isOn)
      paintLedGlow(r, cornerRadius, accentBright, 0.14f, 0.07f);
    else
      paintUnlitCap(r, cornerRadius);

    const float hoverBoost = shouldDrawButtonAsHighlighted ? 0.10f : 0.06f;
    juce::ColourGradient fill(bgColour.brighter(hoverBoost), r.getCentreX(), r.getY(),
                              bgColour.darker(0.16f), r.getCentreX(), r.getBottom(),
                              false);
    g.setGradientFill(fill);
    g.fillRoundedRectangle(r, cornerRadius);
    g.setColour(juce::Colours::white.withAlpha(isOn ? 0.20f : 0.10f));
    g.drawLine(r.getX() + 1.0f, r.getY() + 1.0f, r.getRight() - 1.0f, r.getY() + 1.0f,
               0.8f);
    g.setColour(juce::Colours::black.withAlpha(0.22f));
    g.drawLine(r.getX() + 1.0f, r.getBottom() - 1.0f, r.getRight() - 1.0f,
               r.getBottom() - 1.0f, 0.9f);
    g.setColour(borderSubtle.withAlpha(0.55f));
    g.drawRoundedRectangle(r, cornerRadius, 0.8f);

    auto btnFont =
        juce::Font(juce::FontOptions(fontFamily, 8.5f * fontScale, juce::Font::plain));
    btnFont.setExtraKerningFactor(0.04f);
    g.setFont(btnFont);
    g.setColour(txtColour);
    g.drawFittedText(button.getButtonText(), b.toNearestInt(),
                     juce::Justification::centred, 1);
  }
}

void PlugLookAndFeel::drawButtonBackground(juce::Graphics &g, juce::Button &button,
                                           const juce::Colour &backgroundColour,
                                           bool shouldDrawButtonAsHighlighted,
                                           bool shouldDrawButtonAsDown) {
  auto bounds = button.getLocalBounds().toFloat().reduced(0.5f);
  juce::Colour base = backgroundColour.isTransparent() ? buttonOff : backgroundColour;
  if (shouldDrawButtonAsDown)
    base = base.darker(0.08f);
  else if (shouldDrawButtonAsHighlighted)
    base = base.brighter(0.08f);

  juce::ColourGradient fill(base.brighter(0.10f), bounds.getCentreX(), bounds.getY(),
                            base.darker(0.18f), bounds.getCentreX(), bounds.getBottom(),
                            false);
  g.setGradientFill(fill);
  g.fillRoundedRectangle(bounds, 2.0f);

  g.setColour(juce::Colours::white.withAlpha(0.12f));
  g.drawLine(bounds.getX() + 1.0f, bounds.getY() + 1.0f, bounds.getRight() - 1.0f,
             bounds.getY() + 1.0f, 0.8f);
  g.setColour(juce::Colours::black.withAlpha(0.25f));
  g.drawLine(bounds.getX() + 1.0f, bounds.getBottom() - 1.0f, bounds.getRight() - 1.0f,
             bounds.getBottom() - 1.0f, 0.8f);
  g.setColour(borderSubtle.withAlpha(0.68f));
  g.drawRoundedRectangle(bounds, 2.0f, 0.8f);
}

void PlugLookAndFeel::drawButtonText(juce::Graphics &g, juce::TextButton &button, bool,
                                     bool) {
  const auto b = button.getLocalBounds().toFloat();
  auto f = juce::Font(juce::FontOptions(
      fontFamily, juce::jmax(8.0f, b.getHeight() * 0.42f), juce::Font::plain));
  f.setExtraKerningFactor(0.04f);
  g.setFont(f);
  g.setColour(button.findColour(juce::TextButton::textColourOffId).isTransparent()
                  ? textPrimary
                  : button.findColour(juce::TextButton::textColourOffId));
  g.drawFittedText(button.getButtonText(), button.getLocalBounds(),
                   juce::Justification::centred, 1);
}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void PlugLookAndFeel::drawLabel(juce::Graphics &g, juce::Label &label) {
  g.setColour(label.findColour(juce::Label::textColourId));
  auto font = juce::Font(
      juce::FontOptions(fontFamily, label.getFont().getHeight(), juce::Font::plain));
  font.setExtraKerningFactor(0.04f);
  g.setFont(font);
  auto textArea = label.getBorderSize().subtractedFrom(label.getLocalBounds());
  g.drawText(label.getText(), textArea, label.getJustificationType(), false);
}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void PlugLookAndFeel::drawComboBox(juce::Graphics &g, int width, int height, bool,
                                   int buttonX, int buttonY, int buttonW, int buttonH,
                                   juce::ComboBox &box) {
  const auto bounds = juce::Rectangle<float>(0.5f, 0.5f, static_cast<float>(width) - 1.0f,
                                             static_cast<float>(height) - 1.0f);
  g.setColour(panelBg);
  g.fillRoundedRectangle(bounds, 2.0f);
  g.setColour(borderSubtle.withAlpha(0.55f));
  g.drawRoundedRectangle(bounds, 2.0f, 0.8f);

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

juce::Font PlugLookAndFeel::getComboBoxFont(juce::ComboBox &) {
  auto f = juce::Font(juce::FontOptions(fontFamily, 11.0f, juce::Font::plain));
  f.setExtraKerningFactor(0.02f);
  return f;
}

void PlugLookAndFeel::positionComboBoxText(juce::ComboBox &box, juce::Label &label) {
  constexpr int leftPadding = 8;
  constexpr int rightPadding = 24;
  constexpr int verticalPadding = 2;
  label.setBounds(leftPadding, verticalPadding,
                  juce::jmax(0, box.getWidth() - leftPadding - rightPadding),
                  juce::jmax(0, box.getHeight() - (verticalPadding * 2)));
  label.setJustificationType(juce::Justification::centred);
  label.setFont(getComboBoxFont(box));
}

} // namespace PlugUI
