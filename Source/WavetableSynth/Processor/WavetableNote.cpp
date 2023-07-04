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
    level = wavetableSynthParameters->levelParameter;
}

void WavetableNote::setFrequency (float frequency)
{
    this->frequency = frequency;

    updateUnison ();
}

void WavetableNote::setAmplitude (float amplitude)
{
    this->amplitude = amplitude;
}

void WavetableNote::updateUnison ()
{
    voices.clear();
    voiceFrequencyValues.clear();
    voiceAmplitudeValues.clear();
    voicePanValues.clear();

    // calculate detune frequencies
    if (*numVoices == 1)
    {
        voiceFrequencyValues.push_back (frequency);
    }
    else
    {
        float lowerFrequency = std::pow(2, -(*detune)/1200) * frequency;
        const float upperFrequency = std::pow(2, (*detune)/1200) * frequency;
        const float frequencyInterval = (upperFrequency - lowerFrequency) / static_cast<float>((*numVoices) - 1);

        for (int voiceNum = 0; voiceNum < (*numVoices); ++voiceNum)
        {
            voiceFrequencyValues.push_back (lowerFrequency);
            lowerFrequency += frequencyInterval;
        }
    }

    // calculate amplitudes
    for (int voiceNum = 0; voiceNum < (*numVoices); ++voiceNum)
    {
        voiceAmplitudeValues.push_back (*blend);
    }
    voiceAmplitudeValues[(voiceAmplitudeValues.size()/2)] = 1.f;
    if (voiceAmplitudeValues.size() % 2 == 0)
    {
        voiceAmplitudeValues[((voiceAmplitudeValues.size()/2) - 1)] = 1.f;
    }

    // calculate pan values
    if (*numVoices == 1)
    {
        voicePanValues.push_back (0.f);
    }
    else
    {
        float leftPan = -0.8f;
        float rightPan = 0.8f;
        float panInterval = (std::abs(leftPan) + std::abs(rightPan)) / static_cast<float>(*numVoices - 1);
        for (int voiceNum = 0; voiceNum < (*numVoices); ++voiceNum)
        {
            voicePanValues.push_back (leftPan);
            leftPan += panInterval;
        }
    }

    // create voices
    for (int voiceNum = 0; voiceNum < (*numVoices); ++voiceNum)
    {
        std::unique_ptr<WavetableOscillator> newOsc = std::make_unique<WavetableOscillator> ();
        newOsc->setFrequency (voiceFrequencyValues[voiceNum]);
        voices.push_back (*newOsc);
    }
}

void WavetableNote::renderAudio (juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    for (int voiceNum = 0; voiceNum < voices.size(); ++voiceNum)
    {
        if (voices[voiceNum].isPlaying())
        {
            const float panValue = voicePanValues[voiceNum];
            const float gainFactor = 1.f - std::abs(panValue);

            int destBuffer = 0;
            int sourceBuffer = 1;
            if (panValue < 0.f)
            {
                destBuffer = 1;
                sourceBuffer = 0;
            }

            auto* channel = buffer.getWritePointer (sourceBuffer);
            auto* pannedChannel = buffer.getWritePointer (destBuffer);

            for (int sample = startSample; sample <= endSample; ++sample)
            {
                const float nextSample = voices[voiceNum].getNextSample () * voiceAmplitudeValues[voiceNum] * (*level);
                channel[sample] += nextSample;
                pannedChannel[sample] += nextSample * gainFactor;
            }
        }
    }
}

void WavetableNote::start ()
{
    setFrequency (frequency);
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