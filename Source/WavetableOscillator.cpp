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
}

void WavetableOscillator::setFrequency (float frequency)
{
    sampleIndexIncrement = getSampleIndexIncrement (frequency);
}

void WavetableOscillator::setAmplitude (float amplitude)
{
    this->amplitude = amplitude;
}

void WavetableOscillator::setPhase (std::atomic<float>* phase)
{
    this->phase = phase;
}

void WavetableOscillator::setRandom (std::atomic<float>* random)
{
    this->random = random;
}

void WavetableOscillator::setWavetable (std::vector<float>* wavetableSamples)
{
    samples = wavetableSamples;
}

float WavetableOscillator::getSample ()
{
    const int floorIndex = std::fmod(currentSampleIndex, static_cast<float>(WAVETABLE_LENGTH));
    const float sample = ((currentSampleIndex - floorIndex) * (*samples)[floorIndex + 1]) +
            ((floorIndex + 1 - currentSampleIndex) * (*samples)[floorIndex]);
    currentSampleIndex += sampleIndexIncrement;

    if (currentSampleIndex >= WAVETABLE_LENGTH - 1)
    {
        currentSampleIndex -= static_cast<float>(WAVETABLE_LENGTH - 1);
    }

    return sample * amplitude;
}

bool WavetableOscillator::isPlaying ()
{
    return sampleIndexIncrement != 0.f;
}

void WavetableOscillator::stop ()
{
    setRandomStartIndex ();
    sampleIndexIncrement = 0.f;
}

float WavetableOscillator::getSampleIndexIncrement (float frequency)
{
    return (frequency * static_cast<float>(WAVETABLE_LENGTH)) / (float)sampleRate;
}

void WavetableOscillator::setRandomStartIndex ()
{
    currentSampleIndex = calculateRandomStartSample ((*phase), (*random));
}

float WavetableOscillator::calculateRandomStartSample (float phase, float random)
{
    float randPhase = (((std::rand() % WAVETABLE_LENGTH) * random) + ((WAVETABLE_LENGTH * (phase - random)) / 2));
    randPhase = (static_cast<int>(randPhase) + WAVETABLE_LENGTH) % WAVETABLE_LENGTH;
    return randPhase;
}