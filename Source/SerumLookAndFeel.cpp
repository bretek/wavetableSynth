/*
  ==============================================================================

    SerumLookAndFeel.cpp
    Created: 15 Jun 2023 11:24:26pm
    Author:  joseph

  ==============================================================================
*/

#include "SerumLookAndFeel.h"

SerumLookAndFeel::SerumLookAndFeel ()
{
    setColour (SerumGrey1, juce::Colour (137, 148, 154));
    setColour (SerumGrey2, juce::Colour (99, 108, 114));
    setColour (SerumGrey3, juce::Colour (66, 72, 76));
    setColour (SerumGrey4, juce::Colour (50, 55, 60));
    setColour (SerumGrey5, juce::Colour (37, 41, 45));

    setColour (SerumBlue1, juce::Colour (194, 238, 255));
    setColour (SerumBlue2, juce::Colour (108, 170, 235));
    setColour (SerumBlue3, juce::Colour (6, 131, 195));

    setColour (SerumGreen1, juce::Colour (134, 248, 1));
    setColour (SerumGreen2, juce::Colour (41, 66, 16));

    setColour (SerumRed1, juce::Colour (64, 44, 43));

    setColour (juce::ResizableWindow::backgroundColourId, findColour (SerumGrey1));
}

void SerumLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    g.setColour (label.findColour (juce::Label::ColourIds::textColourId));
    g.setFont (getCustomFont().withHeight (label.getHeight()));
    g.drawFittedText (label.getText(), 0, 0, label.getWidth(), label.getHeight(), label.getJustificationType(), 1);
}

void SerumLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button, 
                    const juce::Colour& backgroundColour, 
                    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    g.setColour (findColour (SerumGrey5));
    g.fillRoundedRectangle (0, 0, button.getWidth(), button.getHeight(), roundedRectangleFactor1);
}

void SerumLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    const auto size = juce::jmin (button.getWidth(), button.getHeight());
    juce::ColourGradient buttonGradient;

    if (button.getToggleState())
    {
        buttonGradient = juce::ColourGradient (findColour (SerumBlue3), 
                                                button.getWidth()/2, button.getHeight()/2,
                                                findColour (SerumBlue1),
                                                button.getWidth()/2, button.getHeight()/2 + size,
                                                true);
    }
    else
    {
        buttonGradient = juce::ColourGradient (findColour (SerumGrey5), 
                                                button.getWidth()/2, button.getHeight()/2,
                                                findColour (SerumGrey4),
                                                button.getWidth()/2, button.getHeight()/2 + size,
                                                true);
    }

    g.setGradientFill (buttonGradient);
    g.fillRoundedRectangle ((button.getWidth() - size) / 2, (button.getHeight() - size) / 2, size, size, roundedRectangleFactor1);
}

const juce::Font SerumLookAndFeel::getCustomFont()
{
    static auto typeface = juce::Typeface::createSystemTypefaceFor (BinaryData::Seravek_Basic_ttf,
                                                                    BinaryData::Seravek_Basic_ttfSize);

    return juce::Font (typeface);
}

