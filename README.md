# PlugUI

A reusable JUCE-based UI framework for audio plugins. Drop it into any JUCE project to get a complete, theme-consistent set of components with zero plugin-specific logic.

## Components

| Component | Description |
|---|---|
| `PlugTheme` | Centralised design tokens — colours, spacing, typography, sizes |
| `PlugLookAndFeel` | Custom JUCE LookAndFeel with knobs, faders, buttons, labels, combo boxes |
| `PlugKnob` | Labelled rotary knob (label above, uppercase) |
| `PlugMeter` | Horizontal gain-reduction meter with dB scale |
| `PlugGrMeter` | Analog-style needle compressor gain-reduction meter |
| `PlugLedMeter` | Vertical 10-segment LED column meter (-30 to 0 dBFS) |
| `PlugVuMeter` | Analog-style needle VU meter with lit/unlit states |
| `PlugCorrelationMeter` | Stereo phase correlation bar (−1 to +1) |
| `PlugScope` | Scrolling pre/post amplitude scope with clip indication |
| `PlugSegmentedControl` | Flat segmented button strip (any number of labels) |
| `PlugPillButton` | Small rounded toggle badge |
| `PlugSectionLabel` | Uppercase section header |
| `PlugHeader` | Plugin title and subtitle strip |
| `PlugPanel` | Translucent panel overlay for grouping sections |
| `PlugSignature` | Brand signature footer |
| `BackgroundRenderer` | Procedural background — deep charcoal-blue with subtle tonal shifts and optional texture overlay |

## Requirements

- C++17
- JUCE 7 or 8 (provided by the parent project)
- CMake 3.22+

## Integration

Add PlugUI as a subdirectory in your JUCE plugin project:

```cmake
add_subdirectory(external/PlugUI)
target_link_libraries(YourPlugin PRIVATE PlugUI)
```

PlugUI expects JUCE to already be configured in the parent build (`juce::juce_gui_basics` and `juce::juce_graphics` must be available).

## Quick Start

```cpp
#include <PlugUI/PlugTheme.h>
#include <PlugUI/PlugLookAndFeel.h>
#include <PlugUI/PlugKnob.h>
#include <PlugUI/BackgroundRenderer.h>

class MyEditor : public juce::AudioProcessorEditor {
    PlugUI::PlugLookAndFeel laf;
    PlugUI::PlugKnob gainKnob;

    MyEditor() {
        setLookAndFeel(&laf);

        gainKnob.init("GAIN");
        addAndMakeVisible(gainKnob.slider);
        addAndMakeVisible(gainKnob.label);
    }

    void paint(juce::Graphics& g) override {
        PlugUI::BackgroundRenderer::paintMainBackground(g, getLocalBounds());
    }

    ~MyEditor() { setLookAndFeel(nullptr); }
};
```

## Background

```cpp
// Default built-in texture
PlugUI::BackgroundRenderer::paintMainBackground(g, getLocalBounds());

// Custom texture at a specific opacity
PlugUI::BackgroundRenderer::paintMainBackground(g, getLocalBounds(), &myImage, 0.18f);

// Engine panel (darker inset well, used by PlugPanel internally)
PlugUI::BackgroundRenderer::paintEnginePanel(g, panelBounds);
```

## Meters

```cpp
// Gain reduction meter (horizontal bar)
PlugUI::PlugMeter grMeter;
grMeter.setGainReductionDb(6.5f); // positive value = dB of reduction

// Analog-style gain reduction meter (needle)
PlugUI::PlugGrMeter grNeedleMeter;
grNeedleMeter.setGainReductionDb(3.0f); // deflects left

// Vertical LED level meter
PlugUI::PlugLedMeter ledMeter;
ledMeter.setLevelDb(-6.0f); // -30 dBFS to 0 dBFS

// VU meter
PlugUI::PlugVuMeter vuMeter;
vuMeter.setLabel("IN");
vuMeter.setLit(true);            // false = dimmed "device off" look
vuMeter.setLevel(-3.0f);         // dB

// Stereo correlation meter
PlugUI::PlugCorrelationMeter corrMeter;
corrMeter.setCorrelation(0.85f); // −1.0 to 1.0
```

## Segmented Controls

```cpp
PlugUI::PlugSegmentedControl mode({"SMOOTH", "TAPE", "PUNCH"});
mode.onSelectionChanged = [](int id) { /* id is 1-based */ };
mode.setSelectedId(2); // select "TAPE"
```

## Pill Buttons

```cpp
PlugUI::PlugPillButton autoBtn("A");
autoBtn.setClickingTogglesState(true);
addAndMakeVisible(autoBtn);
```

## Design

PlugUI uses a single locked colour palette (`PlugUI::Theme`) and a custom `PlugLookAndFeel` that all components share. See [PlugUI_SPEC.md](PlugUI_SPEC.md) for the visual specification and locked component rules.

## License

GPL-3.0-or-later. See [LICENSE](LICENSE).

## Tests

PlugUI includes unit tests built with JUCE's UnitTest framework. They exercise non-rendering state logic for core components such as Theme, PlugMeter, PlugVuMeter, PlugCorrelationMeter, PlugSegmentedControl, and PlugScope.

- What runs
  - Tests are compiled and run by enabling tests in CMake:
    - cmake -S . -B build -DPLUGUI_BUILD_TESTS=ON
  - Build the test runner:
    - cmake --build build --target PlugUITestRunner
  - Run the tests (console runner):
    - ./build/PlugUITestRunner

- Tests covered
  - Theme: verifies alpha channels, knob sizes, padding, font sizes, font family, arc thickness, knob rim fraction
  - PlugMeter: default max gain reduction, boundary values, custom max
  - PlugVuMeter: default construction and basic setters
  - PlugCorrelationMeter: default, clamps to [-1, 1]
  - PlugSegmentedControl: default selection, setSelectedId behavior, callbacks, notification types
  - PlugScope: default construction, column data handling, idempotent setLevels, clip flash

- How to run in CI
  - Ensure the JUCE environment is available in the CI runner.
  - Enable PLUGUI_BUILD_TESTS in the CMake invocation as shown above.
  - CI will execute the tests as part of the build, reporting failures if any.

- Notes
  - Tests are designed to be self-contained and do not render UI; they validate internal state and behavior.
  - If you modify tests, ensure you update or add corresponding test coverage as needed.
