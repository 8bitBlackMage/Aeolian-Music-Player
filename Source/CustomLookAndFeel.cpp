/*
  ==============================================================================

    CustomLookAndFeel.cpp
    Created: 18 Aug 2022 5:04:25pm
    Author:  Alice Exley

  ==============================================================================
*/

#include "CustomLookAndFeel.h"
#include "Colours.h"


CustomLookAndFeel::CustomLookAndFeel()
{
    setColour(juce::Slider::thumbColourId,Colours::offWhite );
    setColour(juce::Slider::backgroundColourId, Colours::lightGrey);
    setColour(juce::Slider::trackColourId, Colours::appleBlue);
}



void CustomLookAndFeel::drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &)
{
    juce::Rectangle<int> bounds = { x,y, width,height};
    bounds = bounds.reduced(5);
    
    auto radius = (float) juce::jmin (bounds.getWidth() / 2, bounds.getHeight() / 2) - 4.0f;
    auto centreX = (float) bounds.getX() + (float) bounds.getWidth()  * 0.5f;
    auto centreY = (float) bounds.getY() + (float) bounds.getHeight() * 0.5f;
    auto rx = centreX - radius;
    auto ry = centreY - radius;
    auto rw = radius * 2.0f;
    auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour (Colours::lightGrey);
    g.fillEllipse (rx, ry, rw, rw);

    // outline
    g.setColour (Colours::darkGrey);
    g.drawEllipse (rx, ry, rw, rw, 1.0f);

    juce::Path p;
    auto pointerLength = radius * 0.33f;
    auto pointerThickness = 2.0f;
    p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));

    // pointer
    g.setColour (Colours::appleBlue);
    g.fillPath (p);
}


