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

    float currentFrequency = frequency - (((numVoices-1) / 2) * detune);

    for (int voice = 0; voice < numVoices; ++voice)
    {
        WavetableOscillator* newOsc = new WavetableOscillator();
        
        newOsc->sampleRate = sampleRate;
        newOsc->setFrequency (currentFrequency);
        newOsc->setAmplitude ((amplitude / numVoices) * blend);
        voices.push_back (*newOsc);
        currentFrequency += detune;
    }

    voices[(numVoices/2)].setAmplitude (amplitude / numVoices);
    if (numVoices % 2 == 0)
    {
        voices[(numVoices/2)-1].setAmplitude (amplitude / numVoices);
    }

    this->frequency = frequency;
}

void WavetableNote::setAmplitude (float amplitude)
{
    if (amplitude > 1.f)
    {
        amplitude = 0;
    }
    
    this->amplitude = amplitude;
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