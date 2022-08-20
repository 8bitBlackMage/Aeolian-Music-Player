/*
  ==============================================================================

    RoundedImageButton.cpp
    Created: 15 Aug 2022 7:50:39pm
    Author:  Alice Exley

  ==============================================================================
*/

#include "RoundedImageButton.h"
#include "Colours.h"


RoundedImageButton::RoundedImageButton():
juce::Button("button")
{
    for(int i =0; i < numIDs; i++)
        colours.add(juce::Colour(241,240,240));
    
    
    //colours.set(backgroundHighlightOn,juce::Colour(58, 128,246));
    colours.set(backgroundOn, Colours::lightGrey);
    colours.set(backgroundHighlightOn, Colours::darkGrey);
    colours.set(backgroundHighlightOff, Colours::darkGrey);
    
}
RoundedImageButton::RoundedImageButton(juce::String ButtonName):
juce::Button(ButtonName)
{
    for(int i =0; i < numIDs; i++)
        colours.add(juce::Colours::white);
}
void RoundedImageButton::setImage(juce::Image & i)
{
    image = i;
}
void RoundedImageButton::paintButton(juce::Graphics & g, bool down, bool highlighted)
{
    juce::Rectangle<float> boundstoDraw = getLocalBounds().reduced(5).toFloat();

    if(down)
       if(highlighted)
           g.setColour(colours[backgroundHighlightOn]);
        else
            g.setColour(colours[backgroundOn]);
    else
        if(highlighted)
            g.setColour(colours[backgroundHighlightOff]);
        else
            g.setColour(colours[backgroundOff]);
    
    
    g.fillRoundedRectangle(boundstoDraw, 5.0f);

    g.drawImage(image, boundstoDraw);
    
}
