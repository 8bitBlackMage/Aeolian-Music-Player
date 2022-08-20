/*
  ==============================================================================

    PlaylistTablebox.cpp
    Created: 16 Aug 2022 2:20:32pm
    Author:  Alice Exley

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistTablebox.h"
#include "Colours.h"

//==============================================================================
PlaylistTablebox::PlaylistTablebox():
table({},this),
data(nullptr)
{
    for(int i = 0; i < numIDs; i++)
        colours.add(juce::Colours::white);

    colours.set(oddBackground, juce::Colour(245,245,245));
    colours.set(HighlightBackground, juce::Colour(0,122,255));
    table.setColour(juce::ListBox::ColourIds::backgroundColourId, juce::Colours::white);
    
    auto& header = table.getHeader();
    header.setColour(juce::TableHeaderComponent::ColourIds::highlightColourId, Colours::lightGrey);
    header.setColour(juce::TableHeaderComponent::ColourIds::outlineColourId, Colours::lightGrey);
    
    
    header.addColumn("Track Number", 1, 100,70,100, juce::TableHeaderComponent::ColumnPropertyFlags::notSortable);
    header.addColumn("Name", 2, 150,150,200,juce::TableHeaderComponent::ColumnPropertyFlags::notSortable);
    header.addColumn("Artist", 3, 150,150,200,juce::TableHeaderComponent::ColumnPropertyFlags::notSortable);
    header.addColumn("Album", 4, 150,150,400,juce::TableHeaderComponent::ColumnPropertyFlags::notSortable);
    addAndMakeVisible (table); 
}

PlaylistTablebox::~PlaylistTablebox()
{
}

int PlaylistTablebox::getNumRows()
{
    if(data != nullptr)
        return data->filesInPlaylist.size();
    else
        return 0;
}
void PlaylistTablebox::setData(Playlist *playlist)
{
    data = playlist;

    table.updateContent();
    
}
void PlaylistTablebox::paintRowBackground(juce::Graphics &g, int rowNumber, int /*width*/, int /*height*/, bool rowIsSelected)
{
    if(rowIsSelected)
        g.fillAll(colours[HighlightBackground]);
    else
    {
        if(rowNumber % 2 ==0)
            g.fillAll(colours[evenBackground]);
        else
            g.fillAll(colours[oddBackground]);
    }
}

void PlaylistTablebox::paintCell(juce::Graphics &g, int rowNumber, int columnNumber, int width, int height, bool rowIsSelected)
{
    if(rowIsSelected)
        g.setColour(juce::Colours::white);
        
        
    g.drawText(data->getDataFromRowColumn(rowNumber, columnNumber),0,0, width, height,juce::Justification::centredLeft, true);
}
