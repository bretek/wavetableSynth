/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "SerumLookAndFeel.h"
#include "Oscillator.h"

//==============================================================================
/**
*/
class SerumAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SerumAudioProcessorEditor (SerumAudioProcessor&, juce::AudioProcessorValueTreeState& valueTreeState);
    ~SerumAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SerumAudioProcessor& audioProcessor;

    SerumLookAndFeel serumLookAndFeel;

    juce::AudioProcessorValueTreeState& valueTreeState;

    Oscillator osc1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SerumAudioProcessorEditor)
};
