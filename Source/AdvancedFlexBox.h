/*
  ==============================================================================

    AdvancedFlexBox.h
    Created: 21 Jun 2023 1:45:43pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AdvancedFlexBox  : public juce::Component
{
public:
    AdvancedFlexBox();
    ~AdvancedFlexBox() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Colour backgroundColour;
    juce::FlexBox flexBox;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdvancedFlexBox)
};
