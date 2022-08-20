/*
  ==============================================================================

    menuBar.cpp
    Created: 26 Jul 2022 9:24:31pm
    Author:  Alice Exley

  ==============================================================================
*/

#include "menuBar.h"




menuBar::menuBar()
{
    
}

juce::StringArray menuBar::getMenuBarNames() { 
    juce::StringArray ret;
    ret.add("File");
    ret.add("Control");

    return ret;
}

juce::PopupMenu menuBar::getMenuForIndex(int topLevelMenuIndex, const juce::String &menuName)
{
    juce::PopupMenu menu;
    switch(topLevelMenuIndex)
    {
        case 0:
            menu.addItem("Open File...",openFile);
            menu.addItem("Open Folder...",openFolder);
            break;
        case 1:
            menu.addItem("play",play);
            menu.addItem("pause",pause);
            menu.addItem("Stop",stop);
            menu.addItem("Skip Backward",skipBackward);
            menu.addItem("skip Forward",skipForward);
            break;
            
    }
    return menu;
}




void menuBar::menuItemSelected(int menuItemID, int topLevelMenuIndex)
{
    
}
