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

float WavetableOscillator::getNextSample ()
{
    const float sample = sincInterpolateSamples (currentSampleIndex);

    currentSampleIndex += sampleIndexIncrement;

    // loop back to beginning
    if (currentSampleIndex >= wavetableSamples->size() - 1)
    {
        currentSampleIndex -= static_cast<float>(wavetableSamples->size() - 1);
    }

    return sample;
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

inline float WavetableOscillator::lanczosKernel (float x) const
{
    static const float piSquared = std::pow(juce::MathConstants<float>::pi, 2);
    if (x == 0)
    {
        return 1.f;
    }
    if (x >= -SINC_KERNEL_SIZE && x < SINC_KERNEL_SIZE)
    {
        float top = SINC_KERNEL_SIZE * std::sin (juce::MathConstants<float>::pi * x) * std::sin ((juce::MathConstants<float>::pi * x) / SINC_KERNEL_SIZE);
        float bottom = piSquared * std::pow(x, 2);
        return (top / bottom);
    }
    else
    {
        return 0.f;
    }
}

inline float WavetableOscillator::sincInterpolateSamples (float index) const
{
    float sampleValue = 0;

    int floorX = std::floor(index);

    for (int i = floorX - SINC_KERNEL_SIZE + 1; i <= floorX + SINC_KERNEL_SIZE; ++i)
    {
        int sampleIndex = (i + wavetableSamples->size()) % wavetableSamples->size();
        float sincVal = (*wavetableSamples)[sampleIndex] * lanczosKernel (index - i);
        sampleValue += sincVal;
    }

    return sampleValue;
}

inline float WavetableOscillator::calculateSampleIndexIncrement (float frequency) const
{
    return (frequency * static_cast<float>(wavetableSamples->size())) / (float)(*sampleRate);
}

inline float WavetableOscillator::calculateRandomStartSample (float phase, float random) const
{
    float randPhase = (((std::rand() % wavetableSamples->size()) * random) + ((wavetableSamples->size() * (phase - random)) / 2));
    randPhase = (static_cast<int>(randPhase) + wavetableSamples->size()) % wavetableSamples->size();
    return randPhase;
}
