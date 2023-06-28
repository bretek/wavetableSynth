/*
  ==============================================================================

    WavetableNote.cpp
    Created: 28 Jun 2023 4:54:42pm
    Author:  joseph

  ==============================================================================
*/

#include "WavetableNote.h"

void WavetableNote::setFrequency (float frequency)
{
    voices.clear();

    float frequencyDifference = .25f;
    float currentFrequency = frequency - (((numVoices-1) / 2) * frequencyDifference);

    for (int voice = 0; voice < numVoices; ++voice)
    {
        WavetableOscillator* newOsc = new WavetableOscillator();
        
        newOsc->sampleRate = sampleRate;
        newOsc->setFrequency (currentFrequency);
        voices.push_back (*newOsc);
        currentFrequency += frequencyDifference;
    }

    this->frequency = frequency;
}

float WavetableNote::getSample ()
{
    float currentSample = 0;
    for (auto& voice : voices)
    {
        if (voice.isPlaying())
        {
            currentSample += voice.getSample ();
        }
    }

    return currentSample;
}

void WavetableNote::stop ()
{
    frequency = 0.f;
    for (auto& voice : voices)
    {
        voice.stop();
    }
}

bool WavetableNote::isPlaying ()
{
    return frequency != 0.f;
}