cmake_minimum_required(VERSION 3.22)

project(PlugUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Sources ────────────────────────────────────────────────────────────────────
add_library(PlugUI STATIC
    src/PlugLookAndFeel.cpp
    src/PlugKnob.cpp
    src/PlugSectionLabel.cpp
    src/PlugMeter.cpp
    src/PlugSegmentedControl.cpp
    src/PlugPillButton.cpp
)

# ── Public headers ─────────────────────────────────────────────────────────────
target_include_directories(PlugUI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# ── JUCE link — only GUI modules needed ────────────────────────────────────────
# When consumed via add_subdirectory inside a JUCE project, the juce:: targets
# are already defined by the parent build.
target_link_libraries(PlugUI PUBLIC
    juce::juce_gui_basics
    juce::juce_graphics
)

# Suppress JUCE web / curl in the PlugUI compile context
target_compile_definitions(PlugUI PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)
