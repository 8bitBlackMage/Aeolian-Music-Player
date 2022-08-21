/*
  ==============================================================================

    AudioProcessor.h
    Created: 21 Aug 2022 8:23:40am
    Author:  Alice Exley

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "transportState.h"




class AudioPlaybackEngine: public juce::AudioSource, juce::ChangeListener
{
public:
    AudioPlaybackEngine();
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void releaseResources();
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void changeListenerCallback (juce::ChangeBroadcaster* source);
    void generateReader(juce::File &file);
    void changeState(TransportState newState);
    juce::AudioTransportSource* getTransportSource() {return &transportSource;}
    
private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
};
