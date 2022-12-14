#pragma once

#include <JuceHeader.h>
#include "transportControl.h"
#include "TagDisplay.h"
#include "menuBar.h"
#include "playlist.h"
#include "RoundedImageButton.h"
#include "PlaylistTablebox.h"
#include "CustomLookAndFeel.h"
#include "transportState.h"
#include "AudioPlaybackEngine.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/



class MainComponent  : public juce::Component, juce::ChangeListener
{
public:
    //==============================================================================
    MainComponent(AudioPlaybackEngine* engine);
    ~MainComponent() override;



    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

    void openFileButtonPressed();
    void openFolderButtonPressed();
    void playButtonPressed();
    void pauseButtonPressed();
    void stopButtonPressed();
    void skipForwardButtonPressed();
    void skipBackwardButtonPressed();
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    void changeState(TransportState state);

    
    void updateComponentsFromSettingsChange();
    
private:
    //==============================================================================
    // Your private member variables go here...
    RoundedImageButton openFileButton;
    RoundedImageButton openFolderButton;
    RoundedImageButton playButton;
    RoundedImageButton skipForwardButton;
    RoundedImageButton skipBackwardButton;
    
    juce::TooltipWindow tooltip;
    
    juce::Slider volumeControl;
    
    PlaylistTablebox listbox;
    
    
    std::unique_ptr<juce::FileChooser> chooser;
    AudioPlaybackEngine* playbackEngine;
    
    TransportState state;
    transportControl transport;
    TagInfo tags;
    TagDisplay tagsDisplay;
    juce::StringPairArray dbg;
    Playlist list;

    CustomLookAndFeel theme;
    juce::Image skipForwardImage;
    juce::Image skipBackwardImage;
    juce::Image pauseImage;
    juce::Image playImage;
    juce::Image fileImage;
    juce::Image folderImage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

