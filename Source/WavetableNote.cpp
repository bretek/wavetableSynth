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

    // keep total level at one during blend
    float scaleFactor;
    if (static_cast<int>(*numVoices) % 2 == 0)
    {
        scaleFactor = 2.f / (2.f + (((*numVoices) - 2.f) * (*blend)));
    }
    else
    {
        scaleFactor = 1.f / (1.f + (((*numVoices) - 1.f) * (*blend)));
    }

    for (int voice = 0; voice < static_cast<int>(*numVoices); ++voice)
    {
        WavetableOscillator* newOsc = new WavetableOscillator();
        
        newOsc->sampleRate = sampleRate;
        newOsc->setWavetable (wavetableSamples);
        newOsc->setPhase (phase);
        newOsc->setRandom (random);
        newOsc->setRandomStartIndex ();
        newOsc->setFrequency (currentFrequency);
        newOsc->setAmplitude ((*blend) * scaleFactor);

        voices.push_back (*newOsc);
        currentFrequency += (*detune);
    }

    // set centre voices louder
    voices[(static_cast<int>(*numVoices)/2)].setAmplitude (1.f * scaleFactor);
    if (static_cast<int>(*numVoices) % 2 == 0)
    {
        voices[(static_cast<int>(*numVoices)/2)-1].setAmplitude (1.f * scaleFactor);
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

void WavetableNote::setPhase (std::atomic<float>* phase)
{
    this->phase = phase;
    for (auto& voice : voices)
    {
        voice.setPhase (phase);
    }
}

void WavetableNote::setRandom (std::atomic<float>* random)
{
    this->random = random;
    for (auto& voice : voices)
    {
        voice.setRandom (random);
    }
}

void WavetableNote::setWavetable (std::vector<float>* wavetableSamples)
{
    this->wavetableSamples = wavetableSamples;
    for (auto& voice : voices)
    {
        voice.setWavetable (wavetableSamples);
    }
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