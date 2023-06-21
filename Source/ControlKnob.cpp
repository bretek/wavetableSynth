/*
  ==============================================================================

    ControlKnob.cpp
    Created: 17 Jun 2023 3:21:53pm
    Author:  joseph

  ==============================================================================
*/

#include "ControlKnob.h"

ControlKnob::ControlKnob ()
{
    knob.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    knob.setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);

    addAndMakeVisible (&knob);
    addAndMakeVisible (&label);
}

void ControlKnob::setText (juce::String labelText)
{
    label.setText (labelText, juce::NotificationType::dontSendNotification);
}

void ControlKnob::paint (juce::Graphics& g)
{
    
}

void ControlKnob::resized()
{
    auto area = getLocalBounds();
    knob.setBoundsRelative (0.0f, 0.0f, 1.0f, 0.8f);
    label.setBoundsRelative (0.0f, 0.8f, 1.0f, 0.2f);
}
