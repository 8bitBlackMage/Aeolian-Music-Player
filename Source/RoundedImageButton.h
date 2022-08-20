/*
  ==============================================================================

    RoundedImageButton.h
    Created: 15 Aug 2022 7:50:39pm
    Author:  Alice Exley

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"





class RoundedImageButton: public juce::Button
{
public:
    enum ColourIds
    {
        backgroundOff,
        backgroundOn,
        backgroundHighlightOn,
        backgroundHighlightOff,
        numIDs
    };
    RoundedImageButton();
    RoundedImageButton(juce::String ButtonName);
    void setImage(juce::Image & i);
    void paintButton(juce::Graphics & g, bool Down, bool Highlighted);
private:
    juce::Image image;
    juce::Array<juce::Colour> colours;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RoundedImageButton)
};
