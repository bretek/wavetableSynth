/*
  ==============================================================================

    WavetableVisualiser.cpp
    Created: 30 Jun 2023 4:40:58pm
    Author:  joseph

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WavetableVisualiser.h"

//==============================================================================
WavetableVisualiser::WavetableVisualiser()
{
    this->setLookAndFeel (&serumLookAndFeel);
}

WavetableVisualiser::~WavetableVisualiser()
{
    this->setLookAndFeel (nullptr);
}

void WavetableVisualiser::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (serumLookAndFeel.SerumGrey5));   // clear the background

    // draw gradient
    for (int sampleNum = 0; sampleNum < samples->size(); ++sampleNum)
    {
        juce::Point<int> pixelPoint = sampleToPixelPoint ((*samples)[sampleNum] * -1.f, sampleNum);
        juce::Point<float> basePoint (pixelPoint.getX(), getHeight() / 2);
        g.setGradientFill (juce::ColourGradient ( getLookAndFeel().findColour (serumLookAndFeel.SerumGreen2),
                                            basePoint, 
                                            getLookAndFeel().findColour (serumLookAndFeel.SerumGreen3),
                                            juce::Point<float>(pixelPoint.getX(), pixelPoint.getY()),
                                            false));
        g.drawLine (basePoint.getX(), basePoint.getY(), pixelPoint.getX(), pixelPoint.getY(), 1);
    }

    // draw waveform
    g.setColour (getLookAndFeel().findColour (serumLookAndFeel.SerumGreen1));
    for (int sampleNum = 0; sampleNum < samples->size() - 1; ++sampleNum)
    {
        juce::Point<int> pixelPoint1 = sampleToPixelPoint ((*samples)[sampleNum] * -1.f, sampleNum);
        juce::Point<int> pixelPoint2 = sampleToPixelPoint ((*samples)[sampleNum+1] * -1.f, sampleNum+1);
        g.drawLine (pixelPoint1.getX(), pixelPoint1.getY(), pixelPoint2.getX(), pixelPoint2.getY(), 2);
    }
}

void WavetableVisualiser::resized()
{
}

void WavetableVisualiser::setWavetable (std::vector<float>* wavetableSamples)
{
    samples = wavetableSamples;
    repaint ();
}

juce::Point<int> WavetableVisualiser::sampleToPixelPoint (float sampleValue, int sampleNum)
{
    int x = 2.5f + ((static_cast<float>(getWidth() - 5) / static_cast<float>(samples->size())) * static_cast<float>(sampleNum));
    int y = (static_cast<float>(getHeight()) / 2.f) + ((static_cast<float>(getHeight() - 5) / 2.f) * sampleValue);
    return juce::Point<int> (x, y);
}