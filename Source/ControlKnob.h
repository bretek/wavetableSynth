/*
  ==============================================================================

    ControlKnob.h
    Created: 17 Jun 2023 3:21:53pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AdvancedSlider.h"
#include "AdvancedLabel.h"

class ControlKnob : public juce::Component
{
public:
    ControlKnob ();
    void paint (juce::Graphics& g) override;
    void setText (juce::String labelText);
    void resized() override;

    AdvancedSlider knob;
    AdvancedLabel label;
};