/*
  ==============================================================================

    WavetableOscillator.h
    Created: 25 Jun 2023 11:15:35pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <cmath>
#include <ctime>
#include <iostream>

#define WAVETABLE_LENGTH 2048

class WavetableOscillator
{
public:
    WavetableOscillator ();

    void setFrequency (float frequency);
    void setAmplitude (float amplitude);
    void setWavetable (std::vector<float>* wavetableSamples);

    void setPhase (std::atomic<float>* phase);
    void setRandom (std::atomic<float>* random);
    void setRandomStartIndex ();

    float getSample ();
    bool isPlaying ();
    void stop ();
    
    double sampleRate;

private:
    float getSampleIndexIncrement (float frequency);

    float calculateRandomStartSample (float phase, float random);

    std::vector<float>* samples;

    float sampleIndexIncrement = 0.f;
    float currentSampleIndex = 0.f;

    std::atomic<float>* phase;
    std::atomic<float>* random;

    float amplitude = 0.f;
};