void SerumRotarySliderDial::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&)
{
    const auto radius = juce::jmin (width/2, height/2);
    const auto diameter = radius*2;
    const auto centerX = x + width/2;
    const auto centerY = y + height/2;
    const auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    const auto markerRadius = diameter * 0.03f;
    const auto gap1 = diameter * 0.01f;
    const auto outerRingWidth = diameter * 0.09f;
    const auto shadowWidth = diameter * 0.15f;
    const auto ridgeWidth = diameter * 0.09f;
    const auto dialLength = diameter * 0.55f;
    const auto dialWidth = diameter * 0.06f;

    // draw markers
    drawDialMarkers (g, 3, 
                    diameter, diameter - markerRadius*2, 
                    centerX, centerY, 
                    rotaryStartAngle, rotaryEndAngle,
                    findColour (SerumGrey2));   //juce::Colour(192,211,226));

    // draw outer ring
    drawRing (g, 
              diameter - markerRadius*2 - gap1, diameter - markerRadius*2 - gap1 - outerRingWidth, 
              centerX, centerY, 
              rotaryStartAngle, rotaryEndAngle, 
              juce::ColourGradient (findColour (SerumGrey2), juce::Point<float> (0,0),  //juce::Colour (151,171,188)
                                    findColour (SerumGrey2), juce::Point<float> (0,0),   //juce::Colour (151,171,188)
                                    false));

    // draw main knob shadow
    const auto shadowOuterDiameter = diameter - markerRadius*2 - gap1 - outerRingWidth;
    drawRing (g, 
              shadowOuterDiameter, shadowOuterDiameter - shadowWidth, 
              centerX, centerY, 
              0, 2.5f*juce::MathConstants<float>::pi, 
              juce::ColourGradient (juce::Colour (26,32,38), juce::Point<float> (0,0), 
                                    juce::Colour (26,32,38), juce::Point<float> (0,0), 
                                    false));

    // draw main knob ridge
    auto ridgeOuterDiameter = shadowOuterDiameter - shadowWidth;
    juce::ColourGradient ridgeColourGradient (findColour (SerumGrey4), juce::Point<float> (centerX,centerY),   //juce::Colour (52,64,76)
                                              findColour (SerumGrey4), juce::Point<float> (centerX,centerY+ridgeOuterDiameter/2), 
                                              true);
    ridgeColourGradient.addColour (1.0 - ((ridgeWidth/2) / (ridgeOuterDiameter/2)), findColour (SerumGrey2));

    drawRing (g, 
              ridgeOuterDiameter, ridgeOuterDiameter - ridgeWidth, 
              centerX, centerY, 
              0, 2.5f*juce::MathConstants<float>::pi, 
              ridgeColourGradient);

    // draw main knob
    drawRing (g, 
              ridgeOuterDiameter - ridgeWidth, 0, 
              centerX, centerY, 
              0, 2.5f*juce::MathConstants<float>::pi, 
              juce::ColourGradient (findColour (SerumGrey4), juce::Point<float> (0,centerY+(ridgeOuterDiameter - ridgeWidth)/2),   //juce::Colour (53,65,76)
                                    findColour (SerumGrey3), juce::Point<float> (0,centerY-(ridgeOuterDiameter - ridgeWidth)/2), false));  //juce::Colour (70,85,100)

    // draw blue dial
    drawDial (g, 
              shadowOuterDiameter, ridgeOuterDiameter, 
              centerX, centerY,
              angle, dialWidth,
              findColour (SerumBlue3));

    // draw white dial
    drawDial (g, 
              ridgeOuterDiameter - ridgeWidth, ridgeOuterDiameter - dialLength, 
              centerX, centerY,
              angle, dialWidth,
              juce::Colour (255,255,255));
}

