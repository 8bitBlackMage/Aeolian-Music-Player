/*
  ==============================================================================

    transportControl.h
    Created: 25 Jul 2022 6:22:51pm
    Author:  Alice Exley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class transportControl  : public juce::AnimatedAppComponent
{
public:
    enum colourIds
    {
        DisplayBackground,
        TransportLine,
        TransportHandle,
        ScrubHandle,
        TransportText,
        ScrubText,
        numIds 
    };
    
    
    transportControl();
    ~transportControl() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setTransportSource(juce::AudioTransportSource * source) { transport = source;}
    void update() override;
    
    
    void mouseEnter(const juce::MouseEvent & event) override {mouseOver = true;}
    void mouseExit(const juce::MouseEvent & event) override {mouseOver = false;}
    void mouseMove(const juce::MouseEvent & event) override;
    void mouseUp(const juce::MouseEvent & event) override;
    void mouseDown(const juce::MouseEvent & event) override;
    void mouseDrag(const juce::MouseEvent &event) override;

private:
    juce::Array<juce::Colour> colours;
    
    
    juce::AudioTransportSource * transport;
    bool mouseOver;
    float size = 10;
    float mousePos;
    bool mouseHeld;
    juce::Font customFont;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (transportControl)
};
