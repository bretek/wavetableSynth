/*
  ==============================================================================

    WavetableVisualiser.h
    Created: 30 Jun 2023 4:40:58pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "../../SerumLookAndFeel.h"

//==============================================================================
/*
*/
class WavetableVisualiser  : public juce::Component
{
public:
    WavetableVisualiser();
    ~WavetableVisualiser() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void setWavetable (std::vector<float>* wavetableSamples);

private:
    juce::Point<int> sampleToPixelPoint (float sampleValue, int sampleNum);

    std::vector<float>* samples;

    SerumLookAndFeel serumLookAndFeel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableVisualiser)
};
