/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 18 Aug 2022 5:04:25pm
    Author:  Alice Exley

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


class CustomLookAndFeel: public juce::LookAndFeel_V4
{
public:
    CustomLookAndFeel();
    
    
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
                           const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override;
    
};
