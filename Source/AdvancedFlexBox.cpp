/*
  ==============================================================================

    AdvancedFlexBox.cpp
    Created: 21 Jun 2023 1:45:43pm
    Author:  joseph

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdvancedFlexBox.h"

//==============================================================================
AdvancedFlexBox::AdvancedFlexBox()
{
    
}

AdvancedFlexBox::~AdvancedFlexBox()
{
}

void AdvancedFlexBox::paint (juce::Graphics& g)
{
    g.setColour (backgroundColour);
    g.fillRoundedRectangle (0, 0, getWidth(), getHeight(), 10.0f);
}

void AdvancedFlexBox::resized()
{
    flexBox.performLayout (getLocalBounds());
}
