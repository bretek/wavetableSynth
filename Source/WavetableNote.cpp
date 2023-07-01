/*
  ==============================================================================

    WavetableNote.cpp
    Created: 28 Jun 2023 4:54:42pm
    Author:  joseph

  ==============================================================================
*/

#include "WavetableNote.h"

WavetableNote::WavetableNote ()
{
    extern struct WavetableSynthParameters wavetableSynthParametersExt;
    wavetableSynthParameters = &wavetableSynthParametersExt;
    numVoices = wavetableSynthParameters->unisonParameter;
    detune = wavetableSynthParameters->detuneParameter;
    blend = wavetableSynthParameters->blendParameter;
}

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
        
        newOsc->setFrequency (currentFrequency);
        newOsc->setAmplitude ((*blend) * scaleFactor);
        newOsc->start ();

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

float WavetableNote::getSample ()
{
    float currentSample = 0;
    for (auto& voice : voices)
    {
        if (voice.isPlaying())
        {
            currentSample += voice.getNextSample ();
        }
    }

    return currentSample;
}

void WavetableNote::start ()
{
    playing = true;
    for (auto& voice : voices)
    {
        voice.start();
    }
}

void WavetableNote::stop ()
{
    playing = false;
    for (auto& voice : voices)
    {
        voice.stop();
    }
}

bool WavetableNote::isPlaying () const
{
    return playing;
}