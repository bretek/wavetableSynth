/*
  ==============================================================================

    SerumLookAndFeel.h
    Created: 15 Jun 2023 11:24:26pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "AdvancedLabel.h"

class SerumLookAndFeel : public juce::LookAndFeel_V4
{
public:
    juce::Colour grey1 = {133, 146, 154};
    juce::Colour grey2 = {99, 108, 114};
    juce::Colour grey3 = {50, 55, 60};
    juce::Colour grey4 = {35, 40, 42};

    juce::Colour serumBlue = {2,145,197};

    float roundedRectangleFactor1 = 10.0f;
    float troundedRectangleFactor2 = 30.0f;

    SerumLookAndFeel ();
    
    void drawLabel (juce::Graphics& g, juce::Label& label) override;
    void drawBasicLabel (juce::Graphics& g, juce::Label& label);
    void drawAdvancedLabel (juce::Graphics& g, AdvancedLabel& label);

    void drawButtonBackground (juce::Graphics& g, juce::Button& button, 
                    const juce::Colour& backgroundColour, 
                    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    static const juce::Font getCustomFont();

    void drawDialMarkers (juce::Graphics& g, int numMarkers, 
                          float outerDiameter, float innerDiameter, 
                          float centerX, float centerY, 
                          float startAngle, float endAngle,
                          juce::Colour colour);

    void drawRing (juce::Graphics& g, 
                  float outerDiameter, float innerDiameter, 
                  float centerX, float centerY,
                  float startAngle, float endAngle,
                  juce::ColourGradient colourGradient);

    void drawDial (juce::Graphics& g, 
                  float outerDiameter, float innerDiameter, 
                  float centerX, float centerY,
                  float angle, float width,
                  juce::Colour colour);
};

class SerumRotarySliderDial : public SerumLookAndFeel
{
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
};

class SerumSliderBox : public SerumLookAndFeel
{
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
};

class SerumSliderBox2 : public SerumLookAndFeel
{
public:
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
};