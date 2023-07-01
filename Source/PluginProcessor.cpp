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
            std::make_unique<juce::AudioParameterInt> ("octave",
                                                        "Octave",
                                                        -4, 4,
                                                        0),
            std::make_unique<juce::AudioParameterInt> ("semi",
                                                        "Semi",
                                                        -24, 24,
                                                        0),
            std::make_unique<juce::AudioParameterFloat> ("fine",
                                                        "Fine",
                                                        -1.f, 1.f,
                                                        0),
            std::make_unique<juce::AudioParameterFloat> ("coarse",
                                                        "Coarse",
                                                        -100.f, 100.f,
                                                        0),

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
    extern struct WavetableSynthParameters wavetableSynthParametersExt;
    this->wavetableSynthParameters = &wavetableSynthParametersExt;

    wavetableSynthParameters->octaveParameter = parameters.getRawParameterValue ("octave");
    wavetableSynthParameters->semiParameter = parameters.getRawParameterValue ("semi");
    wavetableSynthParameters->fineParameter = parameters.getRawParameterValue ("fine");
    wavetableSynthParameters->coarseParameter = parameters.getRawParameterValue ("coarse");

    wavetableSynthParameters->unisonParameter = parameters.getRawParameterValue ("unison");
    wavetableSynthParameters->detuneParameter = parameters.getRawParameterValue ("detune");
    wavetableSynthParameters->blendParameter = parameters.getRawParameterValue ("blend");
    wavetableSynthParameters->phaseParameter = parameters.getRawParameterValue ("phase");
    wavetableSynthParameters->randomParameter = parameters.getRawParameterValue ("random");
    wavetableSynthParameters->panParameter = parameters.getRawParameterValue ("pan");
    wavetableSynthParameters->levelParameter = parameters.getRawParameterValue ("level");

    wavetableSynth.setUnison (wavetableSynthParameters->unisonParameter);
    wavetableSynth.setDetune (wavetableSynthParameters->detuneParameter);
    wavetableSynth.setBlend (wavetableSynthParameters->blendParameter);
    wavetableSynth.setPan (wavetableSynthParameters->panParameter);
    wavetableSynth.setLevel (wavetableSynthParameters->levelParameter);
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

    wavetableSynth.setPitchControls (wavetableSynthParameters->octaveParameter, 
                                    wavetableSynthParameters->semiParameter, 
                                    wavetableSynthParameters->fineParameter, 
                                    wavetableSynthParameters->coarseParameter);

    wavetableSynth.setUnison (wavetableSynthParameters->unisonParameter);
    wavetableSynth.setDetune (wavetableSynthParameters->detuneParameter);
    wavetableSynth.setBlend (wavetableSynthParameters->blendParameter);
    wavetableSynth.setPan (wavetableSynthParameters->panParameter);
    wavetableSynth.setLevel (wavetableSynthParameters->levelParameter);
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
    return new SerumAudioProcessorEditor (*this, parameters, &(wavetableSynth.wavetableSamples));
}

//==============================================================================
void SerumAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void SerumAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
    {
        if (xmlState.get()->hasTagName (parameters.state.getType()))
        {
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SerumAudioProcessor();
}
