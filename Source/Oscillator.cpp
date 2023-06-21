/*
  ==============================================================================

    Oscillator.cpp
    Created: 20 Jun 2023 11:41:27pm
    Author:  joseph

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator()
{
    this->setLookAndFeel (&serumLookAndFeel);

    addAndMakeVisible (&oscToggle);

    titleLabel.setText ("OSC A", juce::NotificationType::dontSendNotification);
    titleLabel.justification = juce::Justification::left;
    addAndMakeVisible (&titleLabel);

    addAndMakeVisible (&wtComponent);

    std::vector<ControlKnob*> controls = { &unisonControl, &detuneControl, &blendControl, &phaseControl, &randControl, &wtControl, &morphControl, &panControl, &levelControl };

    unisonControl.setText ("UNISON");
    unisonControl.knob.setRange (1, 16, 1.0f);
    unisonControl.label.closedRight = true;

    detuneControl.setText ("DETUNE");
    detuneControl.label.closedLeft = true;
    detuneControl.label.closedRight = true;

    blendControl.setText ("BLEND");
    blendControl.label.closedLeft = true;

    phaseControl.setText ("PHASE");
    phaseControl.label.closedRight = true;

    randControl.setText ("RAND");
    randControl.label.closedLeft = true;

    wtControl.setText ("WT POS");
    wtControl.label.closedRight = true;

    morphControl.setText ("OFF");
    morphControl.label.closedLeft = true;

    panControl.setText ("PAN");
    panControl.label.closedRight = true;

    levelControl.setText ("LEVEL");
    levelControl.label.closedLeft = true;

    for (auto control : controls)
    {
        control->setLookAndFeel (&serumDial);
    }
    unisonControl.setLookAndFeel (&serumSliderBox);

    for (auto control : controls)
    {
        addAndMakeVisible (control);
    }
}

Oscillator::~Oscillator()
{
    this->setLookAndFeel (nullptr);
}

void Oscillator::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void Oscillator::resized()
{
    juce::FlexBox verticalLayout;
    verticalLayout.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    verticalLayout.flexDirection = juce::FlexBox::Direction::column;

    // Title bar
    AdvancedFlexBox titleRow;
    titleRow.flexBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    //titleRow.backgroundColour = juce::Colour (99, 108, 114);
    titleRow.backgroundColour = juce::Colour (255, 255, 255);

    titleRow.flexBox.items.add (juce::FlexItem (oscToggle).withFlex (1.0f).withMargin (juce::FlexItem::Margin (10.0f)));
    titleRow.flexBox.items.add (juce::FlexItem (titleLabel).withFlex (20.0f).withMargin (juce::FlexItem::Margin (10.0f)));

    titleRow.setBounds (getLocalBounds());

    // Osc controls
    juce::FlexBox topRow;
    topRow.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    topRow.items.add (juce::FlexItem (unisonControl).withFlex (1.0f));
    topRow.items.add (juce::FlexItem (detuneControl).withFlex (1.0f));
    topRow.items.add (juce::FlexItem (blendControl).withFlex (1.0f));
    topRow.items.add (juce::FlexItem ().withFlex (0.3f));
    topRow.items.add (juce::FlexItem (phaseControl).withFlex (1.0f));
    topRow.items.add (juce::FlexItem (randControl).withFlex (1.0f));

    topRow.performLayout (getLocalBounds());

    juce::FlexBox bottomRow;
    bottomRow.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    bottomRow.items.add (juce::FlexItem (wtControl).withFlex (1.0f));
    bottomRow.items.add (juce::FlexItem (morphControl).withFlex (2.0f));
    bottomRow.items.add (juce::FlexItem ().withFlex (0.3f));
    bottomRow.items.add (juce::FlexItem (panControl).withFlex (1.0f));
    bottomRow.items.add (juce::FlexItem (levelControl).withFlex (1.0f));

    bottomRow.performLayout (getLocalBounds());

    verticalLayout.items.add (juce::FlexItem (titleRow).withFlex (1.7f));
    verticalLayout.items.add (juce::FlexItem (wtComponent).withFlex (10.0f));
    verticalLayout.items.add (juce::FlexItem  (topRow).withFlex (4.0f));
    verticalLayout.items.add (juce::FlexItem  (bottomRow).withFlex (4.0f));

    verticalLayout.performLayout (getLocalBounds());
}
