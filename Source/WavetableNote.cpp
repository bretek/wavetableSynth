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

    float currentFrequency = frequency - (((static_cast<int>(*numVoices)-1) / 2) * (*detune));

    for (int voice = 0; voice < static_cast<int>(*numVoices); ++voice)
    {
        WavetableOscillator* newOsc = new WavetableOscillator();
        
        newOsc->sampleRate = sampleRate;
        newOsc->setFrequency (currentFrequency);
        newOsc->setAmplitude ((amplitude / static_cast<int>(*numVoices)) * (*blend));
        voices.push_back (*newOsc);
        currentFrequency += (*detune);
    }

    voices[(static_cast<int>(*numVoices)/2)].setAmplitude (amplitude / static_cast<int>(*numVoices));
    if (static_cast<int>(*numVoices) % 2 == 0)
    {
        voices[(static_cast<int>(*numVoices)/2)-1].setAmplitude (amplitude / static_cast<int>(*numVoices));
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

void WavetableNote::setNumVoices (std::atomic<float>* numVoices)
{
    this->numVoices = numVoices;
}

void WavetableNote::setDetune (std::atomic<float>* detune)
{
    this->detune = detune;
}

void WavetableNote::setBlend (std::atomic<float>* blend)
{
    this->blend = blend;
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