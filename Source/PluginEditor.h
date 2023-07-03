/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

#include "SerumLookAndFeel.h"
#include "WavetableSynth/Editor/Oscillator.h"

//==============================================================================
/**
*/
class SerumAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SerumAudioProcessorEditor (SerumAudioProcessor&, juce::AudioProcessorValueTreeState& valueTreeState, std::vector<float>* wt);
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
    std::vector<float>* wavetable;

    Oscillator osc1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SerumAudioProcessorEditor)
};
