#include "MainComponent.h"
#include "Colours.h"
#include "Tag.h"
//==============================================================================

MainComponent::MainComponent(AudioPlaybackEngine* engine)
{
    // Make sure you set the size of the component after
    // you add any child components.
    
    playbackEngine = engine;
    
    setSize (800, 600);
    juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("System Font");
    // Some platforms require permissions to open input channels so request that here
//    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
//        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
//    {
//        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
//                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
//    }
//    else
//    {
//        // Specify the number of input and output channels that we want to open
//        setAudioChannels (2, 2);
//    }
    
   // formatManager.registerBasicFormats();
   // transportSource.addChangeListener(this);
   // transport.setTransportSource(&transportSource);
    
    
    
    skipForwardImage =     juce::ImageFileFormat::loadFrom(BinaryData::skipforwardline_png, BinaryData::skipforwardline_pngSize);
    skipBackwardImage =    juce::ImageFileFormat::loadFrom(BinaryData::skipbackline_png, BinaryData::skipbackline_pngSize);
    pauseImage =    juce::ImageFileFormat::loadFrom(BinaryData::pauseline_png, BinaryData::pauseline_pngSize);
    playImage =     juce::ImageFileFormat::loadFrom(BinaryData::playline_png, BinaryData::playline_pngSize);
    fileImage =     juce::ImageFileFormat::loadFrom(BinaryData::fileline_png, BinaryData::fileline_pngSize);
    folderImage =   juce::ImageFileFormat::loadFrom(BinaryData::folderline_png, BinaryData::folderline_pngSize);
    
    //setButtonImage(openFileButton, fileImage);
    openFileButton.setImage(fileImage);
    openFileButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::blue);
    openFileButton.onClick = [this] {openFileButtonPressed();};
    
    openFolderButton.setImage(folderImage);
    openFolderButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::purple);
    openFolderButton.onClick = [this] {openFolderButtonPressed();};
    
    playButton.setImage(playImage);
    playButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::green);
    playButton.onClick = [this] {playButtonPressed();};
    
    skipForwardButton.setImage(skipForwardImage);
    skipForwardButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::orange);
    skipForwardButton.onClick = [this] {skipForwardButtonPressed();};
    
    //setButtonImage(skipBackwardButton, skipBackwardImage);
    skipBackwardButton.setImage(skipBackwardImage);
    skipBackwardButton.onClick = [this] {skipBackwardButtonPressed();};
//    stopButton.setButtonText("stop");
//    stopButton.setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colours::red);
//    stopButton.onClick = [this] {stopButtonPressed();};
    
    
    volumeControl.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::purple);
    volumeControl.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    volumeControl.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    volumeControl.setRange(0.0, 1.0);
    volumeControl.setValue(0.75);
    volumeControl.setLookAndFeel(&theme);
    volumeControl.onValueChange = [this]
    {
        playbackEngine->getTransportSource()->setGain(volumeControl.getValue());
        float tooltip = volumeControl.getValue() * 100;
        juce::String tooltipString = juce::String(tooltip) + "%";
        volumeControl.setTooltip(tooltipString);
        
    };
    
    addAndMakeVisible(openFileButton);
    addAndMakeVisible(playButton);
    addAndMakeVisible(openFolderButton);
    addAndMakeVisible(skipForwardButton);
    addAndMakeVisible(skipBackwardButton);
    addAndMakeVisible(transport);
    addAndMakeVisible(volumeControl);
    addAndMakeVisible(tagsDisplay);
    addAndMakeVisible(listbox);
    tooltip.setMillisecondsBeforeTipAppears(100);
    addAndMakeVisible(tooltip);
    mainMenu.play = [this] {playButtonPressed();};
    mainMenu.stop = [this] {stopButtonPressed();};
    mainMenu.pause = [this] {pauseButtonPressed();};
    mainMenu.skipForward = [this] {skipForwardButtonPressed();};
    mainMenu.skipBackward = [this] {skipBackwardButtonPressed();};
    mainMenu.openFile = [this]{openFileButtonPressed();};
    mainMenu.openFolder = [this]{openFolderButtonPressed();};
    juce::MenuBarModel::setMacMainMenu(&mainMenu);
    
    jassert(loadSettings());

    resized();
}

