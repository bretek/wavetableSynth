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

    void setUnison (std::atomic<float>* numVoices);
    void setDetune (std::atomic<float>* detune);
    void setBlend (std::atomic<float>* blend);
    void setPhase (std::atomic<float>* phase);
    void setRandom (std::atomic<float>* random);
    void setPan (std::atomic<float>* pan);
    void setLevel (std::atomic<float>* level);
private:
    void renderAudio (juce::AudioBuffer<float>& buffer, int startSample, int endSample);
    void handleMidi (juce::MidiMessage& message);

    WavetableNote note;
    double sampleRate;

    std::atomic<float>* level;
    std::atomic<float>* pan;

    float amplitude = 1.f;

    int octaveOffset = 0;
    int semiOffset = 0;
    float fineOffset = 0.f;
    float coarsePitch = 0.f;
};