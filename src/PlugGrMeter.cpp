// PlugGrMeter.cpp
// Analog-style compressor GR meter — dark glass face, amber needle & scale.
//
// Scale: 0 dB GR (no compression) rests at the RIGHT. Needle deflects LEFT
// as gain reduction increases (SSL Bus / LA-2A convention).
// Major ticks: 0, 1, 2, 3, 4, 5, 6 dB — minor at every 0.5 dB.
//
// All pixel constants are multiplied by `s` (= bounds.height / 108) so the
// meter draws correctly at any window size.

#include "PlugUI/PlugGrMeter.h"
#include "PlugUI/PlugTheme.h"
#include <array>
#include <cmath>

namespace PlugUI {

void PlugGrMeter::setGainReductionDb(float grDb) noexcept {
    m_grDb = grDb;
    repaint();
}

void PlugGrMeter::paint(juce::Graphics& g) {
    using namespace Theme;

    const auto bounds = getLocalBounds().toFloat();
    // Scale factor: 108 px is the reference component height (meterH at uiScale=1).
    // Every hard-coded pixel value is multiplied by s so the meter looks identical
    // regardless of window size.
    const float s      = bounds.getHeight() / 108.0f;
    const float corner = 4.0f * s;

    // 0 GR → endAngle (right, needle at rest)
    // maxGr → startAngle (left, maximum compression)
    auto grToAngle = [](float gr) {
        const float norm = juce::jlimit(0.0f, 1.0f, gr / maxGr);
        return endAngle - norm * (endAngle - startAngle);
    };

    // Amber palette for this meter
    const auto amberBright = juce::Colour(0xFFD4982E);
    const auto amberMid    = juce::Colour(0xFFAA7820);
    const auto amberDim    = juce::Colour(0xFF7A5616);

    // ── Bezel ─────────────────────────────────────────────────────────────────
    const auto bezelOuter = juce::Colour(0xff1a1b1d);
    const auto bezelInner = juce::Colour(0xff2a2c2f);
    juce::ColourGradient bezelGrad(bezelInner, bounds.getTopLeft(), bezelOuter,
                                   bounds.getBottomRight(), false);
    g.setGradientFill(bezelGrad);
    g.fillRoundedRectangle(bounds, corner);

    const auto frameBounds = bounds.reduced(2.0f * s);
    g.setColour(juce::Colour(0xff000000).withAlpha(0.50f));
    g.drawRoundedRectangle(frameBounds, corner - 0.5f * s, 1.0f);

    // ── Dark glass face ───────────────────────────────────────────────────────
    const auto faceBounds  = frameBounds.reduced(3.0f * s);
    const float faceCorner = corner * 0.70f; // proportional to corner

    // Base fill: very dark, very slightly warm tint
    const auto faceBase = juce::Colour(0xFF0C0E12);
    const auto faceHi   = faceBase.brighter(0.30f);
    juce::ColourGradient faceGrad(
        faceHi, juce::Point<float>(faceBounds.getCentreX(), faceBounds.getY()), faceBase,
        juce::Point<float>(faceBounds.getCentreX(), faceBounds.getBottom()), false);
    g.setGradientFill(faceGrad);
    g.fillRoundedRectangle(faceBounds, faceCorner);

    // Top-edge glass glare
    {
        juce::ColourGradient glare(
            juce::Colours::white.withAlpha(0.07f),
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getY() + 1.0f),
            juce::Colours::transparentWhite,
            juce::Point<float>(faceBounds.getCentreX(),
                               faceBounds.getY() + faceBounds.getHeight() * 0.28f),
            false);
        g.setGradientFill(glare);
        g.fillRoundedRectangle(faceBounds.reduced(1.0f * s), faceCorner * 0.85f);
    }

