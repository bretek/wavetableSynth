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
    oscillator.sampleRate = sampleRate;
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
    
    if (oscillator.isPlaying())
    {
        for (auto sample = startSample; sample <= endSample; ++sample)
        {
            channel[sample] += oscillator.getSample();
        }
    }
}

void WavetableSynth::handleMidi (juce::MidiMessage& message)
{
    if (message.isNoteOn())
    {
        oscillator.setFrequency (message.getMidiNoteInHertz(message.getNoteNumber()));
    }
    else if (message.isNoteOff())
    {
        oscillator.stop();
    }
}

void WavetableSynth::setUnison (int unison)
{
    this->unison = unison;
    blendAmp = 1.f / static_cast<float>(unison);
    unisonLowTune = frequency - ((unison - 1.f) / 2.f);
}