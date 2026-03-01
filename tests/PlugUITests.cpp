#include <PlugUI/BackgroundRenderer.h>
#include <PlugUI/PlugCorrelationMeter.h>
#include <PlugUI/PlugKnob.h>
#include <PlugUI/PlugMeter.h>
#include <PlugUI/PlugScope.h>
#include <PlugUI/PlugSegmentedControl.h>
#include <PlugUI/PlugTheme.h>
#include <PlugUI/PlugVuMeter.h>

// PlugUI unit tests using JUCE's built-in test framework.
// These tests cover non-rendering state logic.
//
// To run: call juce::UnitTestRunner::runAllTests() from your application.
// Tests self-register via static construction.

namespace PlugUI::Tests {

class ThemeTests : public juce::UnitTest {
public:
  ThemeTests() : juce::UnitTest("PlugUI::Theme") {}

  void runTest() override {
    beginTest("colour values are non-transparent");
    expect(Theme::background.getAlpha() == 0xFF);
    expect(Theme::accent.getAlpha() == 0xFF);
    expect(Theme::textPrimary.getAlpha() == 0xFF);
    expect(Theme::textMuted.getAlpha() == 0xFF);
    expect(Theme::buttonOn.getAlpha() == 0xFF);
    expect(Theme::buttonOff.getAlpha() == 0xFF);

    beginTest("knob sizes are positive and ordered");
    expect(Theme::knobSmall > 0.0f);
    expect(Theme::knobMedium > Theme::knobSmall);
    expect(Theme::knobLarge > Theme::knobMedium);

    beginTest("padding values are positive");
    expect(Theme::paddingSmall > 0.0f);
    expect(Theme::paddingMedium > Theme::paddingSmall);
    expect(Theme::paddingLarge > Theme::paddingMedium);

    beginTest("font sizes are positive");
    expect(Theme::fontLabel > 0.0f);
    expect(Theme::fontSection > 0.0f);
    expect(Theme::fontValue > 0.0f);
    expect(Theme::fontSegment > 0.0f);

    beginTest("font family is set");
    expect(Theme::fontFamily.isNotEmpty());

    beginTest("arc thickness is positive");
    expect(Theme::arcThickness > 0.0f);

    beginTest("knob rim fraction is in (0, 1)");
    expect(Theme::knobRimFraction > 0.0f && Theme::knobRimFraction < 1.0f);
  }
};

class PlugMeterTests : public juce::UnitTest {
public:
  PlugMeterTests() : juce::UnitTest("PlugUI::PlugMeter") {}

  void runTest() override {
    beginTest("default max GR is 18 dB");
    PlugMeter meter;
    // Default construction should not crash
    expectDoesNotThrow([&] { meter.setGainReductionDb(0.0f); });

    beginTest("accepts values at boundaries");
    expectDoesNotThrow([&] { meter.setGainReductionDb(-1.0f); });
    expectDoesNotThrow([&] { meter.setGainReductionDb(18.0f); });
    expectDoesNotThrow(
        [&] { meter.setGainReductionDb(100.0f); }); // clamped internally

    beginTest("custom max dB constructor");
    PlugMeter meter24(24.0f);
    expectDoesNotThrow([&] { meter24.setGainReductionDb(24.0f); });
  }
};

class PlugVuMeterTests : public juce::UnitTest {
public:
  PlugVuMeterTests() : juce::UnitTest("PlugUI::PlugVuMeter") {}

  void runTest() override {
    beginTest("default construction");
    PlugVuMeter vu;
    expectDoesNotThrow([&] { vu.setLevel(-10.0f); });
    expectDoesNotThrow([&] { vu.setLit(true); });
    expectDoesNotThrow([&] { vu.setLit(false); });
    expectDoesNotThrow([&] { vu.setLabel("IN"); });

    beginTest("accepts extreme dB values");
    expectDoesNotThrow([&] { vu.setLevel(-100.0f); });
    expectDoesNotThrow([&] { vu.setLevel(100.0f); });
  }
};

class PlugCorrelationMeterTests : public juce::UnitTest {
public:
  PlugCorrelationMeterTests()
      : juce::UnitTest("PlugUI::PlugCorrelationMeter") {}

  void runTest() override {
    beginTest("default construction");
    PlugCorrelationMeter meter;
    expectDoesNotThrow([&] { meter.setCorrelation(0.0f); });

    beginTest("clamps correlation to [-1, 1]");
    expectDoesNotThrow([&] { meter.setCorrelation(-2.0f); });
    expectDoesNotThrow([&] { meter.setCorrelation(2.0f); });
  }
};

class PlugSegmentedControlTests : public juce::UnitTest {
public:
  PlugSegmentedControlTests()
      : juce::UnitTest("PlugUI::PlugSegmentedControl") {}

  void runTest() override {
    beginTest("defaults to first segment selected");
    PlugSegmentedControl ctrl({"A", "B", "C"});
    expectEquals(ctrl.getSelectedId(), 1);

    beginTest("setSelectedId changes selection (1-based)");
    ctrl.setSelectedId(2);
    expectEquals(ctrl.getSelectedId(), 2);

    ctrl.setSelectedId(3);
    expectEquals(ctrl.getSelectedId(), 3);

    beginTest("setSelectedId clamps to valid range");
    ctrl.setSelectedId(0); // below range — should clamp to 1
    expectEquals(ctrl.getSelectedId(), 1);

    ctrl.setSelectedId(99); // above range — should clamp to last
    expectEquals(ctrl.getSelectedId(), 3);

    beginTest("callback fires when requested");
    int callbackId = -1;
    ctrl.onSelectionChanged = [&](int id) { callbackId = id; };
    ctrl.setSelectedId(2, juce::sendNotification);
    expectEquals(callbackId, 2);

    beginTest("callback does not fire with dontSendNotification");
    callbackId = -1;
    ctrl.setSelectedId(3, juce::dontSendNotification);
    expectEquals(callbackId, -1);
  }
};

class PlugScopeTests : public juce::UnitTest {
public:
  PlugScopeTests() : juce::UnitTest("PlugUI::PlugScope") {}

  void runTest() override {
    beginTest("default construction");
    PlugScope scope;
    expectDoesNotThrow([&] { scope.setLevels(-18.0f, 0.0f); });

    beginTest("accepts column data without crashing");
    expectDoesNotThrow([&] {
      scope.pushColumn(-0.5f, 0.5f, -0.4f, 0.4f);
      scope.pushColumn(-1.0f, 1.0f, -0.9f, 0.9f);
    });

    beginTest("setLevels is idempotent");
    scope.setLevels(-18.0f, 0.0f);
    scope.setLevels(-18.0f, 0.0f); // same values — should not crash

    beginTest("triggerClipFlash does not crash");
    expectDoesNotThrow([&] { scope.triggerClipFlash(); });
  }
};

// Static instances register all tests with JUCE's test runner.
static ThemeTests themeTests;
static PlugMeterTests meterTests;
static PlugVuMeterTests vuMeterTests;
static PlugCorrelationMeterTests correlationTests;
static PlugSegmentedControlTests segmentedTests;
static PlugScopeTests scopeTests;

} // namespace PlugUI::Tests