    // Bottom vignette — darkens lower portion where pivot lives
    {
        juce::ColourGradient vignette(
            juce::Colours::transparentBlack,
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getCentreY()),
            juce::Colour(0xff000000).withAlpha(0.38f),
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getBottom()), false);
        g.setGradientFill(vignette);
        g.fillRoundedRectangle(faceBounds.reduced(0.5f * s), faceCorner * 0.90f);
    }

    // Ambient amber glow washing up from the needle region
    {
        juce::ColourGradient needleGlow(
            amberDim.withAlpha(0.12f),
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getBottom() - 12.0f * s),
            juce::Colours::transparentBlack,
            juce::Point<float>(faceBounds.getCentreX(),
                               faceBounds.getBottom() - faceBounds.getHeight() * 0.55f),
            false);
        g.setGradientFill(needleGlow);
        g.fillRoundedRectangle(faceBounds.reduced(0.8f * s), faceCorner * 0.95f);
    }

    g.setColour(juce::Colour(0xff000000).withAlpha(0.30f));
    g.drawRoundedRectangle(faceBounds, faceCorner, 1.0f);

    // ── Geometry ──────────────────────────────────────────────────────────────
    const auto centre = juce::Point<float>(std::floor(faceBounds.getCentreX()) + 0.5f,
                                           std::floor(faceBounds.getBottom() - 11.0f * s) + 0.5f);
    const float radius =
        juce::jmin(faceBounds.getWidth() * 0.47f, faceBounds.getHeight() * 0.90f) - 4.0f * s;

    // ── Ticks ─────────────────────────────────────────────────────────────────
    // Major ticks at every integer dB (0–6); minor at every 0.5 dB.
    // Labels shown at 0, 2, 4, 6 to avoid crowding.
    const std::array<float, 7> majorTicks   = {0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
    const std::array<float, 4> labeledTicks = {0.0f, 2.0f, 4.0f, 6.0f};

    g.setFont(juce::Font(juce::FontOptions(fontFamily, 8.8f * s, juce::Font::bold)));

    for (float gr = 0.0f; gr <= maxGr + 0.25f; gr += 0.5f) {
        bool isMajor = false;
        for (float t : majorTicks)
            if (std::abs(gr - t) < 0.1f) {
                isMajor = true;
                break;
            }

        const float angle      = grToAngle(gr);
        const float tickOuterR = radius;
        const float tickInnerR = radius - (isMajor ? 9.5f * s : 5.2f * s);

        const auto p1 = juce::Point<float>(centre.x + std::cos(angle) * tickInnerR,
                                           centre.y + std::sin(angle) * tickInnerR);
        const auto p2 = juce::Point<float>(centre.x + std::cos(angle) * tickOuterR,
                                           centre.y + std::sin(angle) * tickOuterR);

        const auto tickCol = isMajor ? amberMid.withAlpha(0.90f) : amberDim.withAlpha(0.65f);
        g.setColour(tickCol);
        g.drawLine({p1, p2}, isMajor ? 1.6f * s : 0.9f * s);

        if (!isMajor)
            continue;

        bool showLabel = false;
        for (float l : labeledTicks)
            if (std::abs(gr - l) < 0.1f) {
                showLabel = true;
                break;
            }
        if (!showLabel)
            continue;

        const juce::String labelText =
            (gr < 0.1f) ? juce::String("0") : ("-" + juce::String(static_cast<int>(gr)));

        float labelR        = tickInnerR - 5.8f * s;
        float tangentOffset = 0.0f;
        float radialOffset  = 0.0f;

        if (gr < 0.1f) {
            tangentOffset += 2.0f * s;
            radialOffset += 1.0f * s;
        } else if (gr > 1.9f && gr < 2.1f) {
            tangentOffset += 0.8f * s;
            radialOffset -= 1.4f * s;
        } else if (gr > 3.9f && gr < 4.1f) {
            tangentOffset -= 0.2f * s;
            radialOffset -= 1.6f * s;
        } else if (gr > 5.9f) {
            tangentOffset -= 2.0f * s;
            radialOffset += 1.0f * s;
        }

        labelR += radialOffset;
        const float radialX  = std::cos(angle);
        const float radialY  = std::sin(angle);
        const float tangentX = -radialY;
        const float tangentY = radialX;
        const auto labelPos =
            juce::Point<float>(centre.x + radialX * labelR + tangentX * tangentOffset,
                               centre.y + radialY * labelR + tangentY * tangentOffset);

        const int lW = static_cast<int>(24.0f * s);
        const int lH = static_cast<int>(11.0f * s);
        g.setColour(amberMid.withAlpha(0.85f));
        g.drawText(labelText, static_cast<int>(labelPos.x - 12.0f * s),
                   static_cast<int>(labelPos.y - 5.5f * s), lW, lH, juce::Justification::centred);
    }

    // ── Digital GR readout ────────────────────────────────────────────────────
    {
        const juce::String grText =
            (m_grDb > 0.05f) ? "-" + juce::String(m_grDb, 1) + " dB" : "0.0 dB";

        // Main value
        g.setFont(juce::Font(juce::FontOptions(fontFamily, 15.0f * s, juce::Font::bold)));
        g.setColour(amberBright.withAlpha(0.95f));
        g.drawText(grText, static_cast<int>(faceBounds.getX()),
                   static_cast<int>(centre.y - 31.0f * s), static_cast<int>(faceBounds.getWidth()),
                   static_cast<int>(16.0f * s), juce::Justification::centred);

        // "GR" sub-label
        g.setFont(juce::Font(juce::FontOptions(fontFamily, 9.0f * s, juce::Font::bold)));
        g.setColour(amberDim.withAlpha(0.80f));
        g.drawText("GR", static_cast<int>(faceBounds.getX()),
                   static_cast<int>(centre.y - 16.0f * s), static_cast<int>(faceBounds.getWidth()),
                   static_cast<int>(10.0f * s), juce::Justification::centred);
    }

    // ── Needle ────────────────────────────────────────────────────────────────
    const float clampedGr   = juce::jlimit(0.0f, maxGr, m_grDb);
    const float needleAngle = grToAngle(clampedGr);
    const float needleLen   = radius + 2.0f * s;

    // Bloom / glow layers behind the needle
    if (clampedGr > 0.1f) {
        for (int layer = 3; layer >= 1; --layer) {
            juce::Path glow;
            glow.startNewSubPath(centre.x, centre.y);
            glow.lineTo(centre.x + std::cos(needleAngle) * needleLen,
                        centre.y + std::sin(needleAngle) * needleLen);
            const float alpha = 0.06f / static_cast<float>(layer);
            g.setColour(amberBright.withAlpha(alpha));
            g.strokePath(glow, juce::PathStrokeType(static_cast<float>(layer) * 3.0f * s,
                                                    juce::PathStrokeType::mitered,
                                                    juce::PathStrokeType::rounded));
        }
    }

    // Shadow
    {
        juce::Path shadow;
        shadow.startNewSubPath(centre.x + 1.0f * s, centre.y + 1.0f * s);
        shadow.lineTo(centre.x + std::cos(needleAngle) * needleLen + 1.0f * s,
                      centre.y + std::sin(needleAngle) * needleLen + 1.0f * s);
        g.setColour(juce::Colour(0xff000000).withAlpha(0.35f));
        g.strokePath(shadow, juce::PathStrokeType(1.2f * s));
    }

    // Needle body — amber
    juce::Path needle;
    needle.startNewSubPath(centre);
    needle.lineTo(centre.x + std::cos(needleAngle) * needleLen,
                  centre.y + std::sin(needleAngle) * needleLen);
    g.setColour(amberBright.withAlpha(0.96f));
    juce::PathStrokeType needleStroke(1.3f * s);
    needleStroke.setJointStyle(juce::PathStrokeType::curved);
    needleStroke.setEndStyle(juce::PathStrokeType::rounded);
    g.strokePath(needle, needleStroke);

    // Pivot dot
    g.setColour(juce::Colour(0xFF1A1610));
    g.fillEllipse(centre.x - 4.2f * s, centre.y - 4.2f * s, 8.4f * s, 8.4f * s);
    g.setColour(amberDim.withAlpha(0.80f));
    g.fillEllipse(centre.x - 1.4f * s, centre.y - 1.4f * s, 2.8f * s, 2.8f * s);
}

} // namespace PlugUI
