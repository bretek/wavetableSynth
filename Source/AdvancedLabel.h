/*
  ==============================================================================

    AdvancedLabel.h
    Created: 19 Jun 2023 6:44:12pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class AdvancedLabel : public juce::Label
{
public:
    bool closedLeft = false;
    bool closedRight = false;

    juce::Justification justification = juce::Justification::centred;
};