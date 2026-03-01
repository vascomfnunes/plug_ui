cmake_minimum_required(VERSION 3.22)

project(PlugUI VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# ── Sources ────────────────────────────────────────────────────────────────────
add_library(PlugUI STATIC
    src/PlugLookAndFeel.cpp
    src/PlugKnob.cpp
    src/PlugSectionLabel.cpp
    src/PlugMeter.cpp
    src/PlugSegmentedControl.cpp
    src/PlugPillButton.cpp
    src/BackgroundRenderer.cpp
    src/PlugHeader.cpp
    src/PlugPanel.cpp
    src/PlugSignature.cpp
    src/PlugScope.cpp
    src/PlugVuMeter.cpp
    src/PlugCorrelationMeter.cpp
)

if(COMMAND juce_add_binary_data)
    juce_add_binary_data(PlugUIAssets
        NAMESPACE PlugUIBinaryData
        SOURCES assets/metal_bg.png
    )
endif()

# ── Public headers ─────────────────────────────────────────────────────────────
target_include_directories(PlugUI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)


# ── JUCE link — only GUI modules needed ────────────────────────────────────────
# When consumed via add_subdirectory inside a JUCE project, the juce:: targets
# are already defined by the parent build.
if(COMMAND juce_add_binary_data)
    target_link_libraries(PlugUI PUBLIC
        juce::juce_gui_basics
        juce::juce_graphics
    )
    target_link_libraries(PlugUI PRIVATE PlugUIAssets)
endif()

# Suppress JUCE web / curl in the PlugUI compile context
target_compile_definitions(PlugUI PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)

# ── Tests ───────────────────────────────────────────────────────────────────────
# Tests require a JUCE application context provided by the parent build.
# Enable with: cmake -DPLUGUI_BUILD_TESTS=ON
option(PLUGUI_BUILD_TESTS "Build PlugUI unit tests" OFF)

if(PLUGUI_BUILD_TESTS AND TARGET juce::juce_gui_basics)
    juce_add_console_app(PlugUITestRunner PRODUCT_NAME "PlugUI Tests")
    target_sources(PlugUITestRunner PRIVATE tests/PlugUITests.cpp)
    target_link_libraries(PlugUITestRunner PRIVATE PlugUI juce::juce_gui_basics)
    target_compile_definitions(PlugUITestRunner PRIVATE
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_USE_OGGVORBIS=0
        JUCE_USE_FLAC=0
        JUCE_USE_MP3AUDIOFORMAT=0
    )
    enable_testing()
    add_test(NAME PlugUITests COMMAND PlugUITestRunner)
endif()
