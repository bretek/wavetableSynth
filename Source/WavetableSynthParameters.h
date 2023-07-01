/*
  ==============================================================================

    WavetableSynthParameters.h
    Created: 1 Jul 2023 4:58:31pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct WavetableSynthParameters
{
    std::atomic<float>* octaveParameter = nullptr;
    std::atomic<float>* semiParameter = nullptr;
    std::atomic<float>* fineParameter = nullptr;
    std::atomic<float>* coarseParameter = nullptr;

    std::atomic<float>* unisonParameter = nullptr;
    std::atomic<float>* detuneParameter = nullptr;
    std::atomic<float>* blendParameter = nullptr;
    std::atomic<float>* phaseParameter = nullptr;
    std::atomic<float>* randomParameter = nullptr;
    std::atomic<float>* panParameter = nullptr;
    std::atomic<float>* levelParameter = nullptr;

    std::vector<float>* wavetableSamples;

    double* sampleRate;
};