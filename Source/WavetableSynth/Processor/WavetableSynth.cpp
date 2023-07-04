/*
  ==============================================================================

    WavetableSynth.cpp
    Created: 25 Jun 2023 11:15:27pm
    Author:  joseph

  ==============================================================================
*/

#include "WavetableSynth.h"

WavetableSynth::WavetableSynth ()
{
    oversampling.reset (new juce::dsp::Oversampling<float> (2, OVERSAMPLING_FACTOR, juce::dsp::Oversampling<float>::filterHalfBandFIREquiripple, false));
}

void WavetableSynth::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate * OVERSAMPLING_RATIO;

    oversampling->initProcessing (static_cast<size_t> (samplesPerBlock));

    initWavetableSaw ();
    extern struct WavetableSynthParameters wavetableSynthParametersExt;
    wavetableSynthParameters = &wavetableSynthParametersExt;
    wavetableSynthParameters->wavetableSamples = &wavetableSamples;
    wavetableSynthParameters->sampleRate = &this->sampleRate;

    octaveOffset = wavetableSynthParameters->octaveParameter;
    semiOffset = wavetableSynthParameters->semiParameter;
    fineOffset = wavetableSynthParameters->fineParameter;
    coarsePitch = wavetableSynthParameters->coarseParameter;

    pan = wavetableSynthParameters->panParameter;
    level = wavetableSynthParameters->levelParameter;

    notes.clear();

    for (int note = 0; note < NUM_NOTES; ++note)
    {
        std::unique_ptr<WavetableNote> newNote (new WavetableNote());
        notes.push_back (*newNote);
    }
}

void WavetableSynth::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples ();

    int currentSample = 0;

    juce::dsp::AudioBlock<float> targetBlock = juce::dsp::AudioBlock<float> (buffer);
    juce::dsp::AudioBlock<float> upsampledBlock = oversampling->processSamplesUp (targetBlock);

    for (const auto midiData : midiMessages)
    {
        auto midiMessage = midiData.getMessage();
        int midiTime = midiMessage.getTimeStamp() * OVERSAMPLING_RATIO;
        renderAudio (upsampledBlock, currentSample*OVERSAMPLING_RATIO, midiTime);
        currentSample = midiTime;
        handleMidi (midiMessage);
    }

    renderAudio (upsampledBlock, currentSample*OVERSAMPLING_RATIO, numSamples*OVERSAMPLING_RATIO);

    oversampling->processSamplesDown (targetBlock);

    panAudio (buffer);
}

void WavetableSynth::renderAudio (juce::dsp::AudioBlock<float>& buffer, int startSample, int endSample)
{
    for (int i = 0; i < notes.size(); ++i)
    {
        if (notes[i].isPlaying())
        {
            notes[i].renderAudio (buffer, startSample, endSample);
        }
    }
}

void WavetableSynth::panAudio (juce::AudioBuffer<float>& buffer)
{
    const float gainFactor = 1.f - std::abs(*pan);

    int destBuffer = 0;
    int sourceBuffer = 1;
    if (*pan < 0.f)
    {
        destBuffer = 1;
        sourceBuffer = 0;
    }

    buffer.copyFrom (destBuffer, 0, buffer.getWritePointer (sourceBuffer), buffer.getNumSamples(), gainFactor);
}

void WavetableSynth::handleMidi (juce::MidiMessage& message)
{
    if (message.isNoteOn())
    {
        notes[message.getNoteNumber()].setAmplitude (message.getVelocity() / 127.f);
        float frequency = message.getMidiNoteInHertz(message.getNoteNumber());

        frequency *= std::pow(2, (static_cast<float>((*octaveOffset) * 12) + static_cast<float>((*semiOffset)) + (*fineOffset) + (*coarsePitch)) / 12.f);

        notes[message.getNoteNumber()].setFrequency (frequency);
        notes[message.getNoteNumber()].start();
    }
    else if (message.isNoteOff())
    {
        notes[message.getNoteNumber()].stop();
    }
}

void WavetableSynth::initWavetableSin ()
{
    wavetableSamples.resize (WAVETABLE_LENGTH);
    for (int sample = 0; sample < WAVETABLE_LENGTH; ++sample)
    {
        wavetableSamples[sample] = std::sin ((((float)sample / WAVETABLE_LENGTH)) * juce::MathConstants<float>::twoPi);
    }
}

void WavetableSynth::initWavetableSaw ()
{
    wavetableSamples.resize (WAVETABLE_LENGTH);
    float currentSampleValue = 0;
    float sampleValueDifference = 2.f / static_cast<float>(WAVETABLE_LENGTH-1);
    for (int sample = 0; sample < WAVETABLE_LENGTH; ++sample)
    {
        wavetableSamples[sample] = currentSampleValue;
        currentSampleValue += sampleValueDifference;
        if (currentSampleValue > 1.f)
        {
            currentSampleValue -= 2.f;
        }
    }
}

void WavetableSynth::initWavetableSquare ()
{
    wavetableSamples.resize (WAVETABLE_LENGTH);
    for (int sample = 0; sample < WAVETABLE_LENGTH / 2; ++sample)
    {
        wavetableSamples[sample] = 1.f;
    }
    for (int sample = WAVETABLE_LENGTH / 2; sample < WAVETABLE_LENGTH; ++sample)
    {
        wavetableSamples[sample] = -1.f;
    }
}