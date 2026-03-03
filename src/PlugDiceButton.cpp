#include "PlugUI/PlugDiceButton.h"
#include "PlugUI/PlugTheme.h"

#include <cmath>

namespace PlugUI {

PlugDiceButton::PlugDiceButton() : juce::Button("Dice") {
  setTooltip("Randomize seed and generate");
  setMouseCursor(juce::MouseCursor::PointingHandCursor);
}

void PlugDiceButton::paintButton(juce::Graphics &g, bool hovered, bool pressed) {
  juce::ignoreUnused(hovered, pressed);

  auto bounds = getLocalBounds().toFloat();
  if (bounds.isEmpty())
    return;

  const float scale = 1.0f + 0.02f * hoverAmount_;
  const float rotation = juce::degreesToRadians(90.0f * clickAmount_);
  auto transform = juce::AffineTransform::scale(scale, scale, bounds.getCentreX(),
                                                bounds.getCentreY())
                       .rotated(rotation, bounds.getCentreX(), bounds.getCentreY());

  auto baseBounds = bounds.reduced(1.4f);
  juce::Path diceShape;
  const float corner = juce::jmax(2.0f, baseBounds.getWidth() * 0.12f);
  diceShape.addRoundedRectangle(baseBounds, corner);
  diceShape.applyTransform(transform);

  auto base = Theme::buttonOff.brighter(0.03f + 0.04f * hoverAmount_);
  juce::ColourGradient fill(base.brighter(0.08f), bounds.getCentreX(), bounds.getY(),
                            base.darker(0.18f), bounds.getCentreX(), bounds.getBottom(), false);
  g.setGradientFill(fill);
  g.fillPath(diceShape);

  g.setColour(Theme::borderSubtle.withAlpha(0.72f));
  g.strokePath(diceShape, juce::PathStrokeType(0.7f));

  const auto pipColour = Theme::accentBright.brighter(0.10f * hoverAmount_);
  g.setColour(pipColour);

  const auto pipArea = baseBounds.reduced(baseBounds.getWidth() * 0.24f);
  const float pipR = juce::jmax(0.8f, pipArea.getWidth() * 0.075f);
  auto pip = [&](float xNorm, float yNorm) {
    auto x = pipArea.getX() + pipArea.getWidth() * xNorm;
    auto y = pipArea.getY() + pipArea.getHeight() * yNorm;
    juce::Path circle;
    circle.addEllipse(x - pipR, y - pipR, pipR * 2.0f, pipR * 2.0f);
    circle.applyTransform(transform);
    g.fillPath(circle);
  };

  const bool center = (faceValue_ == 1 || faceValue_ == 3 || faceValue_ == 5);
  const bool topLeftBottomRight = (faceValue_ == 2 || faceValue_ == 3 || faceValue_ == 4 ||
                                   faceValue_ == 5 || faceValue_ == 6);
  const bool topRightBottomLeft = (faceValue_ == 4 || faceValue_ == 5 || faceValue_ == 6);
  const bool midLeftRight = (faceValue_ == 6);

  if (topLeftBottomRight) {
    pip(0.0f, 0.0f);
    pip(1.0f, 1.0f);
  }
  if (topRightBottomLeft) {
    pip(1.0f, 0.0f);
    pip(0.0f, 1.0f);
  }
  if (midLeftRight) {
    pip(0.0f, 0.5f);
    pip(1.0f, 0.5f);
  }
  if (center)
    pip(0.5f, 0.5f);
}

void PlugDiceButton::mouseEnter(const juce::MouseEvent &e) {
  juce::ignoreUnused(e);
  hoverTarget_ = true;
  startTimerHz(60);
}

void PlugDiceButton::mouseExit(const juce::MouseEvent &e) {
  juce::ignoreUnused(e);
  hoverTarget_ = false;
  startTimerHz(60);
}

void PlugDiceButton::clicked() {
  setFaceValue(1 + juce::Random::getSystemRandom().nextInt(6));
  clickAmount_ = 1.0f;
  startTimerHz(60);
}

void PlugDiceButton::setFaceValue(int value) {
  faceValue_ = juce::jlimit(1, 6, value);
  repaint();
}

void PlugDiceButton::timerCallback() {
  const float hoverTarget = hoverTarget_ ? 1.0f : 0.0f;
  hoverAmount_ += (hoverTarget - hoverAmount_) * 0.25f;
  clickAmount_ *= 0.68f;

  if (std::abs(hoverAmount_ - hoverTarget) < 0.01f && clickAmount_ < 0.01f) {
    hoverAmount_ = hoverTarget;
    clickAmount_ = 0.0f;
    stopTimer();
  }

  repaint();
}

} // namespace PlugUI
