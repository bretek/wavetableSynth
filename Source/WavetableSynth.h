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
#include "WavetableSynthParameters.h"

#define NUM_NOTES 128
#define WAVETABLE_LENGTH 2048

class WavetableSynth
{
public:
    WavetableSynth ();
    void prepareToPlay (double sampleRate);
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&);

    std::vector<float> wavetableSamples;
private:
    void renderAudio (juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    void handleMidi (juce::MidiMessage& message);

    void initWavetableSin ();
    void initWavetableSaw ();
    void initWavetableSquare ();

    WavetableSynthParameters* wavetableSynthParameters;

    double sampleRate;

    std::atomic<float>* octaveOffset;
    std::atomic<float>* semiOffset;
    std::atomic<float>* fineOffset;
    std::atomic<float>* coarsePitch;
    std::atomic<float>* level;
    std::atomic<float>* pan;

    std::vector<WavetableNote> notes;

    float amplitude = 1.f;
};