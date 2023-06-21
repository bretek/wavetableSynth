/*
  ==============================================================================

    Oscillator.h
    Created: 20 Jun 2023 11:41:27pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SerumLookAndFeel.h"
#include "ControlKnob.h"
#include "AdvancedLabel.h"
#include "WtComponent.h"
#include "AdvancedFlexBox.h"

//==============================================================================
/*
*/
class Oscillator  : public juce::Component
{
public:
    Oscillator();
    ~Oscillator() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // looks and feels
    SerumLookAndFeel serumLookAndFeel;
    SerumRotarySliderDial serumDial;
    SerumSliderBox serumSliderBox;

    // components
    juce::ToggleButton oscToggle;
    AdvancedLabel titleLabel;

    WtComponent wtComponent;

    ControlKnob unisonControl;
    ControlKnob detuneControl;
    ControlKnob blendControl;
    ControlKnob phaseControl;
    ControlKnob randControl;
    ControlKnob wtControl;
    ControlKnob morphControl;
    ControlKnob panControl;
    ControlKnob levelControl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};
