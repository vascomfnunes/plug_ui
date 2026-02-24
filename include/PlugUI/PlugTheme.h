#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace PlugUI::Theme {

// ─── Colours ──────────────────────────────────────────────────────────────────
inline const juce::Colour background{0xFF14181D};
inline const juce::Colour panelBg{0xFF1A1F25};
inline const juce::Colour borderSubtle{0xFF2A3037};

inline const juce::Colour accent{0xFFA88245};
inline const juce::Colour accentBright{0xFFCAA86A};
inline const juce::Colour accentDim{0xFF6D5B3D};

inline const juce::Colour textPrimary{0xFFE1E6EC};
inline const juce::Colour textMuted{0xFF9AA3AD};
inline const juce::Colour textValue{0xFFC2CAD3};

inline const juce::Colour knobFill{0xFF252B32};
inline const juce::Colour knobArc{0xFF343C45};
inline const juce::Colour knobIndicator{0xFFFFFFFF};

inline const juce::Colour meterFill = accentBright;
inline const juce::Colour divider   = borderSubtle;

inline const juce::Colour buttonOn{0xFFA97F32};
inline const juce::Colour buttonOff{0xFF3C434C};
inline const juce::Colour buttonOnText{0xFF14181C};
inline const juce::Colour buttonOffText{0xFFB6BEC6};

// ─── Spacing ──────────────────────────────────────────────────────────────────
inline constexpr float paddingSmall  = 2.0f;
inline constexpr float paddingMedium = 5.0f;
inline constexpr float paddingLarge  = 8.0f;

// ─── Knob sizes (base, before UI scale) ───────────────────────────────────────
inline constexpr float knobLarge  = 110.0f;
inline constexpr float knobMedium = 88.0f;
inline constexpr float knobSmall  = 74.0f;

// ─── Typography sizes (base, before UI scale) ────────────────────────────────
inline constexpr float fontSection = 10.2f;
inline constexpr float fontLabel   = 8.8f;
inline constexpr float fontValue   = 7.5f;
inline constexpr float fontSegment = 8.0f;

// ─── Font name ────────────────────────────────────────────────────────────────
inline const juce::String fontFamily{"Helvetica Neue"};

} // namespace PlugUI::Theme
