#include "PlugUI/BackgroundRenderer.h"
#include "BinaryData.h"
#include "PlugUI/PlugTheme.h"

namespace PlugUI {

namespace {

juce::Image getMicroNoiseImage() {
    static juce::Image noiseImg;
    if (noiseImg.isNull()) {
        constexpr int size = 256;
        noiseImg           = juce::Image(juce::Image::ARGB, size, size, true);
        juce::Image::BitmapData data(noiseImg, juce::Image::BitmapData::writeOnly);
        juce::Random rng(12345); // deterministic seed for stability over process
                                 // lifetime (no random-per-repaint)

        for (int y = 0; y < size; ++y) {
            uint8_t* row = data.getLinePointer(y);
            for (int x = 0; x < size; ++x) {
                // Generate a very constrained noise value to avoid high contrast static
                // Instead of 0-255, we constrain to a narrow band around middle
                // brightness to act as a micro-variation rather than extreme
                // black/white.
                uint16_t val = static_cast<uint16_t>(116 + rng.nextInt(26));

                // Extremely low opacity for the noise
                // Barely visible (approx 1% to 3.5% opacity)
                uint16_t alpha = static_cast<uint16_t>(3 + rng.nextInt(5));

                // JUCE Image::ARGB format uses PRE-MULTIPLIED alpha.
                // We MUST multiply the RGB values by the alpha ratio, or else
                // macOS CoreGraphics will have extreme rendering artifacts/glitches.
                uint8_t premultipliedVal = static_cast<uint8_t>((val * alpha) / 255);
                uint8_t aVal             = static_cast<uint8_t>(alpha);

                row[x * 4 + 0] = premultipliedVal; // B
                row[x * 4 + 1] = premultipliedVal; // G
                row[x * 4 + 2] = premultipliedVal; // R
                row[x * 4 + 3] = aVal;             // A
            }
        }
    }
    return noiseImg;
}

const juce::Image& getDefaultTextureImage() {
    static const juce::Image texture = juce::ImageCache::getFromMemory(
        PlugUIBinaryData::metal_bg_png, PlugUIBinaryData::metal_bg_pngSize);
    return texture;
}

} // namespace

void BackgroundRenderer::paint(juce::Graphics& g) {
    paintMainBackground(g, getLocalBounds());
}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void BackgroundRenderer::paintMainBackground(juce::Graphics& g, juce::Rectangle<int> bounds,
                                             const juce::Image* overlayTexture,
                                             float overlayOpacity) {
    const float cx = static_cast<float>(bounds.getCentreX());
    const float cy = static_cast<float>(bounds.getCentreY());
    const float h  = static_cast<float>(bounds.getHeight());

    // 1. Base deep charcoal-blue color
    // Top is slightly lighter than bottom for a very subtle vertical tonal shift
    const juce::Colour topColor(0xFF131821);
    const juce::Colour bottomColor(0xFF0D1017);

    juce::ColourGradient grad(topColor, cx, 0.0f, bottomColor, cx, h, false);
    g.setGradientFill(grad);
    g.fillRect(bounds);

    // 2. Very subtle center lift (barely perceptible)
    // A soft radial glow in the middle to give the UI breathing room
    juce::ColourGradient lift(juce::Colour(0xFF181E29).withAlpha(0.25f), cx, cy * 0.9f,
                              juce::Colour(0xFF181E29).withAlpha(0.0f), cx, h * 0.9f, true);
    g.setGradientFill(lift);
    g.fillRect(bounds);

    // 3. Extremely fine micro-noise (very low opacity)
    // Drawn via tiled, pre-computed semi-transparent image
    g.setTiledImageFill(getMicroNoiseImage(), 0, 0,
                        1.0f); // Image already has low alpha
    g.fillRect(bounds);

    const juce::Image* textureToDraw = overlayTexture;
    if (textureToDraw == nullptr)
        textureToDraw = &getDefaultTextureImage();

    if (textureToDraw != nullptr && textureToDraw->isValid()) {
        const float clampedOpacity = juce::jlimit(0.0f, 1.0f, overlayOpacity);
        if (clampedOpacity > 0.0f) {
            g.setOpacity(clampedOpacity);
            g.drawImageWithin(*textureToDraw, bounds.getX(), bounds.getY(), bounds.getWidth(),
                              bounds.getHeight(), juce::RectanglePlacement::stretchToFit);
            g.setOpacity(1.0f);
        }
    }
}

// ========================================================
// LOCKED COMPONENT — PLUGUI CORE STYLE
// Do NOT change visual design here unless necessary.
// Layout or wiring changes should happen elsewhere.
// ========================================================
void BackgroundRenderer::paintEnginePanel(juce::Graphics& g, juce::Rectangle<int> bounds) {
    auto b             = bounds.toFloat();
    const float radius = 24.0f;
    const auto inner   = b.reduced(0.8f);

    // Inset engine well: slightly darker body with stronger lower falloff.
    juce::ColourGradient base(juce::Colour(0xFF000000).withAlpha(0.06f), inner.getCentreX(),
                              inner.getY(), juce::Colour(0xFF000000).withAlpha(0.13f),
                              inner.getCentreX(), inner.getBottom(), false);
    g.setGradientFill(base);
    g.fillRoundedRectangle(inner, radius);

    // Inner edge cue: restrained bottom shade only.
    auto topEdge = inner.reduced(2.0f);
    g.setColour(juce::Colours::black.withAlpha(0.24f));
    g.drawLine(topEdge.getX() + 9.0f, topEdge.getBottom() - 0.9f, topEdge.getRight() - 9.0f,
               topEdge.getBottom() - 0.9f, 1.0f);

    // Local floor shadow to seat controls in the panel cavity.
    juce::ColourGradient floorShadow(juce::Colours::transparentBlack, inner.getCentreX(),
                                     inner.getCentreY(), juce::Colours::black.withAlpha(0.16f),
                                     inner.getCentreX(), inner.getBottom() + 22.0f, false);
    g.setGradientFill(floorShadow);
    g.fillRoundedRectangle(inner.reduced(6.0f, 4.0f), radius - 5.0f);
}

} // namespace PlugUI
