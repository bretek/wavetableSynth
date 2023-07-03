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

}

void WavetableSynth::prepareToPlay (double sampleRate)
{
    this->sampleRate = sampleRate;

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
        WavetableNote* newNote = new WavetableNote();
        notes.push_back (*newNote);
    }
}

void WavetableSynth::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    int numSamples = buffer.getNumSamples ();

    int currentSample = 0;

    for (const auto midiData : midiMessages)
    {
        auto midiMessage = midiData.getMessage();
        int midiTime = midiMessage.getTimeStamp();
        renderAudio (buffer, currentSample, currentSample + midiTime);
        currentSample = midiTime;
        handleMidi (midiMessage);
    }

    renderAudio (buffer, currentSample, numSamples);

    auto* leftChannel = buffer.getWritePointer (0);
    auto* rightChannel = buffer.getWritePointer (1);

    float panFactor = 1.f - std::abs(*pan);

    auto* affectedChannel = rightChannel;
    if (*pan > 0.f)
    {
        affectedChannel = leftChannel;
    }

    for (int sample = 0; sample < numSamples; ++sample)
    {
        affectedChannel[sample] *= panFactor;
    }
}

void WavetableSynth::renderAudio (juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    auto channel = buffer.getWritePointer (0);
    
    for (int i = 0; i < notes.size(); ++i)
    {
        if (notes[i].isPlaying())
        {
            notes[i].renderAudio (buffer, startSample, endSample);
        }
    }
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