void SerumSliderBox::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    const auto centerX = x + width/2;
    const auto centerY = y + height/2;

    // draw box
    auto boxWidth = width * 0.7f;
    auto boxHeight = boxWidth * 0.5f;

    if (boxHeight > height)
    {
        boxHeight *= height/boxHeight;
        boxWidth *= height/boxHeight;
    }

    const auto outlineLineWidth = width * 0.03f;
    const auto roundingFactor = width * 0.00005f;

    const auto innerWidth = boxWidth - outlineLineWidth;
    const auto innerHeight = boxHeight - outlineLineWidth;

    g.setColour (juce::Colours::black);
    g.drawRoundedRectangle ((width - boxWidth) / 2, (height - boxHeight) / 2, boxWidth, boxHeight, boxWidth * roundingFactor, outlineLineWidth);

    juce::Colour backgroundColour (findColour (SerumGrey5));
    juce::ColourGradient backgroundGradient (findColour (SerumGreen2), juce::Point<float>(0,0), findColour (SerumRed1), juce::Point<float>(1,0), false);

    if (slider.getValue() != slider.getMinimum())
    {
        backgroundColour = backgroundGradient.getColourAtPosition (sliderPos);
    }

    g.setColour (backgroundColour);
    g.fillRoundedRectangle ((width - innerWidth) / 2, (height - innerHeight) / 2, 
                            innerWidth, innerHeight, innerWidth * roundingFactor);

    // draw arrows
    auto triangleWidth = boxWidth * 0.1f;
    auto triangleHeight = triangleWidth * 0.866f;

    if (triangleHeight * 2.5f > innerHeight)
    {
        triangleHeight *= innerHeight/(triangleHeight*2.5f);
        triangleWidth *= innerHeight/(triangleHeight*2.5f);
    }

    const auto triangleXProportion = 0.7f;
    const auto triangleYProportion = 0.1f;

    const juce::Point<float> point1(centerX, centerY + triangleHeight);
    const juce::Point<float> point2(centerX - triangleWidth*0.5f, centerY);
    const juce::Point<float> point3(centerX + triangleWidth*0.5f, centerY);

    if (slider.getThumbBeingDragged() != -1)
    {
        g.setColour (juce::Colours::yellow);
    }
    else
    {
        g.setColour (findColour (SerumGrey1));
    }

    juce::Path path;
    path.addTriangle (point1, point2, point3);
    path.applyTransform (juce::AffineTransform::translation (innerWidth * triangleXProportion * 0.5f, innerHeight * triangleYProportion));
    g.fillPath (path);

    path.clear ();
    path.addTriangle (point1, point2, point3);
    path.applyTransform (juce::AffineTransform::rotation (juce::MathConstants<float>::pi, centerX, centerY));
    path.applyTransform (juce::AffineTransform::translation (innerWidth * triangleXProportion * 0.5f, -innerHeight * triangleYProportion));
    g.fillPath (path);

    // draw text
    g.setColour (juce::Colours::white);
    g.setFont (getCustomFont().withHeight (innerHeight));
    g.drawText (juce::String(slider.getValue()), 
                (width - innerWidth) / 2, (height - innerHeight) / 2, 
                innerWidth, innerHeight, juce::Justification::centred);
}

void SerumSliderBox2::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
    // draw value text
    g.setColour (juce::Colours::white);
    g.setFont (getCustomFont().withHeight (slider.getHeight()));
    g.drawText (juce::String(slider.getValue()), 
                0, 0,
                slider.getWidth(), slider.getHeight(), juce::Justification::centred);
}

// Drawing helpers

void SerumLookAndFeel::drawDialMarkers (juce::Graphics& g, int numMarkers, 
                                        float outerDiameter, float innerDiameter, 
                                        float centerX, float centerY, 
                                        float startAngle, float endAngle,
                                        juce::Colour colour)
{
    auto markerAngle = startAngle;
    const auto centerRadius = (outerDiameter + innerDiameter) / 2 / 2;
    const auto markerRadius = (outerDiameter - innerDiameter) / 2 / 2;

    g.setColour (colour);

    for (int i = 0; i < numMarkers; ++i)
    {
        juce::Path markerPath;
        markerPath.addEllipse (-markerRadius, -centerRadius-markerRadius, markerRadius*2, markerRadius*2);
        markerPath.applyTransform (juce::AffineTransform::rotation (markerAngle).translated (centerX, centerY));
        g.fillPath (markerPath);
        markerAngle += (endAngle - startAngle) * 1/(numMarkers-1);
    }
}

void SerumLookAndFeel::drawRing (juce::Graphics& g, 
                  float outerDiameter, float innerDiameter, 
                  float centerX, float centerY,
                  float startAngle, float endAngle,
                  juce::ColourGradient colourGradient)
{
    juce::Path path;
    const auto centerRadius = (outerDiameter + innerDiameter) / 2 / 2;
    const auto pathWidth = (outerDiameter - innerDiameter) / 2;
    path.addCentredArc (centerX, centerY, 
                        centerRadius, centerRadius, 
                        0, 
                        startAngle, endAngle, true);
    g.setGradientFill (colourGradient);
    g.strokePath (path, juce::PathStrokeType(pathWidth));
}

void SerumLookAndFeel::drawDial (juce::Graphics& g, 
                  float outerDiameter, float innerDiameter, 
                  float centerX, float centerY,
                  float angle, float width,
                  juce::Colour colour)
{
    const auto dialLength = (outerDiameter - innerDiameter) / 2;
    juce::Path path;
    path.addRectangle (-width/2, -outerDiameter/2, width, dialLength);
    path.applyTransform (juce::AffineTransform::rotation (angle).translated (centerX, centerY));
    g.setColour (colour);
    g.fillPath (path);
}
