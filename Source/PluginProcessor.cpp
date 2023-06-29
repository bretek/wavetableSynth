/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SerumAudioProcessor::SerumAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
    parameters (*this, nullptr, juce::Identifier ("SerumControls"),
        {
            std::make_unique<juce::AudioParameterInt> ("unison",
                                                        "Unison",
                                                        1, 16,
                                                        1),
            std::make_unique<juce::AudioParameterFloat> ("detune",
                                                        "Detune",
                                                        0.f, 1.f,
                                                        0.5f),
            std::make_unique<juce::AudioParameterFloat> ("blend",
                                                        "Blend",
                                                        0.f, 1.f,
                                                        0.5f),
            std::make_unique<juce::AudioParameterFloat> ("phase",
                                                        "Phase",
                                                        0.f, 1.f,
                                                        0.f),
            std::make_unique<juce::AudioParameterFloat> ("random",
                                                        "Random",
                                                        0.f, 1.f,
                                                        1.f),
            std::make_unique<juce::AudioParameterFloat> ("pan",
                                                        "Pan",
                                                        0.f, 1.f,
                                                        0.5f),
            std::make_unique<juce::AudioParameterFloat> ("level",
                                                        "Level",
                                                        0.f, 1.f,
                                                        0.8f)
        })
{
    unisonParameter = parameters.getRawParameterValue ("unison");
    detuneParameter = parameters.getRawParameterValue ("detune");
    blendParameter = parameters.getRawParameterValue ("blend");
    phaseParameter = parameters.getRawParameterValue ("phase");
    randomParameter = parameters.getRawParameterValue ("random");
    panParameter = parameters.getRawParameterValue ("pan");
    levelParameter = parameters.getRawParameterValue ("level");

    wavetableSynth.setUnison (unisonParameter);
    wavetableSynth.setDetune (detuneParameter);
    wavetableSynth.setBlend (blendParameter);
    wavetableSynth.setPhase (phaseParameter);
    wavetableSynth.setRandom (randomParameter);
    wavetableSynth.setPan (panParameter);
    wavetableSynth.setLevel (levelParameter);
}

SerumAudioProcessor::~SerumAudioProcessor()
{
}

//==============================================================================
const juce::String SerumAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SerumAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SerumAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SerumAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SerumAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SerumAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SerumAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SerumAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SerumAudioProcessor::getProgramName (int index)
{
    return {};
}

void SerumAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SerumAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    wavetableSynth.prepareToPlay (sampleRate);
}

void SerumAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SerumAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SerumAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    wavetableSynth.processBlock (buffer, midiMessages);
}

//==============================================================================
bool SerumAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SerumAudioProcessor::createEditor()
{
    return new SerumAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void SerumAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SerumAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SerumAudioProcessor();
}
