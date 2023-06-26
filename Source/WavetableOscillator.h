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
    void initWavetableSin ();
    void initWavetableSaw ();

    void setFrequency (float frequency);

    float getSample ();
    bool isPlaying ();
    void stop ();
    
    double sampleRate;

private:
    float getSampleIndexIncrement (float frequency);

    void setRandomStartIndex ();

    float samples[WAVETABLE_LENGTH];

    float sampleIndexIncrement = 0.f;
    float currentSampleIndex = 0.f;

    float phase = 0.f;
    float random = 1.f;
};