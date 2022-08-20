/*
 ==============================================================================
 
 Tag.h
 Created: 25 Jul 2022 9:35:21pm
 Author:  Alice Exley
 
 ==============================================================================
 */

#pragma once
#include "taglib/tag.h"
#include "taglib/fileref.h"
#include "taglib/id3v2.h"
#include "taglib/attachedpictureframe.h"
#include "taglib/id3v2tag.h"
#include "taglib/mpegfile.h"
#include "taglib/id3v2frame.h"
#include "taglib/flacfile.h"
#include "taglib/mp4file.h"
#include "taglib/mp4item.h"
#include "taglib/mp4coverart.h"
#include "JuceHeader.h"





struct TagInfo
{
    unsigned int trackNum;
    juce::String artist;
    juce::String album;
    juce::String genre;
    juce::String title;
    juce::String releaseDate;
    juce::Image artwork;
};

//static juce::String getResultText (const juce::URL& url)
//{
//    juce::StringPairArray responseHeaders;
//    int statusCode = 0;
//
//    if (auto stream = url.createInputStream (juce::URL::InputStreamOptions (juce::URL::ParameterHandling::inAddress)
//                                             .withConnectionTimeoutMs(10000)
//                                             .withResponseHeaders (&responseHeaders)
//                                             .withStatusCode (&statusCode)))
//    {
//        return stream->readEntireStreamAsString();
//    }
//
//    if (statusCode != 0)
//        return "Failed to connect, status code = " + juce::String (statusCode);
//
//    return "Failed to connect!";
//}
//
//
//static juce::Image getFromWeb(juce::String MusicBrainzId)
//{
//    juce::String fullURL = "https://coverartarchive.org/release/" + MusicBrainzId;
//    juce::URL url(fullURL);
//    juce::String result = getResultText(url);
//    juce::var v;
//    juce::Result r = juce::JSON::parse(result, v);
//    if(auto* imageArray = v.getProperty("images", juce::var()).getArray())
//        for(auto& i: *imageArray)
//        {
//            if( (bool)(i.getProperty("front", juce::var())) == true)
//            {
//                juce::String s = i.getProperty("image", juce::var()).toString();
//                juce::URL imagelink(s);
//                juce::MemoryBlock mem;
//                if(imagelink.readEntireBinaryStream(mem))
//                    return juce::ImageFileFormat::loadFrom(mem.getData(), mem.getSize());
//            }
//
//        }
//    return juce::Image();
//}

static juce::Image generateArt(juce::File juceFile)
{
    juce::Image ret;
    bool found = false;
    if(juceFile.getFileExtension() == ".mp3")
    {
        TagLib::MPEG::File mp3File(juceFile.getFullPathName().toRawUTF8());
        TagLib::ID3v2::Tag   * mp3Tag = mp3File.ID3v2Tag();
        if(mp3Tag)
        {
            TagLib::ID3v2::FrameList mp3Frames = mp3Tag->frameListMap()["APIC"];
            for(TagLib::ID3v2::FrameList::ConstIterator it = mp3Frames.begin();
                it != mp3Frames.end(); it++)
            {
                TagLib::ID3v2::AttachedPictureFrame *PicFrame = (TagLib::ID3v2::AttachedPictureFrame *)(*it);
                if(PicFrame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover)
                {
                    ret = juce::ImageCache::getFromMemory(PicFrame->picture().data(), PicFrame->picture().size());
                    found = true;
                }
                
            }
            
        }
    }
    if(juceFile.getFileExtension() == ".flac")
    {
        TagLib::FLAC::File file(juceFile.getFullPathName().toRawUTF8());
        const TagLib::List<TagLib::FLAC::Picture*>& picList = file.pictureList();
        TagLib::FLAC::Picture* pic = picList[0];
        if(pic != nullptr)
        {
            ret = juce::ImageCache::getFromMemory(pic->data().data(), pic->data().size());
            found = true;
        }
    }
    if(juceFile.getFileExtension() == ".m4a" || juceFile.getFileExtension() == ".caf")
    {
        TagLib::MP4::File file(juceFile.getFullPathName().toRawUTF8());
        if(file.isValid())
        {
            TagLib::MP4::Tag * tag = file.tag();
            if(tag != nullptr && file.hasMP4Tag())
            {
                TagLib::MP4::Item covr = tag->item("covr");
                TagLib::MP4::CoverArtList list =  covr.toCoverArtList();
                if(!list.isEmpty())
                {
                    TagLib::MP4::CoverArt art = list.front();
                    ret = juce::ImageCache::getFromMemory(art.data().data(), art.data().size());
                    found = true;

                }
//                else
//                {
//                    std::string key = "----:com.apple.iTunes:MusicBrainz Track Id";
//                    if (tag && tag->itemMap().contains(key))
//                    {
//                        juce::String musicBrainzID =  tag->itemMap()[key].toStringList().toString().to8Bit(true);
//                        DBG(musicBrainzID);
//                        ret = getFromWeb(musicBrainzID);
//                        
//                    }
//                    
//                }
                
            }
        }
    }
        if(found == false)
        {
            juce::Array<juce::File> children;
            juce::File parentdir = juceFile.getParentDirectory();
            parentdir.findChildFiles(children,juce::File::TypesOfFileToFind::findFiles, false ,"*.jpeg,*.jpg,*.png");
            ret = juce::ImageCache::getFromFile(children.getFirst());
            
        }
        
        
        return ret;
        
        
    }


static TagInfo generateTagInfo(juce::File f)
{
    TagInfo ret;
    TagLib::FileRef file(f.getFullPathName().toStdString().c_str());
    ret.trackNum = file.tag()->track();
    ret.artist = file.tag()->artist().toCWString();
    ret.album = file.tag()->album().toCWString();
    ret.genre = file.tag()->genre().toCWString();
    ret.title = file.tag()->title().toCWString();
    ret.releaseDate = juce::String(file.tag()->year());
    ret.artwork = generateArt(f);
        
    return ret;
        
}

