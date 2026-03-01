#include "PlugUI/PlugMeter.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

PlugMeter::PlugMeter(float maxDb) : m_maxGrDb(maxDb) {}

void PlugMeter::setGainReductionDb(float grDb) noexcept {
  m_grDb = grDb;
  repaint();
}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void PlugMeter::paint(juce::Graphics &g) {
  using namespace Theme;

  const auto inner = getLocalBounds().toFloat().reduced(1.0f);

  const float labelW = 24.0f;
  const float fontScale = getProperties().getWithDefault("valFontScale", 1.0f);
  const float dbTextW = 56.0f * fontScale;
  const float barMaxW = inner.getWidth() - labelW - dbTextW;
  const float barRight = inner.getRight() - dbTextW;
  const float barLeft = barRight - barMaxW;
  const float barY = inner.getY() + 5.0f;
  const float barH = inner.getHeight() - 18.0f;

  auto meterFont =
      juce::Font(juce::FontOptions(fontFamily, 11.5f, juce::Font::bold));
  meterFont.setExtraKerningFactor(0.06f);
  g.setFont(meterFont);

  const bool fullyJustify =
      getProperties().getWithDefault("fullyJustified", false);

  g.setColour(textMuted.withAlpha(0.85f));
  g.drawText("GR", inner.withY(barY).withHeight(barH).withWidth(labelW),
             fullyJustify ? juce::Justification::centredLeft
                          : juce::Justification::centred);

  constexpr int segments = 64;
  const float segW = barMaxW / static_cast<float>(segments);
  const float segGap = 1.0f; // gap between segments
  const float thicknessOffset =
      getProperties().getWithDefault("thicknessOffset", 0.0f);
  const float segInset = juce::jmax(0.0f, 1.5f - thicknessOffset);

  const float norm = juce::jlimit(0.0f, 1.0f, m_grDb / m_maxGrDb);
  const float fillW = barMaxW * norm;

  // Draw segments.
  for (int i = 0; i < segments; ++i) {
    const float segX = barLeft + segW * static_cast<float>(i) + segGap * 0.5f;
    const float segDrawW = segW - segGap;
    const float segCenterX = segX + segDrawW * 0.5f;

    if (norm > 0.001f && segCenterX < barLeft + fillW) {
      // Filled segment: interpolate colour across the bar.
      const float t = static_cast<float>(i) / static_cast<float>(segments - 1);
      const float fadeStart = juce::jmax(0.0f, norm - 0.15f);
      // Fade the brightness towards the edge of the fill.
      float alpha = 1.0f;
      if (norm < 1.0f) {
        const float segNorm =
            static_cast<float>(i) / static_cast<float>(segments);
        if (segNorm > fadeStart) {
          alpha = juce::jlimit(
              0.0f, 1.0f, 1.0f - (segNorm - fadeStart) / (norm - fadeStart));
          alpha = alpha * alpha; // ease-out curve
        }
      }
      const float customContrast =
          getProperties().getWithDefault("barContrast", 1.0f);
      const auto segColour =
          accentBright.brighter(0.12f * customContrast)
              .interpolatedWith(accentDim.brighter(0.12f * customContrast), t)
              .withMultipliedAlpha(alpha);

      if (alpha > 0.1f) {
        g.setColour(segColour.withMultipliedAlpha(0.2f * alpha));
        g.fillRect(segX - 1.0f, barY + segInset - 1.0f, segDrawW + 2.0f,
                   barH - segInset * 2.0f + 2.0f);
      }

      g.setColour(segColour);
      g.fillRect(segX, barY + segInset, segDrawW, barH - segInset * 2.0f);
    } else {
      // Unfilled segment: very subtle divider.
      g.setColour(textMuted.withAlpha(0.05f));
      g.fillRect(segX, barY + segInset, segDrawW, barH - segInset * 2.0f);
    }
  }

  // Bottom scale labels.
  auto scaleFont =
      juce::Font(juce::FontOptions(fontFamily, 8.0f, juce::Font::plain));
  scaleFont.setExtraKerningFactor(0.04f);
  g.setFont(scaleFont);
  g.setColour(textMuted.withAlpha(0.77f));
  struct Tick {
    float db;
    const char *text;
  };
  for (const Tick tick : {Tick{0.0f, "0"}, Tick{5.0f, "-5"}, Tick{10.0f, "-10"},
                          Tick{15.0f, "-15"}}) {
    const float x =
        barLeft + barMaxW * juce::jlimit(0.0f, 1.0f, tick.db / m_maxGrDb);
    g.drawText(
        tick.text,
        juce::Rectangle<float>(x - 20.0f, barY + barH + 2.0f, 40.0f, 10.0f),
        juce::Justification::centred, false);
  }

  auto valFont = juce::Font(
      juce::FontOptions(fontFamily, 16.5f * fontScale, juce::Font::bold));
  valFont.setExtraKerningFactor(0.02f);
  g.setFont(valFont);

  // moderately increase contrast for clarity, but reduce opacity for balance
  g.setColour(norm > 0.01f ? textValue.brighter(0.15f).withAlpha(0.95f)
                           : textMuted.brighter(0.5f).withAlpha(0.85f));
  const juce::String dbText =
      (m_grDb > 0.05f) ? "-" + juce::String(m_grDb, 1) + " dB" : "0.0 dB";

  if (fullyJustify) {
    g.drawText(
        dbText,
        inner.withLeft(barRight).withWidth(dbTextW).withY(barY).withHeight(
            barH),
        juce::Justification::centredRight);
  } else {
    g.drawText(dbText,
               inner.withLeft(barRight + 8.0f).withY(barY).withHeight(barH),
               juce::Justification::centredLeft);
  }
}

} // namespace PlugUI
