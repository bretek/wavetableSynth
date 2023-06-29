/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "WavetableSynth.h"

//==============================================================================
/**
*/
class SerumAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    SerumAudioProcessor();
    ~SerumAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    juce::AudioProcessorValueTreeState parameters;

    WavetableSynth wavetableSynth;

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

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SerumAudioProcessor)
};
