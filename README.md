# PlugUI

A reusable JUCE-based UI framework for audio plugins.

## Overview

PlugUI provides a complete design system and reusable components for building
modern, minimal audio plugin interfaces. It includes:

- **PlugTheme** — Centralised design tokens (colours, spacing, typography, sizes)
- **PlugLookAndFeel** — Custom JUCE LookAndFeel with flat knobs, thin accent arcs, white pointer indicators
- **PlugKnob / PlugSmallKnob** — Labelled rotary knob components
- **PlugSectionLabel** — Uppercase section header
- **PlugMeter** — Horizontal gain-reduction meter
- **PlugSegmentedControl** — Flat segmented buttons (accepts any labels)
- **PlugPillButton** — Small rounded toggle badge

## Usage

### Integrating via CMake

Add PlugUI as a subdirectory in your JUCE project:

```cmake
add_subdirectory(external/PlugUI)
target_link_libraries(YourPlugin PRIVATE PlugUI)
```

### Requirements

- C++17
- JUCE 7 or 8 (must be available in the parent build)

### Quick Start

```cpp
#include <PlugUI/PlugTheme.h>
#include <PlugUI/PlugLookAndFeel.h>
#include <PlugUI/PlugKnob.h>

// In your editor constructor:
PlugUI::PlugLookAndFeel laf;
setLookAndFeel(&laf);

PlugUI::PlugKnob gainKnob;
gainKnob.init("GAIN");
addAndMakeVisible(gainKnob.slider);
addAndMakeVisible(gainKnob.label);
```

## Design Philosophy

- **Zero plugin-specific code** — PlugUI contains no parameter IDs or plugin logic
- **Theme-driven** — All visual constants come from `PlugUI::Theme`
- **Flat & modern** — No bevels, no glows, minimal aesthetic
- **Reusable** — Drop into any JUCE plugin project

## License

MIT
