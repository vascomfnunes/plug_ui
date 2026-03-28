#include "PlugUI/PlugVuMeter.h"
#include "PlugUI/PlugTheme.h"
#include <array>
#include <cmath>

namespace PlugUI {

void PlugVuMeter::setLevel(float levelDb) noexcept {
    m_currentDb = levelDb;
    repaint();
}

void PlugVuMeter::setLit(bool isLit) noexcept {
    m_isLit = isLit;
    repaint();
}

void PlugVuMeter::setLabel(const juce::String& text) noexcept {
    m_label = text;
    repaint();
}

void PlugVuMeter::paint(juce::Graphics& g) {
    using namespace Theme;

    const auto bounds  = getLocalBounds().toFloat();
    const float corner = 4.0f;
    auto vuToAngle     = [](float vu) {
        const float normalized = (vu - minVu) / (maxVu - minVu);
        return startAngle + (endAngle - startAngle) * normalized;
    };

    const auto bezelOuter = juce::Colour(0xff1a1b1d);
    const auto bezelInner = juce::Colour(0xff2a2c2f);
    juce::ColourGradient bezelGrad(bezelInner, bounds.getTopLeft(), bezelOuter,
                                   bounds.getBottomRight(), false);
    g.setGradientFill(bezelGrad);
    g.fillRoundedRectangle(bounds, corner);

    const auto frameBounds = bounds.reduced(2.0f);
    g.setColour(juce::Colour(0xff000000).withAlpha(0.45f));
    g.drawRoundedRectangle(frameBounds, corner - 0.5f, 1.0f);

    const auto faceBounds  = frameBounds.reduced(3.0f);
    const float faceCorner = corner - 1.2f;
    const auto paperLit    = juce::Colour(0xffeee2bc);
    // Unlit analog paper should stay warm/desaturated, not blue-grey.
    const auto paperUnlit = juce::Colour(0xffb4ad97);
    const auto paperHi    = (m_isLit ? paperLit : paperUnlit).brighter(0.14f);
    const auto paperLo    = (m_isLit ? paperLit : paperUnlit).darker(m_isLit ? 0.11f : 0.24f);

    juce::ColourGradient paperGrad(
        paperHi, juce::Point<float>(faceBounds.getCentreX(), faceBounds.getY()), paperLo,
        juce::Point<float>(faceBounds.getCentreX(), faceBounds.getBottom()), false);
    g.setGradientFill(paperGrad);
    g.fillRoundedRectangle(faceBounds, faceCorner);

    if (m_isLit) {
        juce::ColourGradient faceLift(
            juce::Colour(0xfffff7d1).withAlpha(0.24f),
            juce::Point<float>(faceBounds.getX() + 18.0f, faceBounds.getY() + 10.0f),
            juce::Colours::transparentWhite,
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getCentreY() + 10.0f), false);
        g.setGradientFill(faceLift);
        g.fillRoundedRectangle(faceBounds.reduced(1.0f), faceCorner - 0.4f);

        juce::ColourGradient lampWarm(
            juce::Colour(0xffffdc8a).withAlpha(0.24f),
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getY() + 8.0f),
            juce::Colours::transparentWhite,
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getBottom() - 12.0f), false);
        lampWarm.addColour(0.35, juce::Colour(0xffffefbf).withAlpha(0.16f));
        lampWarm.addColour(0.62, juce::Colour(0xfffff2cc).withAlpha(0.08f));
        g.setGradientFill(lampWarm);
        g.fillRoundedRectangle(faceBounds.reduced(1.0f), faceCorner - 0.4f);

        juce::ColourGradient hotspot(
            juce::Colour(0xfffff6d8).withAlpha(0.35f),
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getY() + 2.0f),
            juce::Colours::transparentWhite,
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getY() + 62.0f), true);
        g.setGradientFill(hotspot);
        g.fillRoundedRectangle(faceBounds.reduced(1.0f, 0.5f), faceCorner - 0.4f);

        juce::ColourGradient edgeVignette(
            juce::Colours::transparentBlack,
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getCentreY()),
            juce::Colour(0xff000000).withAlpha(0.22f),
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getBottom()), false);
        g.setGradientFill(edgeVignette);
        g.fillRoundedRectangle(faceBounds.reduced(0.5f), faceCorner - 0.3f);
    }
    if (!m_isLit) {
        // Unpowered glass: dims the face without killing readability.
        juce::ColourGradient offMask(
            juce::Colour(0xff000000).withAlpha(0.10f),
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getY()),
            juce::Colour(0xff000000).withAlpha(0.22f),
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getBottom()), false);
        g.setGradientFill(offMask);
        g.fillRoundedRectangle(faceBounds.reduced(0.6f), faceCorner - 0.2f);

        juce::ColourGradient offVignette(
            juce::Colours::transparentBlack,
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getCentreY() - 8.0f),
            juce::Colour(0xff000000).withAlpha(0.10f),
            juce::Point<float>(faceBounds.getCentreX(), faceBounds.getBottom()), false);
        g.setGradientFill(offVignette);
        g.fillRoundedRectangle(faceBounds.reduced(0.8f), faceCorner - 0.3f);
    }

    g.setColour(juce::Colour(0xff000000).withAlpha(0.24f));
    g.drawRoundedRectangle(faceBounds, faceCorner, 1.0f);

    const auto centre = juce::Point<float>(std::floor(faceBounds.getCentreX()) + 0.5f,
                                           std::floor(faceBounds.getBottom() - 11.0f) + 0.5f);

    const float radius =
        juce::jmin(faceBounds.getWidth() * 0.47f, faceBounds.getHeight() * 0.90f) - 4.0f;

    const std::array<float, 11> majorTicks = {-20.0f, -10.0f, -7.0f, -5.0f, -3.0f, -2.0f,
                                              -1.0f,  0.0f,   1.0f,  2.0f,  3.0f};

    const std::array<float, 7> labelTicks = {-20.0f, -10.0f, -7.0f, -5.0f, 0.0f, 2.0f, 3.0f};

    g.setFont(juce::Font(juce::FontOptions(fontFamily, 7.2f, juce::Font::bold)));

    for (float vu = minVu; vu <= maxVu + 0.5f; vu += 1.0f) {
        bool isMajor = false;
        for (float t : majorTicks) {
            if (std::abs(vu - t) < 0.1f) {
                isMajor = true;
                break;
            }
        }

        const float angle      = vuToAngle(vu);
        const float tickOuterR = radius;
        const float tickInnerR = radius - (isMajor ? 9.5f : 5.2f);

        const auto p1 = juce::Point<float>(centre.x + std::cos(angle) * tickInnerR,
                                           centre.y + std::sin(angle) * tickInnerR);
        const auto p2 = juce::Point<float>(centre.x + std::cos(angle) * tickOuterR,
                                           centre.y + std::sin(angle) * tickOuterR);

        juce::Colour tickCol =
            (vu > 0.1f) ? juce::Colour(0xffb53a32) : juce::Colour(0xff121212).withAlpha(0.92f);
        if (!m_isLit)
            tickCol = tickCol.withSaturation(0.10f).withAlpha(0.46f);

        g.setColour(tickCol);
        g.drawLine({p1, p2}, isMajor ? 1.7f : 0.9f);

        if (!isMajor)
            continue;

        bool showLabel = false;
        for (float l : labelTicks) {
            if (std::abs(vu - l) < 0.1f) {
                showLabel = true;
                break;
            }
        }
        if (!showLabel)
            continue;

        const int vuInt = static_cast<int>(vu);
        const juce::String labelText =
            (vuInt < 0) ? juce::String(std::abs(vuInt)) : juce::String(vuInt);

        float labelR        = tickInnerR - 5.8f;
        float tangentOffset = 0.0f;
        float radialOffset  = 0.0f;
        if (vu < -19.9f) {
            labelR -= 1.5f;
            tangentOffset -= 2.0f;
            radialOffset += 1.0f;
        } else if (vu < -9.9f && vu > -10.1f) {
            tangentOffset -= 1.2f;
            radialOffset -= 1.4f;
        } else if (vu < -6.9f && vu > -7.1f) {
            tangentOffset += 0.2f;
            radialOffset -= 1.6f;
        } else if (vu < -4.9f && vu > -5.1f) {
            tangentOffset += 1.2f;
            radialOffset -= 1.4f;
        } else if (vu > -0.1f && vu < 0.1f) {
            tangentOffset += 2.0f;
            radialOffset += 0.8f;
        } else if (vu > 1.9f && vu < 2.1f) {
            tangentOffset += 1.2f;
            radialOffset += 2.4f;
        } else if (vu > 2.9f) {
            tangentOffset += 1.8f;
            radialOffset += 3.8f;
        }
        labelR += radialOffset;
        const auto radialX  = std::cos(angle);
        const auto radialY  = std::sin(angle);
        const auto tangentX = -radialY;
        const auto tangentY = radialX;
        const auto labelPos =
            juce::Point<float>(centre.x + radialX * labelR + tangentX * tangentOffset,
                               centre.y + radialY * labelR + tangentY * tangentOffset);

        juce::Colour textCol =
            (vu > 0.1f) ? juce::Colour(0xffa4322c) : juce::Colour(0xff121212).withAlpha(0.95f);
        if (!m_isLit)
            textCol = textCol.withSaturation(0.10f).withAlpha(0.46f);

        g.setColour(textCol);
        g.drawText(labelText, static_cast<int>(labelPos.x - 8), static_cast<int>(labelPos.y - 4),
                   16, 9, juce::Justification::centred);
    }

    g.setFont(juce::Font(juce::FontOptions(fontFamily, 12.0f, juce::Font::bold)));
    g.setColour(m_isLit ? juce::Colour(0xff191919) : juce::Colour(0xff36332e));
    g.drawText("VU", static_cast<int>(faceBounds.getX()), static_cast<int>(centre.y - 38.0f),
               static_cast<int>(faceBounds.getWidth()), 14, juce::Justification::centred);

    g.setFont(juce::Font(juce::FontOptions(fontFamily, 9.0f, juce::Font::bold)));
    g.setColour(m_isLit ? juce::Colour(0xff2c2c2c).withAlpha(0.75f)
                        : juce::Colour(0xff4a463f).withAlpha(0.78f));
    g.drawText(m_label, static_cast<int>(faceBounds.getX()), static_cast<int>(centre.y - 24.0f),
               static_cast<int>(faceBounds.getWidth()), 12, juce::Justification::centred);

    const float clampedVu   = juce::jlimit(minVu, maxVu, m_currentDb);
    const float needleAngle = vuToAngle(clampedVu);
    const float needleLen   = radius + 2.0f;

    if (m_isLit) {
        juce::Path needleShadow;
        needleShadow.startNewSubPath(centre.x + 1.0f, centre.y + 1.0f);
        needleShadow.lineTo(centre.x + std::cos(needleAngle) * needleLen + 1.0f,
                            centre.y + std::sin(needleAngle) * needleLen + 1.0f);
        g.setColour(juce::Colour(0xff000000).withAlpha(0.22f));
        g.strokePath(needleShadow, juce::PathStrokeType(1.0f));
    }

    juce::Path needle;
    needle.startNewSubPath(centre);
    needle.lineTo(centre.x + std::cos(needleAngle) * needleLen,
                  centre.y + std::sin(needleAngle) * needleLen);

    g.setColour(m_isLit ? juce::Colour(0xff141414) : juce::Colour(0xff2a2925));
    juce::PathStrokeType needleStroke(1.2f);
    needleStroke.setJointStyle(juce::PathStrokeType::curved);
    needleStroke.setEndStyle(juce::PathStrokeType::rounded);
    g.strokePath(needle, needleStroke);

    g.setColour(m_isLit ? juce::Colour(0xff111111) : juce::Colour(0xff23211d));
    g.fillEllipse(centre.x - 4.2f, centre.y - 4.2f, 8.4f, 8.4f);
    g.setColour(m_isLit ? juce::Colour(0xff3a3a3a) : juce::Colour(0xff3b3831));
    g.fillEllipse(centre.x - 1.4f, centre.y - 1.4f, 2.8f, 2.8f);
}

} // namespace PlugUI
