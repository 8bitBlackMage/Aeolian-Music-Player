/*
  ==============================================================================

    playlist.h
    Created: 26 Jul 2022 10:24:59pm
    Author:  Alice Exley

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "Tag.h"

struct juceFileWithTags
{
    juceFileWithTags(juce::File f, TagInfo t)
    {
        file = f;
        tags = t;
    }
    
    bool operator < (const juceFileWithTags& str) const
    {
        return (tags.trackNum < str.tags.trackNum);
    }
    juce::File file;
    TagInfo tags;
};

class Playlist
{
public:
    int curentlyPlaying;
    juce::Array<juceFileWithTags> filesInPlaylist;
    
    juce::String getDataFromRowColumn(int row, int column)
    {
        if(row > filesInPlaylist.size())
            return juce::String();
        const juceFileWithTags &current = filesInPlaylist.getUnchecked(row);
        
        switch(column)
        {
            case 1: //track number
                return juce::String( current.tags.trackNum);
            case 2: // track kName
                return current.tags.title;
            case 3:// artist
                return current.tags.artist;
            case 4: // album
                return current.tags.album;

            default:
                return juce::String();
        }
        
    }
    
    
};

static Playlist createPlaylistFromDirectory(juce::File dir, bool filterByAlbum = true, bool filterByArtist = true)
{
    Playlist ret;
    
    juce::Array<juce::File> directoryContents;
    dir.findChildFiles(directoryContents,juce::File::TypesOfFileToFind::findFiles, false ,"*.flac,*.m4a,*.ogg,*.aiff,*mp3");
    juce::Array<juceFileWithTags> directoryContentsWithTags;
    
    for(int i = directoryContents.size() -1; i > 0; i--)
    {
        if(directoryContents.getReference(i).getFileExtension() != directoryContents.getFirst().getFileExtension())
            directoryContents.remove(i);
    }
    
    juce::Array<TagInfo> tags;
    
    for(int i = 0; i < directoryContents.size(); i++)
    {
     tags.add(generateTagInfo(directoryContents.getReference(i)));
    }
    for(int i = 0; i < directoryContents.size(); i++)
    {
        directoryContentsWithTags.add(juceFileWithTags(directoryContents.getReference(i), tags.getReference(i)));
    }
    
    if(filterByAlbum)
        for(int i = tags.size() -1; i > 0; i--)
        {
            if(tags.getReference(i).album != tags.getFirst().album )
            {
                tags.remove(i);
                directoryContents.remove(i);
            }
        }
    
    if(filterByArtist)
        for(int i = tags.size() -1; i > 0; i--)
        {
            if(tags.getReference(i).artist != tags.getFirst().artist )
            {
                tags.remove(i);
                directoryContents.remove(i);
            }
        }
    std::sort(directoryContentsWithTags.begin(),directoryContentsWithTags.end());
    
    
    
    for(auto f: directoryContentsWithTags)
        DBG(f.file.getFullPathName());
    
    ret.curentlyPlaying = 0;
    ret.filesInPlaylist = directoryContentsWithTags;
    
    
    
    return ret;
}