MainComponent::~MainComponent()
{
    saveSettings();
    volumeControl.setLookAndFeel(nullptr);
    juce::MenuBarModel::setMacMainMenu(nullptr);
//    shutdownAudio();
}

//==============================================================================
//void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
//{
//    transportSource.prepareToPlay (samplesPerBlockExpected, sampleRate);
//}


//void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
//{
//
//       
//    if (readerSource.get() == nullptr)
//    {
//        bufferToFill.clearActiveBufferRegion();
//        return;
//    }
//    transportSource.getNextAudioBlock (bufferToFill);
//}

//void MainComponent::releaseResources()
//{
//    // This will be called when the audio device stops, or when it is being
//    // restarted due to a setting change.
//
//    // For more details, see the help for AudioProcessor::releaseResources()
//}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::white);
    g.setColour(Colours::offWhite);
    g.fillRect(getBounds().removeFromTop(50));
    g.setColour(juce::Colour(192, 191, 192));
    g.drawLine(0.0f, 50.0f, getWidth(), 50.0f);
    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
    juce::Rectangle<int> controlBar = getBounds().removeFromTop(50);
    float transportXpos = (getWidth() * 0.5) - 200;

    
    
    //openFileButton.setBounds(juce::Rectangle<int>{controlBar.getX(), controlBar.getY() + 5, 40,40 } );
   // openFolderButton.setBounds(juce::Rectangle<int>{controlBar.getX() + 40, controlBar.getY() + 5, 40,40 });
    skipBackwardButton.setBounds(juce::Rectangle<int>{controlBar.getX() +10, controlBar.getY() + 5, 40,40 });
    playButton.setBounds(juce::Rectangle<int>{controlBar.getX() + 50, controlBar.getY() + 5, 40,40 });
    skipForwardButton.setBounds(juce::Rectangle<int>{controlBar.getX() + 90, controlBar.getY() + 5, 40,40} );
    
    int sliderWidth = transportXpos - (skipForwardButton.getX() + skipForwardButton.getWidth());
    
    if( sliderWidth < 150 )
    {
        volumeControl.setSliderStyle(juce::Slider::RotaryHorizontalDrag);
    }
    else
    {
        volumeControl.setSliderStyle(juce::Slider::LinearHorizontal);
    }
    volumeControl.setBounds(juce::Rectangle<int>{skipForwardButton.getX() + skipForwardButton.getWidth()
        ,0,
        juce::jmin(sliderWidth ,200)
        ,50});
    
    transport.setBounds(juce::Rectangle<int>{(int)transportXpos, controlBar.getY(), 400 ,50} );
    
    
    juce::Rectangle<int> infoBar = getBounds().removeFromRight(250 );
    infoBar.setY(controlBar.getHeight() + 10);
    tagsDisplay.setBounds(infoBar);
    
    
    juce::Rectangle<int> listArea = {0, 25, getWidth() - infoBar.getWidth(), getHeight() - 25 };
    
    listbox.setBounds(listArea);

}



void MainComponent::openFileButtonPressed()
{
    chooser = std::make_unique<juce::FileChooser> ("Select a Wave file to play...",
                                                          juce::File{},
                                                   "*.flac,*.mp3,*.wav,*.ogg,*.m4a");
            
           auto chooserFlags = juce::FileBrowserComponent::openMode
                             | juce::FileBrowserComponent::canSelectFiles;
    
    chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
    {
            auto file = fc.getResult();
        playbackEngine->generateReader(file);
        if(file != juce::File{})
        {
        tags = generateTagInfo(file);
        tagsDisplay.setTagData(tags);
        }

    });
    
}

void MainComponent::openFolderButtonPressed()
{
    chooser = std::make_unique<juce::FileChooser>("Select a folder of Music...", juce::File{},"");
    auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectDirectories;
    chooser->launchAsync (chooserFlags, [this] (const juce::FileChooser& fc)
    {
        auto file = fc.getResult();
        list = createPlaylistFromDirectory(file);
        if(!list.filesInPlaylist.isEmpty())
        {
            playbackEngine->generateReader(list.filesInPlaylist.getReference(list.curentlyPlaying).file);
            tagsDisplay.setTagData(list.filesInPlaylist.getReference(list.curentlyPlaying).tags);
            listbox.setData(&list);
        }
    });
    
    
}

