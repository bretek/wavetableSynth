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
#include "../WavetableSynthParameters.h"

#define NUM_NOTES 128
#define WAVETABLE_LENGTH 2048
#define OVERSAMPLING_RATIO 4
#define OVERSAMPLING_FACTOR 2

class WavetableSynth
{
public:
    WavetableSynth ();
    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&);

    std::vector<float> wavetableSamples;
private:
    void renderAudio (juce::dsp::AudioBlock<float>& buffer, int startSample, int endSample);
    void panAudio (juce::AudioBuffer<float>& buffer);
    void handleMidi (juce::MidiMessage& message);

    void initWavetableSin ();
    void initWavetableSaw ();
    void initWavetableSquare ();

    WavetableSynthParameters* wavetableSynthParameters;

    std::unique_ptr<juce::dsp::Oversampling<float>> oversampling;

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