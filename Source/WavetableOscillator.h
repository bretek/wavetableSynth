/*
  ==============================================================================

    WavetableOscillator.h
    Created: 25 Jun 2023 11:15:35pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <ctime>

#include "WavetableSynthParameters.h"

class WavetableOscillator
{
public:
    WavetableOscillator ();

    void setFrequency (float frequency);
    void setAmplitude (float amplitude);
    void setPan (float pan);

    float getNextSample ();

    bool isPlaying () const;
    void start ();
    void stop ();

private:
    inline float interpolateSamples (std::vector<float> samples, float index) const;
    inline float calculateSampleIndexIncrement (float frequency) const;
    inline float calculateRandomStartSample (float phase, float random) const;

    WavetableSynthParameters* wavetableSynthParameters;

    double* sampleRate;

    std::vector<float>* wavetableSamples;
    std::atomic<float>* phase;
    std::atomic<float>* random;

    float frequency = 0.f;
    float amplitude = 1.f;
    float pan = 0.5f;

    float sampleIndexIncrement = 0.f;
    float currentSampleIndex = 0.f;

    bool playing = false;
};