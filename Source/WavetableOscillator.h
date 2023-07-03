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

#define SINC_KERNEL_SIZE 3

class WavetableOscillator
{
public:
    WavetableOscillator ();

    void setFrequency (float frequency);
    float getNextSample ();

    bool isPlaying () const;
    void start ();
    void stop ();

private:
    inline float lanczosKernel (float x) const;
    inline float sincInterpolateSamples (float index) const;
    inline float calculateSampleIndexIncrement (float frequency) const;
    inline float calculateRandomStartSample (float phase, float random) const;

    WavetableSynthParameters* wavetableSynthParameters;

    double* sampleRate;

    std::vector<float>* wavetableSamples;
    std::atomic<float>* phase;
    std::atomic<float>* random;

    float frequency = 0.f;

    float sampleIndexIncrement = 0.f;
    float currentSampleIndex = 0.f;

    bool playing = false;
};