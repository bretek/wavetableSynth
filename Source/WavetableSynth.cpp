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

    for (int i = 0; i < 3; ++i)
    {
        oscillators.emplace_back ();
    }

    oscillators[0].sampleRate = sampleRate;
    oscillators[1].sampleRate = sampleRate;
    oscillators[2].sampleRate = sampleRate;
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
}

void WavetableSynth::renderAudio (juce::AudioBuffer<float>& buffer, int startSample, int endSample)
{
    auto channel = buffer.getWritePointer (0);
    
    if (oscillators[0].isPlaying())
    {
        for (auto sample = startSample; sample <= endSample; ++sample)
        {
            channel[sample] += oscillators[0].getSample();
            channel[sample] += oscillators[1].getSample();
            channel[sample] += oscillators[2].getSample();
        }
    }
}

void WavetableSynth::handleMidi (juce::MidiMessage& message)
{
    if (message.isNoteOn())
    {
        oscillators[0].setFrequency (message.getMidiNoteInHertz(message.getNoteNumber()) - 20.f);
        oscillators[1].setFrequency (message.getMidiNoteInHertz(message.getNoteNumber()));
        oscillators[2].setFrequency (message.getMidiNoteInHertz(message.getNoteNumber()) + 20.f);
    }
    else if (message.isNoteOff())
    {
        oscillators[0].stop();
        oscillators[1].stop();
        oscillators[2].stop();
    }
}