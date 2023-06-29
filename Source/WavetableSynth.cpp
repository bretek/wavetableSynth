/*
  ==============================================================================

    WavetableSynth.cpp
    Created: 25 Jun 2023 11:15:27pm
    Author:  joseph

  ==============================================================================
*/

#include "WavetableSynth.h"

void WavetableSynth::prepareToPlay (double sampleRate)
{
    this->sampleRate = sampleRate;

    note.sampleRate = sampleRate;
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

void WavetableSynth::setUnison (std::atomic<float>* numVoices)
{
    note.setNumVoices (numVoices);
}

void WavetableSynth::setDetune (std::atomic<float>* detune)
{
    note.setDetune (detune);
}

void WavetableSynth::setBlend (std::atomic<float>* blend)
{
    note.setBlend (blend);
}

void WavetableSynth::setPhase (std::atomic<float>* phase)
{
    note.setPhase (phase);
}

void WavetableSynth::setRandom (std::atomic<float>* random)
{
    note.setRandom (random);
}

void WavetableSynth::setPan (std::atomic<float>* pan)
{
    this->pan = pan;
}

void WavetableSynth::setLevel (std::atomic<float>* level)
{
    this->level = level;
}

void WavetableSynth::renderAudio (juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    auto channel = buffer.getWritePointer (0);
    
    if (note.isPlaying())
    {
        for (auto sample = startSample; sample <= endSample; ++sample)
        {
            channel[sample] += note.getSample() * (*level);
        }
    }
}

void WavetableSynth::handleMidi (juce::MidiMessage& message)
{
    if (message.isNoteOn())
    {
        note.setAmplitude (message.getVelocity() / 127.f);
        float frequency = message.getMidiNoteInHertz(message.getNoteNumber());

        frequency *= std::pow(2, (static_cast<float>(octaveOffset * 12) + static_cast<float>(semiOffset) + fineOffset + coarsePitch) / 12.f);

        note.setFrequency (frequency);
    }
    else if (message.isNoteOff())
    {
        note.stop();
    }
}