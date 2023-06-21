/*
  ==============================================================================

    WtComponent.cpp
    Created: 21 Jun 2023 12:54:36am
    Author:  joseph

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WtComponent.h"

//==============================================================================
WtComponent::WtComponent()
{
    // wavetable file navigation
    addAndMakeVisible (&wtSpacer);
    wtFile.setButtonText ("Default");
    addAndMakeVisible (&wtFile);
    wtLastFile.setButtonText ("<");
    addAndMakeVisible (&wtLastFile);
    wtNextFile.setButtonText (">");
    addAndMakeVisible (&wtNextFile);

    // wavetable pitch controls
    std::vector<AdvancedSlider*> pitchControls = { &octaveSlide, &semiSlide, &fineSlide, &coarseSlide };

    octaveSlide.setRange (-4, 4, 1.0);
    semiSlide.setRange (-24, 24, 1.0);
    fineSlide.setRange (-5, 5, 0.01);
    coarseSlide.setRange (-100, 100, 0.01);

    for (auto control : pitchControls)
    {
        control->setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
        control->setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        control->setLookAndFeel (&serumSliderBox);
        addAndMakeVisible (control);
    }

    // wavetable editor
    wtEditor.setSize (500, 300);
    wtEditor.setColour (juce::ResizableWindow::backgroundColourId, juce::Colours::red);
    addAndMakeVisible (&wtEditor);
}

WtComponent::~WtComponent()
{
}

void WtComponent::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::black);
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 10.0f);
}

void WtComponent::resized()
{
    auto area = getLocalBounds();

    // Wavetable
    juce::FlexBox wt;
    wt.flexDirection = juce::FlexBox::Direction::column;

    juce::FlexBox fileSelectControls;
    fileSelectControls.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    fileSelectControls.items.add (juce::FlexItem (wtSpacer).withFlex (1.0f).withMargin (juce::FlexItem::Margin (1.0f)));
    fileSelectControls.items.add (juce::FlexItem (wtFile).withFlex (10.0f).withMargin (juce::FlexItem::Margin (1.0f)));
    fileSelectControls.items.add (juce::FlexItem (wtLastFile).withFlex (0.7f).withMargin (juce::FlexItem::Margin (1.0f)));
    fileSelectControls.items.add (juce::FlexItem (wtNextFile).withFlex (0.7f).withMargin (juce::FlexItem::Margin (1.0f)));

    fileSelectControls.performLayout (getLocalBounds());

    juce::FlexBox pitchControls;
    pitchControls.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;

    pitchControls.items.add (juce::FlexItem (octaveSlide).withFlex (1.0f));
    pitchControls.items.add (juce::FlexItem (semiSlide).withFlex (1.0f));
    pitchControls.items.add (juce::FlexItem (fineSlide).withFlex (1.0f));
    pitchControls.items.add (juce::FlexItem (coarseSlide).withFlex (1.0f));

    pitchControls.performLayout (getLocalBounds());

    wt.items.add (juce::FlexItem (fileSelectControls).withFlex (1.0f));
    wt.items.add (juce::FlexItem (pitchControls).withFlex (1.0f));
    wt.items.add (juce::FlexItem (wtEditor).withFlex (5.0f));

    wt.performLayout (getLocalBounds().reduced (2.0f));
}
