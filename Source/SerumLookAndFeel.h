/*
  ==============================================================================

    SerumLookAndFeel.h
    Created: 15 Jun 2023 11:24:26pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SerumLookAndFeel : public juce::LookAndFeel_V4
{
public:
    enum SerumColours
    {
        SerumGrey1,
        SerumGrey2,
        SerumGrey3,
        SerumGrey4,
        SerumGrey5,

        SerumBlue1,
        SerumBlue2,
        SerumBlue3,

        SerumGreen1,
        SerumGreen2,

        SerumRed1
    };

    float roundedRectangleFactor1 = 3.0f;
    float roundedRectangleFactor2 = 30.0f;

    SerumLookAndFeel ();

    void drawLabel (juce::Graphics& g, juce::Label& label) override;

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