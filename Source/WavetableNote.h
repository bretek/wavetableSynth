/*
  ==============================================================================

    WavetableNote.h
    Created: 28 Jun 2023 4:54:42pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <vector>

#include "WavetableOscillator.h"

class WavetableNote
{
public:
    WavetableNote () {};

    void setFrequency (float frequency);
    void setAmplitude (float amplitude);
    void setNumVoices (std::atomic<float>* numVoices);
    void setDetune (std::atomic<float>* detune);
    void setBlend (std::atomic<float>* blend);
    float getSample ();
    void stop ();
    bool isPlaying ();

    double sampleRate;
private:
    std::vector<WavetableOscillator> voices;

    std::vector<float>* wavetableSamples;

    float frequency = 0.f;
    std::atomic<float>* numVoices;
    std::atomic<float>* detune;
    std::atomic<float>* blend;
    std::atomic<float>* phase;
    std::atomic<float>* random;
    float amplitude;
};