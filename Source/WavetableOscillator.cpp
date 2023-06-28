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
    //initWavetableSin ();
    initWavetableSaw ();

    std::srand (std::time (nullptr));
    setRandomStartIndex();
}

void WavetableOscillator::initWavetableSin ()
{
    for (int sample = 0; sample < WAVETABLE_LENGTH; ++sample)
    {
        samples[sample] = std::sin ((((float)sample / WAVETABLE_LENGTH)) * juce::MathConstants<float>::twoPi);
    }
}

void WavetableOscillator::initWavetableSaw ()
{
    float currentSampleValue = 0;
    float sampleValueDifference = 1 / static_cast<float>(WAVETABLE_LENGTH-1);
    for (int sample = 0; sample < WAVETABLE_LENGTH; ++sample)
    {
        samples[sample] = currentSampleValue;
        currentSampleValue += sampleValueDifference;
        if (currentSampleValue > 1.f)
        {
            currentSampleValue -= 2.f;
        }
    }
}

void WavetableOscillator::setFrequency (float frequency)
{
    sampleIndexIncrement = getSampleIndexIncrement (frequency);
}

float WavetableOscillator::getSample ()
{
    const int floorIndex = std::fmod(currentSampleIndex, static_cast<float>(WAVETABLE_LENGTH));
    const float sample = ((currentSampleIndex - floorIndex) * samples[floorIndex + 1]) +
            ((floorIndex + 1 - currentSampleIndex) * samples[floorIndex]);
    currentSampleIndex += sampleIndexIncrement;

    if (currentSampleIndex >= WAVETABLE_LENGTH - 1)
    {
        currentSampleIndex -= static_cast<float>(WAVETABLE_LENGTH - 1);
    }

    return sample * 0.2f;
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
    float randPhase = (((std::rand() % WAVETABLE_LENGTH) * random) + (phase - (WAVETABLE_LENGTH * random) / 2));
    randPhase = (static_cast<int>(randPhase) + WAVETABLE_LENGTH) % WAVETABLE_LENGTH;
    currentSampleIndex = randPhase;
}