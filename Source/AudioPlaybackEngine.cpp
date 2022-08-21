/*
  ==============================================================================

    AudioProcessor.cpp
    Created: 21 Aug 2022 8:23:40am
    Author:  Alice Exley

  ==============================================================================
*/

#include "AudioPlaybackEngine.h"
AudioPlaybackEngine::AudioPlaybackEngine()
{
    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
}

void AudioPlaybackEngine::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
}

void AudioPlaybackEngine::releaseResources()
{
    transportSource.releaseResources();
}

void AudioPlaybackEngine::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock (bufferToFill);
}

void AudioPlaybackEngine::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    if (source == &transportSource)
    {
        if (transportSource.isPlaying())
            changeState (Playing);
        else if((state == Stopping) || (state == Playing))
            changeState (Stopped);
        else if (state == Pausing)
            changeState(Paused);
    }
}


void AudioPlaybackEngine::generateReader(juce::File &file)
{
    if (file != juce::File{})
           {
               auto* reader = formatManager.createReaderFor (file);
               if (reader != nullptr)
               {
                   
                   for(juce::String key : reader->metadataValues.getAllKeys())
                   {
                       DBG("key " + key + " value " + reader->metadataValues.getValue (key, "unknown"));
                   }
                   
                   auto newSource = std::make_unique<juce::AudioFormatReaderSource> (reader, true);
                   transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
                   changeState(Starting);
                   readerSource.reset (newSource.release());
               }
           }
}

void AudioPlaybackEngine::changeState(TransportState newState)
{
    if (state != newState)
        {
            state = newState;
 
            switch (state)
            {
                case Stopped:                           // [3]
                    transportSource.setPosition (0.0);
                    break;
 
                case Starting:
                    //setButtonImage(playButton, pauseImage);
                    //playButton.setImage(pauseImage);
                    transportSource.start();
                    break;
 
                case Playing:
                    //setButtonImage(playButton, pauseImage);
                    //playButton.setImage(pauseImage);
                    break;
                case Pausing:
                    transportSource.stop();
                   // setButtonImage(playButton, playImage);
                  //  playButton.setImage(playImage);
                    break;
                case Paused:
                    
                    break;
                case Stopping:                          // [6]
                    transportSource.stop();
                    break;
            }
        }
}
