/*
  ==============================================================================

    Oscillator.h
    Created: 20 Jun 2023 11:41:27pm
    Author:  joseph

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SerumLookAndFeel.h"
#include "WavetableVisualiser.h"

//==============================================================================
/*
*/
class Oscillator  : public juce::Component
{
public:
    Oscillator();
    ~Oscillator() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    void attachPitchControls (juce::AudioProcessorValueTreeState& vts);

    void attachUnisonControl (juce::AudioProcessorValueTreeState& vts);
    void attachDetuneControl (juce::AudioProcessorValueTreeState& vts);
    void attachBlendControl (juce::AudioProcessorValueTreeState& vts);
    void attachPhaseControl (juce::AudioProcessorValueTreeState& vts);
    void attachRandomControl (juce::AudioProcessorValueTreeState& vts);
    void attachPanControl (juce::AudioProcessorValueTreeState& vts);
    void attachLevelControl (juce::AudioProcessorValueTreeState& vts);

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
private:
    // looks and feels
    SerumLookAndFeel serumLookAndFeel;
    SerumRotarySliderDial serumDial;
    SerumSliderBox serumSliderBox;
    SerumSliderBox2 serumSliderBox2;

    // components
    // Title
    juce::DrawableRectangle titleBackground;
    juce::ToggleButton oscToggle;
    juce::Label titleLabel;

    // WT
    juce::TextButton wtSpacer;
    juce::TextButton wtFile;
    juce::TextButton wtLastFile;
    juce::TextButton wtNextFile;

    juce::Label octaveSlideLabel;
    juce::Label semiSlideLabel;
    juce::Label fineSlideLabel;
    juce::Label coarseSlideLabel;

    juce::Slider octaveSlide;
    std::unique_ptr<SliderAttachment> octaveAttachment;
    juce::Slider semiSlide;
    std::unique_ptr<SliderAttachment> semiAttachment;
    juce::Slider fineSlide;
    std::unique_ptr<SliderAttachment> fineAttachment;
    juce::Slider coarseSlide;
    std::unique_ptr<SliderAttachment> coarseAttachment;

    juce::DrawableRectangle wtBackground;
    juce::DrawableRectangle wtSlideBackground;
    juce::DrawableRectangle slideBackground1;
    juce::DrawableRectangle slideBackground2;
    juce::DrawableRectangle slideBackground3;
    juce::DrawableRectangle slideBackground4;

    WavetableVisualiser wtEditor;

    // WT Controls
    juce::Slider unisonControl;
    std::unique_ptr<SliderAttachment> unisonAttachment;
    juce::Slider detuneControl;
    std::unique_ptr<SliderAttachment> detuneAttachment;
    juce::Slider blendControl;
    std::unique_ptr<SliderAttachment> blendAttachment;
    juce::Slider phaseControl;
    std::unique_ptr<SliderAttachment> phaseAttachment;
    juce::Slider randControl;
    std::unique_ptr<SliderAttachment> randAttachment;
    juce::Slider wtControl;
    juce::Slider morphControl;
    juce::Slider panControl;
    std::unique_ptr<SliderAttachment> panAttachment;
    juce::Slider levelControl;
    std::unique_ptr<SliderAttachment> levelAttachment;

    juce::DrawableRectangle labelBackground1;
    juce::DrawableRectangle labelBackground2;
    juce::DrawableRectangle labelBackground3;
    juce::DrawableRectangle labelBackground4;
    juce::DrawableRectangle labelBackground5;

    juce::Label unisonLabel;
    juce::Label detuneLabel;
    juce::Label blendLabel;
    juce::Label phaseLabel;
    juce::Label randLabel;
    juce::Label wtLabel;
    juce::Label morphLabel;
    juce::Label panLabel;
    juce::Label levelLabel;

    // layout helpers
    juce::FlexBox layoutTitleBar();

    void fitFlexBoxBackground (juce::FlexBox box, juce::DrawableRectangle& background);
    juce::FlexBox layoutControls (juce::FlexBox& knobBox, juce::FlexBox& labelBox, std::vector<juce::Slider*> knobs, std::vector<juce::Label*> labels);
    juce::FlexBox layoutSliderControls (juce::Label& label, juce::Slider& slider);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Oscillator)
};
