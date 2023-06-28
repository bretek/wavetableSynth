/*
  ==============================================================================

    WavetableSynth.h
    Created: 25 Jun 2023 11:15:27pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <vector>

#include "WavetableNote.h"

class WavetableSynth
{
public:
    void prepareToPlay (double sampleRate);
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&);

    void setUnison (int unison);
private:
    void renderAudio (juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    void handleMidi (juce::MidiMessage& message);

    WavetableNote note;
    double sampleRate;

    float pan = 0.3f;
};