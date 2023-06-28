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

    float frequency;
    int unison;
    float detune = 1.f;
    float blend = 0.5f;
    
    float unisonLowTune;

    float blendAmp;
};