/*
  ==============================================================================

    transportControl.cpp
    Created: 25 Jul 2022 6:22:51pm
    Author:  Alice Exley

  ==============================================================================
*/

#include <JuceHeader.h>
#include "transportControl.h"

//==============================================================================
transportControl::transportControl()
{
    for(int i = 0; i < numIds; i ++)
        colours.add(juce::Colours::white);
    
    setFramesPerSecond(60);
    mouseOver = false;
    mouseHeld = false;
    
    customFont = juce::Typeface::createSystemTypefaceFor(BinaryData::DSEG7ClassicBold_ttf, BinaryData::DSEG7ClassicBold_ttfSize);
    colours.set(DisplayBackground, juce::Colour(165, 176, 158));
    for(int i = 1; i < numIds; i++)
        colours.set(i, juce::Colours::black);
    
}

transportControl::~transportControl()
{
}

void transportControl::paint (juce::Graphics& g)
{
    g.setFont(customFont);
    g.setFont(20);
    g.fillAll(colours[DisplayBackground]);
    
    if(transport == nullptr)
        return;
    
    double length = transport->getLengthInSeconds();
    double transportPosition = transport->getCurrentPosition();
    
    double delta = 0;
    if(length > 0 && transportPosition > 0)
        delta = transportPosition / length;
    float width;
    if(!mouseHeld)
        width = getWidth() * delta;
    else
        width = mousePos;
    
    g.setColour(colours[TransportLine]);
    g.fillRect(0,getHeight() - 5,(int) width, 5);
    g.setColour(colours[TransportHandle]);
    
    
    float x = width - (5.0f / 2.0f);
    
    g.fillRect(x, (float)getHeight() - size, 5.0f, size);
    
    g.setColour(colours[ScrubHandle]);
    
    
    float timeInMinutesFloat;
    int timeInMinutes;
    int timeInSeconds;
    if(mouseOver)
    {
     
        g.fillRect((mousePos - size * 0.5f), getHeight() - size, 5.0f, size);
        
        float mouseOvertime = ((mousePos - size * 0.5) / getWidth()) * length;
        timeInMinutesFloat = mouseOvertime / 60;
        timeInMinutes = floorf(timeInMinutesFloat);
        timeInSeconds = mouseOvertime - (timeInMinutes * 60);
        
    }
    else
    {
        timeInMinutesFloat = transportPosition / 60;
        timeInMinutes = floorf(timeInMinutesFloat);
        timeInSeconds = transportPosition - (timeInMinutes * 60);
    }
    
    
    
    
    juce::String seconds;
    juce::String minutes;
    juce::String secondsFlash;
    if(timeInSeconds < 10)
        seconds = "0" + juce::String(timeInSeconds);
    else
        seconds = juce::String(timeInSeconds);
    if(timeInMinutes < 10)
        minutes = "0" + juce::String(timeInMinutes);
    else
        minutes = juce::String(timeInMinutes);
    if(timeInSeconds %2 == 0)
        secondsFlash = ":";
    else
        secondsFlash = " ";
    
    juce::String txt = minutes + secondsFlash + seconds;
    g.setColour(colours[DisplayBackground].darker().withAlpha(0.5f));
    g.drawText("88:88", getWidth() - 80, -15, 80, 80, juce::Justification(juce::Justification::Flags::horizontallyCentred));
    g.setColour(colours[ScrubText]);
    g.drawText(txt, getWidth() - 80, -15, 80, 80, juce::Justification(juce::Justification::Flags::horizontallyCentred));
}

void transportControl::mouseMove(const juce::MouseEvent & event)
{
    mousePos = event.getPosition().getX();
}
void transportControl::mouseUp(const juce::MouseEvent & event)
{
    mouseHeld = false;
    if(transport !=nullptr)
    {
        double position = (((double)event.getPosition().getX()/ getWidth()) * transport->getLengthInSeconds());
        transport->setPosition(position);
        transport->start();
    }


}
void transportControl::mouseDown(const juce::MouseEvent & event)
{
    mouseHeld = true;
if(transport != nullptr)
{
    transport->stop();
}
    
}

void transportControl::mouseDrag(const juce::MouseEvent & event)
{

}
void transportControl::update()
{
    
}

void transportControl::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
