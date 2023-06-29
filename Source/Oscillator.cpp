/*
  ==============================================================================

    Oscillator.cpp
    Created: 20 Jun 2023 11:41:27pm
    Author:  joseph

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Oscillator.h"

//==============================================================================
Oscillator::Oscillator()
{
    this->setLookAndFeel (&serumLookAndFeel);

    // Title bar
    titleBackground.setFill (getLookAndFeel().findColour (serumLookAndFeel.SerumGrey2));
    titleBackground.setCornerSize (juce::Point<float> (5.0f, 5.0f));
    addAndMakeVisible (&titleBackground);

    oscToggle.setToggleState (true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible (&oscToggle);

    titleLabel.setText ("OSC A", juce::NotificationType::dontSendNotification);
    titleLabel.setJustificationType (juce::Justification::left);
    addAndMakeVisible (&titleLabel);

    // WT Box
    wtBackground.setCornerSize (juce::Point<float> (5.0f, 5.0f));
    addAndMakeVisible (&wtBackground);

    // WT file navigation
    addAndMakeVisible (&wtSpacer);
    wtFile.setButtonText ("Default");
    addAndMakeVisible (&wtFile);
    wtLastFile.setButtonText ("<");
    addAndMakeVisible (&wtLastFile);
    wtNextFile.setButtonText (">");
    addAndMakeVisible (&wtNextFile);

    // WT pitch controls
    wtSlideBackground.setFill (getLookAndFeel().findColour (serumLookAndFeel.SerumGrey4));
    addAndMakeVisible (&wtSlideBackground);

    std::vector<juce::Slider*> pitchControls = { &octaveSlide, &semiSlide, &fineSlide, &coarseSlide };

    octaveSlide.setRange (-4, 4, 1.0);
    semiSlide.setRange (-24, 24, 1.0);
    fineSlide.setRange (-5, 5, 0.01);
    coarseSlide.setRange (-100, 100, 0.01);

    for (auto control : pitchControls)
    {
        control->setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
        control->setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        control->setLookAndFeel (&serumSliderBox2);
        addAndMakeVisible (control);
    }

    std::vector<juce::Label*> pitchControlLabels = { &octaveSlideLabel, &semiSlideLabel, &fineSlideLabel, &coarseSlideLabel };

    octaveSlideLabel.setText ("OCT", juce::NotificationType::dontSendNotification);
    semiSlideLabel.setText ("SEM", juce::NotificationType::dontSendNotification);
    fineSlideLabel.setText ("FIN", juce::NotificationType::dontSendNotification);
    coarseSlideLabel.setText ("CRS", juce::NotificationType::dontSendNotification);

    for (auto label : pitchControlLabels)
    {
        label->setColour (label->textColourId, findColour(serumLookAndFeel.SerumGrey2));
        label->setJustificationType (juce::Justification::bottomLeft);
        addAndMakeVisible (label);
    }

    std::vector<juce::DrawableRectangle*> pitchControlBackgrounds = { &slideBackground1, &slideBackground2, &slideBackground3, &slideBackground4 };

    for (auto background : pitchControlBackgrounds)
    {
        background->setFill (getLookAndFeel().findColour (serumLookAndFeel.SerumGrey5));
        addAndMakeVisible (background);
    }

    // WT editor
    wtEditor.setSize (500, 300);
    wtEditor.setColour (juce::ResizableWindow::backgroundColourId, juce::Colours::red);
    addAndMakeVisible (&wtEditor);

    // WT Controls
    std::vector<juce::Slider*> controls = { &unisonControl, &detuneControl, &blendControl, &phaseControl, &randControl, &wtControl, &morphControl, &panControl, &levelControl };

    unisonControl.setRange (1, 16, 1.0f);

    for (auto control : controls)
    {
        control->setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
        control->setTextBoxStyle (juce::Slider::NoTextBox, true, 0, 0);
        control->setLookAndFeel (&serumDial);
    }
    unisonControl.setLookAndFeel (&serumSliderBox);

    for (auto control : controls)
    {
        addAndMakeVisible (control);
    }

    // WT Label Backgrounds
    
    std::vector<juce::DrawableRectangle*> labelBackgrounds = { &labelBackground1, &labelBackground2, &labelBackground3, &labelBackground4, &labelBackground5 };

    for (auto background : labelBackgrounds)
    {
        background->setFill (getLookAndFeel().findColour (serumLookAndFeel.SerumGrey4));
        background->setCornerSize (juce::Point<float> (5.0f, 5.0f));
        addAndMakeVisible (background);
    }

    // WT Labels

    std::vector<juce::Label*> labels = { &unisonLabel, &detuneLabel, &blendLabel, &phaseLabel, &randLabel, &wtLabel, &morphLabel, &panLabel, &levelLabel };

    unisonLabel.setText ("UNISON", juce::NotificationType::dontSendNotification);
    detuneLabel.setText ("DETUNE", juce::NotificationType::dontSendNotification);
    blendLabel.setText ("BLEND", juce::NotificationType::dontSendNotification);
    phaseLabel.setText ("PHASE", juce::NotificationType::dontSendNotification);
    randLabel.setText ("RAND", juce::NotificationType::dontSendNotification);
    wtLabel.setText ("WT POS", juce::NotificationType::dontSendNotification);
    morphLabel.setText ("OFF", juce::NotificationType::dontSendNotification);
    panLabel.setText ("PAN", juce::NotificationType::dontSendNotification);
    levelLabel.setText ("LEVEL", juce::NotificationType::dontSendNotification);

    for (auto label : labels)
    {
        label->setJustificationType (juce::Justification::centred);
        addAndMakeVisible (label);
    }
}

Oscillator::~Oscillator()
{
    this->setLookAndFeel (nullptr);
}

void Oscillator::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void Oscillator::resized()
{
    juce::FlexBox verticalLayout;
    verticalLayout.flexDirection = juce::FlexBox::Direction::column;

    // Title bar
    juce::FlexBox titleBar = layoutTitleBar();

    // WT component
    juce::FlexBox wtComponent;
    wtComponent.flexDirection = juce::FlexBox::Direction::column;

    juce::FlexBox fileControls;
    fileControls.items.add (juce::FlexItem (wtSpacer).withFlex (1.0f).withMargin (2.0f));
    fileControls.items.add (juce::FlexItem (wtFile).withFlex (10.0f).withMargin (2.0f));
    fileControls.items.add (juce::FlexItem (wtLastFile).withFlex (0.7f).withMargin (2.0f));
    fileControls.items.add (juce::FlexItem (wtNextFile).withFlex (0.7f).withMargin (2.0f));
    fileControls.performLayout (getLocalBounds());

    juce::FlexBox pitchControls;
    juce::FlexBox slider1 = layoutSliderControls (octaveSlideLabel, octaveSlide);
    juce::FlexBox slider2 = layoutSliderControls (semiSlideLabel, semiSlide);
    juce::FlexBox slider3 = layoutSliderControls (fineSlideLabel, fineSlide);
    juce::FlexBox slider4 = layoutSliderControls (coarseSlideLabel, coarseSlide);

    pitchControls.items.add (juce::FlexItem (slider1).withFlex (1.0f).withMargin (5.0f));
    pitchControls.items.add (juce::FlexItem (slider2).withFlex (1.0f).withMargin (5.0f));
    pitchControls.items.add (juce::FlexItem (slider3).withFlex (1.0f).withMargin (5.0f));
    pitchControls.items.add (juce::FlexItem (slider4).withFlex (1.0f).withMargin (5.0f));
    pitchControls.performLayout (getLocalBounds());

    wtComponent.items.add (juce::FlexItem (fileControls).withFlex (1.0f));
    wtComponent.items.add (juce::FlexItem (pitchControls).withFlex (1.0f));
    wtComponent.items.add (juce::FlexItem (wtEditor).withFlex (5.0f));
    wtComponent.performLayout (getLocalBounds());

    // Osc controls
    juce::FlexBox topControlRow;

    juce::FlexBox knobSet1;
    juce::FlexBox labelSet1;
    juce::FlexBox controlSet1 = layoutControls (knobSet1, labelSet1, 
    std::vector<juce::Slider*> ({ &unisonControl, &detuneControl, &blendControl }), 
    std::vector<juce::Label*> ({ &unisonLabel, &detuneLabel, &blendLabel }));

    juce::FlexBox knobSet2;
    juce::FlexBox labelSet2;
    juce::FlexBox controlSet2 = layoutControls (knobSet2, labelSet2, 
    std::vector<juce::Slider*> ({ &phaseControl, &randControl }), 
    std::vector<juce::Label*> ({ &phaseLabel, &randLabel }));

    topControlRow.items.add (juce::FlexItem (controlSet1).withFlex (3.0f));
    topControlRow.items.add (juce::FlexItem ().withFlex (0.3f));
    topControlRow.items.add (juce::FlexItem (controlSet2).withFlex (2.0f));

    topControlRow.performLayout (getLocalBounds());

    juce::FlexBox bottomControlRow;

    juce::FlexBox knobSet3;
    juce::FlexBox labelSet3;
    juce::FlexBox controlSet3 = layoutControls (knobSet3, labelSet3, 
    std::vector<juce::Slider*> ({ &wtControl }), 
    std::vector<juce::Label*> ({ &wtLabel }));

    juce::FlexBox knobSet4;
    juce::FlexBox labelSet4;
    juce::FlexBox controlSet4 = layoutControls (knobSet4, labelSet4, 
    std::vector<juce::Slider*> ({ &morphControl }), 
    std::vector<juce::Label*> ({ &morphLabel }));

    juce::FlexBox knobSet5;
    juce::FlexBox labelSet5;
    juce::FlexBox controlSet5 = layoutControls (knobSet5, labelSet5, 
    std::vector<juce::Slider*> ({ &panControl, &levelControl }), 
    std::vector<juce::Label*> ({ &panLabel, &levelLabel }));

    bottomControlRow.items.add (juce::FlexItem (controlSet3).withFlex (1.0f));
    bottomControlRow.items.add (juce::FlexItem ().withFlex (0.3f));
    bottomControlRow.items.add (juce::FlexItem (controlSet4).withFlex (1.7f));
    bottomControlRow.items.add (juce::FlexItem ().withFlex (0.3f));
    bottomControlRow.items.add (juce::FlexItem (controlSet5).withFlex (2.0f));

    bottomControlRow.performLayout (getLocalBounds());

    verticalLayout.items.add (juce::FlexItem (titleBar).withFlex (1.5f));
    verticalLayout.items.add (juce::FlexItem ().withFlex (0.4f));
    verticalLayout.items.add (juce::FlexItem (wtComponent).withFlex (10.0f));
    verticalLayout.items.add (juce::FlexItem (topControlRow).withFlex (4.0f));
    verticalLayout.items.add (juce::FlexItem (bottomControlRow).withFlex (4.0f));

    verticalLayout.performLayout (getLocalBounds());

    fitFlexBoxBackground (titleBar, titleBackground);

    fitFlexBoxBackground (wtComponent, wtBackground);
    fitFlexBoxBackground (pitchControls, wtSlideBackground);

    fitFlexBoxBackground (labelSet1, labelBackground1);
    fitFlexBoxBackground (labelSet2, labelBackground2);
    fitFlexBoxBackground (labelSet3, labelBackground3);
    fitFlexBoxBackground (labelSet4, labelBackground4);
    fitFlexBoxBackground (labelSet5, labelBackground5);
}

void Oscillator::attachUnisonControl (juce::AudioProcessorValueTreeState& vts)
{
    unisonAttachment.reset (new SliderAttachment (vts, "unison", unisonControl));
}

void Oscillator::attachDetuneControl (juce::AudioProcessorValueTreeState& vts)
{
    detuneAttachment.reset (new SliderAttachment (vts, "detune", detuneControl));
}

void Oscillator::attachBlendControl (juce::AudioProcessorValueTreeState& vts)
{
    blendAttachment.reset (new SliderAttachment (vts, "blend", blendControl));
}

void Oscillator::attachPhaseControl (juce::AudioProcessorValueTreeState& vts)
{
    phaseAttachment.reset (new SliderAttachment (vts, "phase", phaseControl));
}

void Oscillator::attachRandomControl (juce::AudioProcessorValueTreeState& vts)
{
    randAttachment.reset (new SliderAttachment (vts, "random", randControl));
}

void Oscillator::attachPanControl (juce::AudioProcessorValueTreeState& vts)
{
    panAttachment.reset (new SliderAttachment (vts, "pan", panControl));
}

void Oscillator::attachLevelControl (juce::AudioProcessorValueTreeState& vts)
{
    levelAttachment.reset (new SliderAttachment (vts, "level", levelControl));
}

juce::FlexBox Oscillator::layoutTitleBar()
{
    juce::FlexBox bar;
    bar.items.add (juce::FlexItem ().withFlex (0.5f));
    bar.items.add (juce::FlexItem (oscToggle).withFlex (1.0f));
    bar.items.add (juce::FlexItem ().withFlex (0.5f));
    bar.items.add (juce::FlexItem (titleLabel).withFlex (12.0f));

    bar.performLayout (getLocalBounds());

    return bar;
}

void Oscillator::fitFlexBoxBackground(juce::FlexBox box, juce::DrawableRectangle& background)
{
    int x = box.items.getFirst().currentBounds.getX();
    int y = box.items.getFirst().currentBounds.getY();
    int width = box.items.getLast().currentBounds.getX() + box.items.getLast().currentBounds.getWidth() - x;
    int height = box.items.getLast().currentBounds.getY() + box.items.getLast().currentBounds.getHeight() - y;

    background.setBounds (x, y, width, height);
    background.setRectangle (juce::Parallelogram<float> (juce::Rectangle<float> (x, y, width, height)));
}

juce::FlexBox Oscillator::layoutControls (juce::FlexBox& knobBox, juce::FlexBox& labelBox, std::vector<juce::Slider*> knobs, std::vector<juce::Label*> labels)
{
    juce::FlexBox controlSet;
    controlSet.flexDirection = juce::FlexBox::Direction::column;

    for (auto knob : knobs)
    {
        knobBox.items.add (juce::FlexItem (*knob).withFlex (1.0f));
    }
    knobBox.performLayout (getLocalBounds());

    for (auto label : labels)
    {
        labelBox.items.add (juce::FlexItem (*label).withFlex (1.0f));
    }
    labelBox.performLayout (getLocalBounds());

    controlSet.items.add (juce::FlexItem (knobBox).withFlex (5.0f));
    controlSet.items.add (juce::FlexItem (labelBox).withFlex (1.0f));
    controlSet.performLayout (getLocalBounds());

    return controlSet;
}

juce::FlexBox Oscillator::layoutSliderControls (juce::Label& label, juce::Slider& slider)
{
    juce::FlexBox box;
    box.items.add (juce::FlexItem (label).withFlex (1.0f));
    box.items.add (juce::FlexItem (slider).withFlex (1.0f));
    box.performLayout (getLocalBounds());

    return box;
}