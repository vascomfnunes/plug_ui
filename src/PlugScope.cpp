#include "PlugUI/PlugScope.h"
#include "PlugUI/PlugTheme.h"
#include <cmath>

namespace PlugUI {

PlugScope::PlugScope() { history.fill({}); }

void PlugScope::pushColumn(float preMin, float preMax, float postMin,
                           float postMax) {
  auto &col = history[static_cast<size_t>(historyWritePos)];
  col.preMin = preMin;
  col.preMax = preMax;
  col.postMin = postMin;
  col.postMax = postMax;
  col.hasData = true;

  historyWritePos = (historyWritePos + 1) % kHistoryColumns;

  // Decay flash
  clipFlashAlpha = juce::jmax(0.0f, clipFlashAlpha - 0.04f);

  repaint();
}

void PlugScope::setLevels(float thresholdDb, float ceilingDb) {
  if (m_threshDb != thresholdDb || m_ceilDb != ceilingDb) {
    m_threshDb = thresholdDb;
    m_ceilDb = ceilingDb;
    repaint();
  }
}

void PlugScope::triggerClipFlash() {
  clipFlashAlpha = 0.55f;
  repaint();
}

void PlugScope::paint(juce::Graphics &g) {
  const auto bounds = getLocalBounds().toFloat();

  // ── Background ──────────────────────────────────────────────────────────
  g.setColour(juce::Colour(0xff12171c));
  g.fillRoundedRectangle(bounds, 3.0f);

  // Subtle inner bevel
  g.setColour(juce::Colour(0xff1e252c));
  g.drawRoundedRectangle(bounds.reduced(0.5f), 3.0f, 1.0f);

  // ── Derive clip ceiling Y position ──────────────────────────────────────
  const float midY = bounds.getCentreY();
  const float halfH = bounds.getHeight() * 0.5f;

  // ── Grid: single very subtle centre line (0 dB orientation) ─────────────
  g.setColour(juce::Colour(0xff222a32).withAlpha(0.15f));
  g.drawHorizontalLine(static_cast<int>(midY), bounds.getX() + 4.0f,
                       bounds.getRight() - 4.0f);

  auto dbToY = [&](float db) {
    const float lin = juce::jlimit(0.0f, 1.0f, std::pow(10.0f, db * 0.05f));
    return midY - lin * halfH;
  };

  const float threshY = dbToY(m_threshDb);
  const float clipY = dbToY(m_ceilDb);

  // ── Clip-zone band fill ─────────────────────────────────────────────────
  {
    // Positive half
    auto clipZone = juce::Rectangle<float>(bounds.getX(), clipY,
                                           bounds.getWidth(), threshY - clipY);
    g.setColour(juce::Colour(0xffb04030).withAlpha(0.05f));
    g.fillRect(clipZone);

    // Negative half
    const float negThreshY = midY + (midY - threshY);
    const float negClipY = midY + (midY - clipY);
    auto clipZoneNeg = juce::Rectangle<float>(
        bounds.getX(), negThreshY, bounds.getWidth(), negClipY - negThreshY);
    g.setColour(juce::Colour(0xffb04030).withAlpha(0.04f));
    g.fillRect(clipZoneNeg);
  }

  // ── Scrolling history rendering ─────────────────────────────────────────
  const auto waveBounds = bounds.reduced(4.0f, 2.0f);
  const int numCols = kHistoryColumns;
  const float colWidth = waveBounds.getWidth() / static_cast<float>(numCols);
  const float wMidY = waveBounds.getCentreY();
  const float wHalfH = waveBounds.getHeight() * 0.5f;

  auto toY = [&](float s) {
    const float clamped = juce::jlimit(-1.0f, 1.0f, s);
    return wMidY - clamped * wHalfH;
  };

  // 1) Pre-clip ghost outline
  {
    juce::Path preOutline;
    bool started = false;

    for (int i = 0; i < numCols; ++i) {
      const int idx = (historyWritePos + i) % kHistoryColumns;
      const auto &col = history[static_cast<size_t>(idx)];
      if (!col.hasData)
        continue;

      const float x = waveBounds.getX() + static_cast<float>(i) * colWidth;
      const float yTop = toY(col.preMax);

      if (!started) {
        preOutline.startNewSubPath(x, yTop);
        started = true;
      } else {
        preOutline.lineTo(x, yTop);
      }
    }

    for (int i = numCols - 1; i >= 0; --i) {
      const int idx = (historyWritePos + i) % kHistoryColumns;
      const auto &col = history[static_cast<size_t>(idx)];
      if (!col.hasData)
        continue;

      const float x = waveBounds.getX() + static_cast<float>(i) * colWidth;
      const float yBottom = toY(col.preMin);
      preOutline.lineTo(x, yBottom);
    }

    if (!preOutline.isEmpty()) {
      preOutline.closeSubPath();
      g.setColour(juce::Colour(0xff4a6070).withAlpha(0.40f));
      g.strokePath(preOutline,
                   juce::PathStrokeType(1.0f, juce::PathStrokeType::curved,
                                        juce::PathStrokeType::rounded));
    }
  }

  // 2) Post-clip filled body
  {
    juce::Path postFill;
    bool started = false;

    for (int i = 0; i < numCols; ++i) {
      const int idx = (historyWritePos + i) % kHistoryColumns;
      const auto &col = history[static_cast<size_t>(idx)];
      if (!col.hasData)
        continue;

      const float x = waveBounds.getX() + static_cast<float>(i) * colWidth;
      const float yTop = toY(col.postMax);

      if (!started) {
        postFill.startNewSubPath(x, yTop);
        started = true;
      } else {
        postFill.lineTo(x, yTop);
      }
    }

    for (int i = numCols - 1; i >= 0; --i) {
      const int idx = (historyWritePos + i) % kHistoryColumns;
      const auto &col = history[static_cast<size_t>(idx)];
      if (!col.hasData)
        continue;

      const float x = waveBounds.getX() + static_cast<float>(i) * colWidth;
      const float yBottom = toY(col.postMin);
      postFill.lineTo(x, yBottom);
    }

    if (!postFill.isEmpty()) {
      postFill.closeSubPath();
      g.setColour(juce::Colour(0xffd4922a).withAlpha(0.45f));
      g.fillPath(postFill);
      g.setColour(juce::Colour(0xffd4922a).withAlpha(0.85f));
      g.strokePath(postFill,
                   juce::PathStrokeType(1.0f, juce::PathStrokeType::curved,
                                        juce::PathStrokeType::rounded));
    }
  }

  // 3) Clipped-ridge highlight
  {
    juce::Path clippedRidgeTop;
    juce::Path clippedRidgeBot;
    bool inSegTop = false;
    bool inSegBot = false;

    for (int i = 0; i < numCols; ++i) {
      const int idx = (historyWritePos + i) % kHistoryColumns;
      const auto &col = history[static_cast<size_t>(idx)];
      if (!col.hasData) {
        inSegTop = false;
        inSegBot = false;
        continue;
      }

      const float x = waveBounds.getX() + static_cast<float>(i) * colWidth;

      const bool clippedTop = std::abs(col.postMax) > 0.98f;
      if (clippedTop) {
        const float yPreTop = toY(col.preMax);
        if (!inSegTop) {
          clippedRidgeTop.startNewSubPath(x, yPreTop);
          inSegTop = true;
        } else {
          clippedRidgeTop.lineTo(x, yPreTop);
        }
      } else {
        inSegTop = false;
      }

      const bool clippedBot = std::abs(col.postMin) > 0.98f;
      if (clippedBot) {
        const float yPreBot = toY(col.preMin);
        if (!inSegBot) {
          clippedRidgeBot.startNewSubPath(x, yPreBot);
          inSegBot = true;
        } else {
          clippedRidgeBot.lineTo(x, yPreBot);
        }
      } else {
        inSegBot = false;
      }
    }

    const auto ridgeColour = juce::Colour(0xffffc040);
    const auto ridgeStroke = juce::PathStrokeType(
        1.8f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded);

    if (!clippedRidgeTop.isEmpty()) {
      g.setColour(ridgeColour);
      g.strokePath(clippedRidgeTop, ridgeStroke);
    }
    if (!clippedRidgeBot.isEmpty()) {
      g.setColour(ridgeColour);
      g.strokePath(clippedRidgeBot, ridgeStroke);
    }
  }

  // ── Single clip ceiling line ────────────────────────────────────────────
  {
    const auto clipLineColour = juce::Colour(0xffcc5533);
    g.setColour(clipLineColour.withAlpha(1.0f));
    g.drawLine(waveBounds.getX(), clipY, waveBounds.getRight(), clipY, 1.5f);

    const float negClipY = midY + (midY - clipY);
    g.setColour(clipLineColour.withAlpha(0.15f));
    g.drawLine(waveBounds.getX(), negClipY, waveBounds.getRight(), negClipY,
               0.5f);
  }

  // ── Label: clip ceiling dB value ────────────────────────────────────────
  {
    auto labelFont = juce::Font(
        juce::FontOptions(Theme::fontFamily, 8.0f, juce::Font::plain));
    labelFont.setExtraKerningFactor(0.06f);
    g.setFont(labelFont);

    const juce::String labelStr = juce::String(m_ceilDb, 1) + " dB";
    juce::GlyphArrangement glyphs;
    glyphs.addLineOfText(labelFont, labelStr, 0.0f, 0.0f);
    const float labelW = glyphs.getBoundingBox(0, -1, false).getWidth() + 14.0f;
    const float labelH = 14.0f;
    const float labelX = waveBounds.getRight() - labelW - 8.0f;
    const float labelY = clipY - labelH - 3.0f;

    auto labelRect =
        juce::Rectangle<float>(labelX, labelY, labelW, labelH).expanded(1.0f);
    g.setColour(juce::Colour(0xff12171c).withAlpha(0.75f));
    g.fillRoundedRectangle(labelRect, 2.0f);
    g.setColour(juce::Colour(0xffcc5533).withAlpha(0.30f));
    g.drawRoundedRectangle(labelRect, 2.0f, 0.5f);

    g.setColour(juce::Colour(0xffcc5533).withAlpha(0.90f));
    g.drawText(labelStr, static_cast<int>(labelX), static_cast<int>(labelY),
               static_cast<int>(labelW), static_cast<int>(labelH),
               juce::Justification::centred, false);
  }

  // ── Clip flash overlay ──────────────────────────────────────────────────
  if (clipFlashAlpha > 0.001f) {
    g.setColour(juce::Colour(0xffff2200).withAlpha(clipFlashAlpha * 0.14f));
    g.fillRoundedRectangle(bounds, 3.0f);
    g.setColour(juce::Colour(0xffff3300).withAlpha(clipFlashAlpha * 0.45f));
    g.drawRoundedRectangle(bounds.reduced(0.5f), 3.0f, 1.5f);
  }

  // ── Outer border ────────────────────────────────────────────────────────
  g.setColour(juce::Colour(0xff000000).withAlpha(0.30f));
  g.drawRoundedRectangle(bounds.reduced(0.5f), 3.0f, 1.0f);
}

} // namespace PlugUI
