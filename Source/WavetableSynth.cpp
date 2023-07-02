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
    auto* channel1 = buffer.getWritePointer (0);
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

    for (auto channel = 1; channel < buffer.getNumChannels(); ++channel)
    {
        std::copy (channel1, channel1 + numSamples, buffer.getWritePointer (channel));
    }

    if ((*pan) != 0.5f)
    {
        float gainFactor = 1.f - (2 * std::abs(0.5f - (*pan)));
        int affectedChannel = 1;
        if ((*pan) > 0.5f)
        {
            affectedChannel = 0;
        }
        auto* channel = buffer.getWritePointer (affectedChannel);

        for (auto sample = 0; sample <= numSamples; ++sample)
        {
            channel[sample] *= gainFactor;
        }
    }
}

void WavetableSynth::renderAudio (juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    auto channel = buffer.getWritePointer (0);
    
    for (int i = 0; i < notes.size(); ++i)
    {
        if (notes[i].isPlaying())
        {
            for (auto sample = startSample; sample <= endSample; ++sample)
            {
                channel[sample] += notes[i].getSample() * (*level);
            }
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