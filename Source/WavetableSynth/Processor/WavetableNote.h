/*
  ==============================================================================

    WavetableNote.h
    Created: 28 Jun 2023 4:54:42pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <vector>

#include "WavetableOscillator.h"
#include "../WavetableSynthParameters.h"

class WavetableNote
{
public:
    WavetableNote ();

    void setFrequency (float frequency);
    void setAmplitude (float amplitude);

    void updateUnison ();

    void renderAudio (juce::dsp::AudioBlock<float>& buffer, int startSample, int endSample);

    bool isPlaying () const;
    void start ();
    void stop ();
private:
    WavetableSynthParameters* wavetableSynthParameters;

    double* sampleRate;

    std::atomic<float>* numVoices;
    std::atomic<float>* detune;
    std::atomic<float>* blend;
    std::atomic<float>* level;

    std::vector<WavetableOscillator> voices;

    std::vector<float> voiceFrequencyValues;
    std::vector<float> voiceAmplitudeValues;
    std::vector<float> voicePanValues;

    float frequency = 0.f;
    float amplitude;

    bool playing = false;
};