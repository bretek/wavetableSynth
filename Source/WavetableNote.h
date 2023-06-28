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
    void setFrequency (float frequency);
    float getSample ();
    void stop ();
    bool isPlaying ();

    double sampleRate;
private:
    std::vector<WavetableOscillator> voices;

    float frequency = 0.f;
    int numVoices = 5;
    float blend = 0.5f;
    float detune = 0.25f;
};