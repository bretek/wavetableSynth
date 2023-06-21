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
    setColour (juce::ResizableWindow::backgroundColourId, grey1);
}

void SerumLookAndFeel::drawLabel (juce::Graphics& g, juce::Label& label)
{
    AdvancedLabel* advancedLabel = dynamic_cast<AdvancedLabel*> (&label);
    if (advancedLabel != NULL)
    {
        drawAdvancedLabel (g, *advancedLabel);
    }
    else
    {
        drawBasicLabel (g, label);
    }
}

void SerumLookAndFeel::drawBasicLabel (juce::Graphics& g, juce::Label& label)
{
    const auto roundingFactor = label.getHeight() * 0.15f;
    g.setColour (grey2);
    g.fillRoundedRectangle (0, 0, label.getWidth(), label.getHeight(), roundingFactor);

    g.setColour (juce::Colours::white);
    g.setFont (getCustomFont().withHeight (label.getHeight()));
    g.drawFittedText (label.getText(), 0, 0, label.getWidth(), label.getHeight(), juce::Justification::centred, 1);
}

void SerumLookAndFeel::drawAdvancedLabel (juce::Graphics& g, AdvancedLabel& label)
{
    const auto roundingFactor = label.getHeight() * 0.15f;
    g.setColour (grey2);
    g.fillRoundedRectangle (0, 0, label.getWidth(), label.getHeight(), roundingFactor);
    if (label.closedLeft)
    {
        g.fillRect (0, 0, label.getWidth()/2, label.getHeight());
    }
    if (label.closedRight)
    {
        g.fillRect (ceil(label.getWidth()/2), 0, ceil(label.getWidth()/2)+1, label.getHeight());
    }

    g.setColour (juce::Colours::white);
    g.setFont (getCustomFont().withHeight (label.getHeight()));
    g.drawFittedText (label.getText(), 0, 0, label.getWidth(), label.getHeight(), label.justification, 1);
}

void SerumLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button, 
                    const juce::Colour& backgroundColour, 
                    bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    const auto roundingFactor = button.getHeight() * 0.05f;
    g.setColour (grey3);
    g.fillRoundedRectangle (0, 0, button.getWidth(), button.getHeight(), roundingFactor);
}

void SerumLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    if (button.getToggleState())
    {
        g.setColour (serumBlue);
    }
    else
    {
        g.setColour (grey3);
    }

    const auto size = juce::jmin (button.getWidth(), button.getHeight());

    g.fillRect ((button.getWidth() - size) / 2, (button.getHeight() - size) / 2, size, size);
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
                    juce::Colour(192,211,226));

    // draw outer ring
    drawRing (g, 
              diameter - markerRadius*2 - gap1, diameter - markerRadius*2 - gap1 - outerRingWidth, 
              centerX, centerY, 
              rotaryStartAngle, rotaryEndAngle, 
              juce::ColourGradient (juce::Colour (151,171,188), juce::Point<float> (0,0), 
                                    juce::Colour (151,171,188), juce::Point<float> (0,0), 
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
    juce::ColourGradient ridgeColourGradient (juce::Colour (52,64,76), juce::Point<float> (centerX,centerY), 
                                              juce::Colour (52,64,76), juce::Point<float> (centerX,centerY+ridgeOuterDiameter/2), 
                                              true);
    ridgeColourGradient.addColour (1.0 - ((ridgeWidth/2) / (ridgeOuterDiameter/2)), juce::Colour (88,106,124));

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
              juce::ColourGradient (juce::Colour (53,65,76), juce::Point<float> (0,centerY+(ridgeOuterDiameter - ridgeWidth)/2), 
                                    juce::Colour (70,85,100), juce::Point<float> (0,centerY-(ridgeOuterDiameter - ridgeWidth)/2), false));

    // draw blue dial
    drawDial (g, 
              shadowOuterDiameter, ridgeOuterDiameter, 
              centerX, centerY,
              angle, dialWidth,
              serumBlue);

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

    const auto outlineLineWidth = width * 0.02f;
    const auto roundingFactor = width * 0.00005f;

    const auto innerWidth = boxWidth - outlineLineWidth;
    const auto innerHeight = boxHeight - outlineLineWidth;

    g.setColour (juce::Colours::black);
    g.drawRoundedRectangle ((width - boxWidth) / 2, (height - boxHeight) / 2, boxWidth, boxHeight, boxWidth * roundingFactor, outlineLineWidth);

    juce::Colour backgroundColour (juce::Colours::grey);
    juce::ColourGradient backgroundGradient (juce::Colour(33,67,48), juce::Point<float>(0,0), juce::Colour(64,44,43), juce::Point<float>(1,0), false);

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
        g.setColour (juce::Colours::lightgrey);
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
    const auto centerX = x + width/2;
    const auto centerY = y + height/2;

    // draw box
    auto boxWidth = width * 0.85f;
    auto boxHeight = boxWidth * 0.4f;

    if (boxHeight > height)
    {
        boxHeight *= height/boxHeight;
        boxWidth *= height/boxHeight;
    }

    g.setColour (grey4);
    g.fillRect ((width - boxWidth) / 2, (height - boxHeight) / 2, boxWidth, boxHeight);

    // draw label text
    g.setColour (grey2);
    g.setFont (getCustomFont().withHeight (boxHeight/3));
    g.drawText ("OCT", 
                (width - boxWidth) / 2, (height - boxHeight) / 2,
                boxWidth/2, boxHeight, juce::Justification::centred);

    // draw value text
    g.setColour (juce::Colours::white);
    g.setFont (getCustomFont().withHeight (boxHeight/2));
    g.drawText (juce::String(slider.getValue()), 
                centerX, (height - boxHeight) / 2,
                boxWidth/2, boxHeight, juce::Justification::centred);
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
