/*
  ==============================================================================

    menuBar.h
    Created: 26 Jul 2022 9:24:31pm
    Author:  Alice Exley

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"


class menuBar: public juce::MenuBarModel
{
public:
    menuBar();
    juce::StringArray getMenuBarNames() override;
    juce::PopupMenu getMenuForIndex(int topLevelMenuIndex, const juce::String &menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;
    
    std::function<void ()> play;
    std::function<void ()> pause;
    std::function<void ()> stop;
    std::function<void ()> skipForward;
    std::function<void ()> skipBackward;
    std::function<void ()> openFile;
    std::function<void ()> openFolder;
    std::function<void ()> openDeviceSettings;
};
