#pragma once

#include <JuceHeader.h>
#include "transportControl.h"
#include "TagDisplay.h"
#include "menuBar.h"
#include "playlist.h"
#include "RoundedImageButton.h"
#include "PlaylistTablebox.h"
#include "CustomLookAndFeel.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/


enum TransportState
{
    Stopped,
    Starting,
    Playing,
    Pausing,
    Paused,
    Stopping
};


class MainComponent  : public juce::AudioAppComponent, juce::ChangeListener
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

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
    void generateReader(juce::File &file);
    void changeState(TransportState state);
    
    void saveSettings();
    bool loadSettings();
    
private:
    //==============================================================================
    // Your private member variables go here...
    RoundedImageButton openFileButton;
    RoundedImageButton openFolderButton;
    RoundedImageButton playButton;
    RoundedImageButton skipForwardButton;
    RoundedImageButton skipBackwardButton;
    
    juce::TooltipWindow tooltip;
    juce::File settingsFile;
    
    juce::Slider volumeControl;
    
    PlaylistTablebox listbox;
    
    
    std::unique_ptr<juce::FileChooser> chooser;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    TransportState state;
    transportControl transport;
    TagInfo tags;
    TagDisplay tagsDisplay;
    juce::StringPairArray dbg;
    Playlist list;
    menuBar mainMenu;
    CustomLookAndFeel theme;
    juce::Image skipForwardImage;
    juce::Image skipBackwardImage;
    juce::Image pauseImage;
    juce::Image playImage;
    juce::Image fileImage;
    juce::Image folderImage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