void MainComponent::playButtonPressed()
{
    if(state == Playing)
        changeState(Pausing);
    else
        changeState(Starting);
}

void MainComponent::pauseButtonPressed()
{
    changeState(Pausing);
}

void MainComponent::stopButtonPressed()
{
    changeState(Stopping);
}

void MainComponent::skipForwardButtonPressed()
{
    if(list.filesInPlaylist.isEmpty())
        return;
    
    list.curentlyPlaying++;
    if(list.curentlyPlaying == list.filesInPlaylist.size() -1 )
        list.curentlyPlaying = 0;
    
    playbackEngine->generateReader(list.filesInPlaylist.getReference(list.curentlyPlaying).file);
    tagsDisplay.setTagData(list.filesInPlaylist.getReference(list.curentlyPlaying).tags);
}

void MainComponent::skipBackwardButtonPressed()
{
    
    float position =  0.0f;
    juce::AudioTransportSource * transportSource = playbackEngine->getTransportSource();
    
    position = (float)transportSource->getCurrentPosition() / (float)transportSource->getLengthInSeconds() ;
    if(position < 0.1 && !list.filesInPlaylist.isEmpty() )
    {
        list.curentlyPlaying--;
        if(list.curentlyPlaying < 0)
            list.curentlyPlaying = 0;
        playbackEngine->generateReader(list.filesInPlaylist.getReference(list.curentlyPlaying).file);
        tagsDisplay.setTagData(list.filesInPlaylist.getReference(list.curentlyPlaying).tags);
        
        
    }
    else
    {
        transportSource->setPosition(0.0);
    }
}

void MainComponent::changeListenerCallback (juce::ChangeBroadcaster* source)
{
    juce::AudioTransportSource * transportSource = playbackEngine->getTransportSource();
    
    if (source == transportSource)
    {
        if (transportSource->isPlaying())
            changeState (Playing);
        else if((state == Stopping) || (state == Playing))
            changeState (Stopped);
        else if (state == Pausing)
            changeState(Paused);
    }
}

void MainComponent::changeState(TransportState newState)
{
    if (state != newState)
        {
            state = newState;
 
            switch (state)
            {
                case Stopped:                           // [3]
                 //   transportSource->setPosition (0.0);
                    break;
 
                case Starting:
                    //setButtonImage(playButton, pauseImage);
                    playButton.setImage(pauseImage);
                   // transportSource.start();
                    break;
 
                case Playing:
                    //setButtonImage(playButton, pauseImage);
                    playButton.setImage(pauseImage);
                    break;
                case Pausing:
                  //  transportSource.stop();
                   // setButtonImage(playButton, playImage);
                    playButton.setImage(playImage);
                    break;
                case Paused:
                    
                    break;
                case Stopping:                          // [6]
                  //  transportSource.stop();
                    break;
            }
        }
}

void MainComponent::saveSettings()
{
    juce::AudioTransportSource * transportSource = playbackEngine->getTransportSource();
    settingsFile = juce::File(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getChildFile("Application Support/8bitMusicPlayer/settings.xml"));
    if(!settingsFile.existsAsFile())
        settingsFile.create();
    
    juce::XmlElement root = juce::XmlElement("root");

    juce::XmlElement* settingsChild = root.createNewChildElement("Settings");
    settingsChild->setAttribute("Volume", transportSource->getGain());
    
    
   jassert( root.writeTo(settingsFile));
    
}


bool MainComponent::loadSettings()
{
    settingsFile = juce::File(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getChildFile("Application Support/8bitMusicPlayer/settings.xml"));
    if(settingsFile.exists())
    {
        juce::XmlDocument doc(settingsFile);
        std::unique_ptr<juce::XmlElement> root = doc.getDocumentElement();
        
        juce::XmlElement* settingsChild = root->getChildByName("Settings");
        if(settingsChild != nullptr)
        {
        volumeControl.setValue(settingsChild->getDoubleAttribute("Volume"));

            return true;
        }
    }
    return false;
}
