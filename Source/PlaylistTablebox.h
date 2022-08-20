/*
  ==============================================================================

    PlaylistTablebox.h
    Created: 16 Aug 2022 2:20:32pm
    Author:  Alice Exley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "playlist.h"
//==============================================================================
/*
*/
class PlaylistTablebox  : public juce::Component
                        , public juce::TableListBoxModel
{
public:
    enum colourIds
    {
        evenBackground,
        oddBackground,
        HighlightBackground,
        textColour,
        textColourHighlight,
        numIDs
    };
    PlaylistTablebox();
    ~PlaylistTablebox() override;
    int getNumRows() override;
    void paintRowBackground(juce::Graphics& g, int rowNumber,int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics& g, int rowNumber, int columnNumber, int width, int height, bool rowIsSelected) override;
    void setData(Playlist *playlist);
    void resized() override
    {
        table.setBounds(getBounds());
    }
private:
    juce::TableListBox table;
    Playlist* data;
    juce::Array<juce::Colour> colours;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistTablebox)
};
