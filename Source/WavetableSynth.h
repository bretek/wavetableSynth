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
    void prepareToPlay (double sampleRate);
    void processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&);

    void setPitchControls (std::atomic<float>* octave, std::atomic<float>* semi, std::atomic<float>* fine, std::atomic<float>* coarse);

    void setUnison (std::atomic<float>* numVoices);
    void setDetune (std::atomic<float>* detune);
    void setBlend (std::atomic<float>* blend);
    void setPhase (std::atomic<float>* phase);
    void setRandom (std::atomic<float>* random);
    void setPan (std::atomic<float>* pan);
    void setLevel (std::atomic<float>* level);

    std::vector<float> wavetableSamples;
private:
    void renderAudio (juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    void handleMidi (juce::MidiMessage& message);

    void initWavetableSin ();
    void initWavetableSaw ();
    void initWavetableSquare ();

    WavetableSynthParameters* wavetableSynthParameters;

    std::vector<WavetableNote> notes;
    double sampleRate;

    std::atomic<float>* level;
    std::atomic<float>* pan;

    float amplitude = 1.f;

    std::atomic<float>* octaveOffset;
    std::atomic<float>* semiOffset;
    std::atomic<float>* fineOffset;
    std::atomic<float>* coarsePitch;
};