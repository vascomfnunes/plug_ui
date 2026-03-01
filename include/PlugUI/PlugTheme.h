#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI::Theme {

// ─── Colours
// ──────────────────────────────────────────────────────────────────
// Background: deep charcoal/blue-black
inline const juce::Colour background{0xFF0D1017};
inline const juce::Colour panelBg{0xFF151A24};
inline const juce::Colour borderSubtle{0xFF2A3140};

// Accent: muted warm gold — brighter than before (~12% luminance bump), no glow
inline const juce::Colour accent{0xFFC29A63};
inline const juce::Colour accentBright{0xFFD8B47A};
inline const juce::Colour accentDim{0xFF7E6848};

// Text: high-contrast off-white / medium grey
inline const juce::Colour textPrimary{0xFFE5E8EE};
inline const juce::Colour textMuted{0xFFA3ABB8};
inline const juce::Colour textValue{0xFFD7DCE4};

// Knobs: body face — warm neutral charcoal (lighter element)
inline const juce::Colour knobFill{0xFF2B2F36};
inline const juce::Colour knobFillCentre{0xFF393E46};
inline const juce::Colour knobArc{0xFF576073};
inline const juce::Colour knobIndicator{0xFFFFFFFF};
// Rim: wide dark shelf — darker than the body, warm neutral
inline const juce::Colour knobRimLight{0xFF262A32};
inline const juce::Colour knobRimDark{0xFF1A1E25};

// Meter
inline const juce::Colour meterBg{0xFF0E1115};
inline const juce::Colour meterFill = accentBright;
inline const juce::Colour divider = borderSubtle;

// Buttons
inline const juce::Colour buttonOn = accent;
inline const juce::Colour buttonOff{0xFF282E36};
inline const juce::Colour buttonOnText{0xFF111419};
inline const juce::Colour buttonOffText{0xFFB0B8C2};

// ─── Spacing (reduced ~8–10% for denser layout) ──────────────────────────────
inline constexpr float paddingSmall = 2.0f;
inline constexpr float paddingMedium = 5.0f;
inline constexpr float paddingLarge = 8.0f;

// ─── Knob sizes (base, before UI scale)
// ───────────────────────────────────────
inline constexpr float knobLarge = 110.0f;
inline constexpr float knobMedium = 88.0f;
inline constexpr float knobSmall = 74.0f;

// ─── Knob rim proportions ────────────────────────────────────────────────────
inline constexpr float knobRimFraction = 0.23f; // wide shelf/bezel
inline constexpr float knobRimMin = 4.0f;       // minimum rim width in px

// ─── Value arc ───────────────────────────────────────────────────────────────
inline constexpr float arcThickness = 1.9f; // thin gold value arc

// ─── Typography sizes (base, before UI scale) ────────────────────────────────
inline constexpr float fontSection = 10.2f;
inline constexpr float fontLabel = 12.5f;
inline constexpr float fontValue = 7.5f;
inline constexpr float fontSegment = 8.0f;

// ─── Font name
// ────────────────────────────────────────────────────────────────
inline const juce::String fontFamily{"Avenir Next"};

} // namespace PlugUI::Theme
