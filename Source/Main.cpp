/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"
#include "menuBar.h"
#include "AudioPlaybackEngine.h"
#include "Colours.h"
//==============================================================================
class NewProjectApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    NewProjectApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const juce::String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
        PlaybackEngine.reset(new AudioPlaybackEngine());
        mainWindow.reset (new MainWindow (getApplicationName(),PlaybackEngine.get()));
        
        PlaybackEngineManager.setSource(PlaybackEngine.get());
        deviceManager.initialiseWithDefaultDevices(0, 2);
        deviceManager.addAudioCallback(&PlaybackEngineManager);
        loadSettings();
        mainWindow->updateComponentsFromSettings();
        

        
    }

    void shutdown() override
    {
        saveSettings();
        deviceManager.removeAudioCallback(&PlaybackEngineManager);
        PlaybackEngineManager.setSource(nullptr);
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const juce::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }
    void saveSettings()
    {
        juce::AudioTransportSource * transportSource = PlaybackEngine->getTransportSource();
       juce::File playerSettingsFile = juce::File(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getChildFile("Application Support/8bitMusicPlayer/PlayerSettings.xml"));
        
        juce::File deviceSettingsFile = juce::File(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getChildFile("Application Support/8bitMusicPlayer/DeviceSettings.xml"));
        
        
        if(!playerSettingsFile.existsAsFile())
            playerSettingsFile.create();
        if(!deviceSettingsFile.existsAsFile())
            deviceSettingsFile.create();
        if(deviceManager.createStateXml() != nullptr)
            deviceManager.createStateXml().get()->writeTo(deviceSettingsFile);
        
        juce::XmlElement root = juce::XmlElement("root");
        juce::XmlElement* settingsChild = root.createNewChildElement("Settings");
        settingsChild->setAttribute("Volume", transportSource->getGain());
        
        
        jassert( root.writeTo(playerSettingsFile));
        
    }
    
    bool loadSettings()
    {
        juce::File deviceSettingsFile = juce::File(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getChildFile("Application Support/8bitMusicPlayer/DeviceSettings.xml"));
        if(deviceSettingsFile.existsAsFile())
        {
            juce::XmlDocument doc(deviceSettingsFile);
            deviceManager.initialise(0, 2, doc.getDocumentElement().get(), false);
        }
        
        
        juce::File playerSettingsFile = juce::File(juce::File::getSpecialLocation(juce::File::SpecialLocationType::userApplicationDataDirectory).getChildFile("Application Support/8bitMusicPlayer/PlayerSettings.xml"));
        if(playerSettingsFile.exists())
        {
            juce::XmlDocument doc(playerSettingsFile);
            std::unique_ptr<juce::XmlElement> root = doc.getDocumentElement();
            
            juce::XmlElement* settingsChild = root->getChildByName("Settings");
            if(settingsChild != nullptr)
            {
                PlaybackEngine->getTransportSource()->setGain(settingsChild->getDoubleAttribute("Volume"));

                return true;
            }
        }
        return false;
    }
    
    void openDeviceSettings()
    {
        audioSettings.reset( new AudioSettingsWindow( &deviceManager));
    }
    
    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name, AudioPlaybackEngine* e)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons),
            engine(e)
        {
            setUsingNativeTitleBar (true);
            component = new MainComponent(engine);
            setContentOwned (component, true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
            
            
            mainMenu.play = [this] {component->playButtonPressed();};
            mainMenu.stop = [this] {component->stopButtonPressed();};
            mainMenu.pause = [this] {component->pauseButtonPressed();};
            mainMenu.skipForward = [this] {component->skipForwardButtonPressed();};
            mainMenu.skipBackward = [this] {component->skipBackwardButtonPressed();};
            mainMenu.openFile = [this]{component->openFileButtonPressed();};
            mainMenu.openFolder = [this]{component->openFolderButtonPressed();};
            mainMenu.openDeviceSettings = [this]
            {
                NewProjectApplication* application = dynamic_cast<NewProjectApplication*>(JUCEApplication::getInstance());
                application->openDeviceSettings();
            };
            
            juce::MenuBarModel::setMacMainMenu(&mainMenu);
            
        }
        void updateComponentsFromSettings()
        {
            component->updateComponentsFromSettingsChange();
        }
        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            juce::MenuBarModel::setMacMainMenu(nullptr);
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        MainComponent* component;
        class menuBar mainMenu;
        AudioPlaybackEngine* engine;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };
    class AudioSettingsWindow: public juce::DocumentWindow
    {
    public:
        AudioSettingsWindow(juce::AudioDeviceManager* device):
        juce::DocumentWindow("Audio Playback settings",
                             Colours::offWhite,
                             DocumentWindow::closeButton)
        {
            setUsingNativeTitleBar(true);
            centreWithSize(600, 600);
            setVisible(true);
            selector = std::make_unique<juce::AudioDeviceSelectorComponent> (*device,2,2,2,2,false,false,true,false);
            selector->setLookAndFeel(&lookAndFeel);
            selector->setBounds(getBounds());
            setContentOwned(selector.get(),true);

            addToDesktop();
            setAlwaysOnTop(true);
        
            
            juce::LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName ("System Font");
            
        }
        void closeButtonPressed()
        {
            removeFromDesktop();
        }
        CustomLookAndFeel lookAndFeel;
        std::unique_ptr<juce::AudioDeviceSelectorComponent> selector;
    };
private:
    juce::AudioDeviceManager deviceManager;
    juce::AudioSourcePlayer PlaybackEngineManager;
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<AudioSettingsWindow> audioSettings;
    std::shared_ptr<AudioPlaybackEngine> PlaybackEngine;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (NewProjectApplication)
