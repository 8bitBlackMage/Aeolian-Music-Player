/*
  ==============================================================================

    TagDisplay.cpp
    Created: 25 Jul 2022 10:11:54pm
    Author:  Alice Exley

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TagDisplay.h"

//==============================================================================

TagEntry::TagEntry()
{
    key.setTitle("key");
    value.setTitle("value");
    addAndMakeVisible(key);
    addAndMakeVisible(value);
}

TagEntry::TagEntry(juce::String sKey, juce::String sValue)
{
    TagEntry();
    setStrings(sKey, sValue);
}

void TagEntry::paint(juce::Graphics & g)
{
    g.fillAll(juce::Colours::grey);
}

void TagEntry::resized()
{
    key.setBounds(0,0,getWidth() * 0.3, getHeight());
    value.setBounds(getWidth() *0.3, 0, getWidth() / 2, getHeight());
}







TagDisplay::TagDisplay()
{
    addAndMakeVisible(title);
    addAndMakeVisible(album);
    addAndMakeVisible(artist);
    addAndMakeVisible(releaseDate);
}

TagDisplay::~TagDisplay()
{
}
void TagDisplay::setTagData(TagInfo newTags)
{
    tagData.artwork.clear(tagData.artwork.getBounds());
    tagData = newTags;
    title.setStrings("Title:", tagData.title);
    album.setStrings("Album:", tagData.album);
    artist.setStrings("Artist:", tagData.artist);
    releaseDate.setStrings("Year:", tagData.releaseDate);
}

void TagDisplay::paint (juce::Graphics& g)
{

    //if(tagData.artwork.isValid())
        g.drawImage(tagData.artwork, 0,getBounds().removeFromBottom(350).getY(), 250, 250, 0, 0, tagData.artwork.getWidth(), tagData.artwork.getHeight());
    
}

void TagDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    int entryHeight = 40;
    title.setBounds(0,entryHeight * 0, getWidth(),entryHeight);
    album.setBounds(0,entryHeight * 1, getWidth(),entryHeight);
    artist.setBounds(0,entryHeight * 2, getWidth(),entryHeight);
    releaseDate.setBounds(0,entryHeight * 3, getWidth(),entryHeight);
}
