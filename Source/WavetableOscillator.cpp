/*
  ==============================================================================

    WavetableOscillator.cpp
    Created: 25 Jun 2023 11:15:35pm
    Author:  joseph

  ==============================================================================
*/

#include "WavetableOscillator.h"

WavetableOscillator::WavetableOscillator ()
{
    std::srand (std::time (nullptr));

    extern struct WavetableSynthParameters wavetableSynthParametersExt;
    wavetableSynthParameters = &wavetableSynthParametersExt;
    wavetableSamples = wavetableSynthParameters->wavetableSamples;
    phase = wavetableSynthParameters->phaseParameter;
    random = wavetableSynthParameters->randomParameter;
    sampleRate = wavetableSynthParameters->sampleRate;
}

void WavetableOscillator::setFrequency (float frequency)
{
    this->frequency = frequency;
    sampleIndexIncrement = calculateSampleIndexIncrement (frequency);
}

void WavetableOscillator::setAmplitude (float amplitude)
{
    this->amplitude = amplitude;
}

void WavetableOscillator::setPan (float pan)
{
    this-> pan = pan;
}

float WavetableOscillator::getNextSample ()
{
    const int floorIndex = std::fmod(currentSampleIndex, static_cast<float>(wavetableSamples->size()));
    const float sample = ((currentSampleIndex - floorIndex) * (*wavetableSamples)[floorIndex + 1]) +
            ((floorIndex + 1 - currentSampleIndex) * (*wavetableSamples)[floorIndex]);
    currentSampleIndex += sampleIndexIncrement;

    if (currentSampleIndex >= wavetableSamples->size() - 1)
    {
        currentSampleIndex -= static_cast<float>(wavetableSamples->size() - 1);
    }

    return sample * amplitude;
}

bool WavetableOscillator::isPlaying () const
{
    return playing;
}

void WavetableOscillator::start ()
{
    currentSampleIndex = calculateRandomStartSample ((*phase), (*random));
    playing = true;
}

void WavetableOscillator::stop ()
{
    playing = false;
}

float WavetableOscillator::calculateSampleIndexIncrement (float frequency) const
{
    return (frequency * static_cast<float>(wavetableSamples->size())) / (float)(*sampleRate);
}

float WavetableOscillator::calculateRandomStartSample (float phase, float random) const
{
    float randPhase = (((std::rand() % wavetableSamples->size()) * random) + ((wavetableSamples->size() * (phase - random)) / 2));
    randPhase = (static_cast<int>(randPhase) + wavetableSamples->size()) % wavetableSamples->size();
    return randPhase;
}
