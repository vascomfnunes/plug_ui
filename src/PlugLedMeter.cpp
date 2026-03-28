// PlugLedMeter.cpp
// Thin vertical LED column — 10 amber-to-red segments, -30 to 0 dBFS.
// Designed to flank the PlugGrMeter, sharing the same amber palette.

#include "PlugUI/PlugLedMeter.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

void PlugLedMeter::setLevelDb(float levelDb) noexcept {
    m_levelDb = levelDb;
    repaint();
}

void PlugLedMeter::paint(juce::Graphics& g) {
    const auto bounds = getLocalBounds().toFloat();

    // ── Background ──────────────────────────────────────────────────────────
    // Matches PlugGrMeter dark glass face
    const auto bgBase = juce::Colour(0xFF080A0E);
    const auto bgHi   = bgBase.brighter(0.20f);
    juce::ColourGradient bgGrad(bgHi, juce::Point<float>(bounds.getCentreX(), bounds.getY()), bgBase,
                                juce::Point<float>(bounds.getCentreX(), bounds.getBottom()), false);
    g.setGradientFill(bgGrad);
    g.fillRoundedRectangle(bounds, 2.0f);

    // Subtle inner border
    g.setColour(juce::Colour(0xFF000000).withAlpha(0.45f));
    g.drawRoundedRectangle(bounds.reduced(0.5f), 2.0f, 1.0f);

    // ── LED segments ─────────────────────────────────────────────────────────
    // Segment colours (bottom to top): amber dim → amber mid → amber bright → orange → red
    const juce::Colour cols[N_SEGS] = {
        juce::Colour(0xFF6B4A12), // 0  – dim amber  (-30 to -27)
        juce::Colour(0xFF7A5616), // 1  – dim amber  (-27 to -24)
        juce::Colour(0xFF8A6420), // 2  – dim amber  (-24 to -21)
        juce::Colour(0xFF9A7228), // 3  – amber      (-21 to -18)
        juce::Colour(0xFFAA7C2C), // 4  – amber      (-18 to -15)
        juce::Colour(0xFFBB8830), // 5  – amber      (-15 to -12)
        juce::Colour(0xFFC09030), // 6  – amber mid  (-12 to  -9)
        juce::Colour(0xFFD4982E), // 7  – amber brt  ( -9 to  -6)
        juce::Colour(0xFFCC6020), // 8  – orange     ( -6 to  -3)
        juce::Colour(0xFFBB2820), // 9  – red        ( -3 to   0)
    };

    constexpr float gap = 1.5f;
    const float segH    = (bounds.getHeight() - gap * (N_SEGS - 1)) / static_cast<float>(N_SEGS);
    const float segW    = bounds.getWidth() - 2.0f;
    const float segX    = bounds.getX() + 1.0f;

    for (int i = 0; i < N_SEGS; ++i) {
        // i = 0 is bottom, N_SEGS-1 is top
        // Threshold for segment i: the level that lights this segment
        const float threshold =
            MIN_DB + static_cast<float>(i) * (MAX_DB - MIN_DB) / static_cast<float>(N_SEGS);
        const bool isLit = m_levelDb >= threshold;

        // Y position: segment 0 at bottom, N_SEGS-1 at top
        const float segY = bounds.getBottom() - static_cast<float>(i + 1) * (segH + gap) + gap;

        const juce::Colour lit = cols[i];
        const juce::Colour dim = lit.withAlpha(0.09f);
        g.setColour(isLit ? lit : dim);
        g.fillRoundedRectangle(segX, segY, segW, segH, 0.8f);
    }
}

} // namespace PlugUI
