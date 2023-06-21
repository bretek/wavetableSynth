/*
  ==============================================================================

    WtComponent.h
    Created: 21 Jun 2023 12:54:36am
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SerumLookAndFeel.h"
#include "AdvancedSlider.h"

//==============================================================================
/*
*/
class WtComponent  : public juce::Component
{
public:
    WtComponent();
    ~WtComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // looks and feels
    SerumLookAndFeel serumLookAndFeel;
    SerumRotarySliderDial serumDial;
    SerumSliderBox2 serumSliderBox;

    juce::TextButton wtSpacer;
    juce::TextButton wtFile;
    juce::TextButton wtLastFile;
    juce::TextButton wtNextFile;

    AdvancedSlider octaveSlide;
    AdvancedSlider semiSlide;
    AdvancedSlider fineSlide;
    AdvancedSlider coarseSlide;

    juce::TextButton wtEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WtComponent)
};
