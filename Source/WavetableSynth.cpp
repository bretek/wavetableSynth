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

    notes.clear();

    for (int note = 0; note < NUM_NOTES; ++note)
    {
        WavetableNote* newNote = new WavetableNote();
        newNote->sampleRate = sampleRate;
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

void WavetableSynth::setUnison (std::atomic<float>* numVoices)
{
    //for (auto& note : notes)
    for (int i = 0; i < notes.size(); ++i)
    {
        notes[i].setNumVoices (numVoices);
    }
}

void WavetableSynth::setDetune (std::atomic<float>* detune)
{
    for (int i = 0; i < notes.size(); ++i)
    {
        notes[i].setDetune (detune);
    }
}

void WavetableSynth::setBlend (std::atomic<float>* blend)
{
    for (int i = 0; i < notes.size(); ++i)
    {
        notes[i].setBlend (blend);
    }
}

void WavetableSynth::setPhase (std::atomic<float>* phase)
{
    for (int i = 0; i < notes.size(); ++i)
    {
        notes[i].setPhase (phase);
    }
}

void WavetableSynth::setRandom (std::atomic<float>* random)
{
    for (int i = 0; i < notes.size(); ++i)
    {
        notes[i].setRandom (random);
    }
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

        frequency *= std::pow(2, (static_cast<float>(octaveOffset * 12) + static_cast<float>(semiOffset) + fineOffset + coarsePitch) / 12.f);

        notes[message.getNoteNumber()].setFrequency (frequency);
    }
    else if (message.isNoteOff())
    {
        notes[message.getNoteNumber()].stop();
    }
}