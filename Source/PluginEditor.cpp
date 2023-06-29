/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SerumAudioProcessorEditor::SerumAudioProcessorEditor (SerumAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p),
    valueTreeState (vts)
{
    this->setLookAndFeel (&serumLookAndFeel);
    setSize (540, 620);
    setResizable (true, true);

    osc1.attachUnisonControl (valueTreeState);
    osc1.attachDetuneControl (valueTreeState);
    osc1.attachBlendControl (valueTreeState);
    osc1.attachPhaseControl (valueTreeState);
    osc1.attachRandomControl (valueTreeState);
    osc1.attachPanControl (valueTreeState);
    osc1.attachLevelControl (valueTreeState);

    addAndMakeVisible (osc1);
}

SerumAudioProcessorEditor::~SerumAudioProcessorEditor()
{
    this->setLookAndFeel (nullptr);
}

//==============================================================================
void SerumAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void SerumAudioProcessorEditor::resized()
{
    juce::FlexBox main;

    main.items.add (juce::FlexItem (osc1).withFlex (1.0f).withMargin (10.0f));

    main.performLayout (getLocalBounds());
}
