/*
  ==============================================================================

    TagDisplay.h
    Created: 25 Jul 2022 10:11:54pm
    Author:  Alice Exley

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Tag.h"

//==============================================================================
/*
*/

class TagEntry : public juce::Component
{
public:
    TagEntry();
    TagEntry(juce::String sKey, juce::String sValue);
    void resized() override;
    void paint(juce::Graphics&) override;
    void setStrings(juce::String sKey, juce::String sValue)
    {
        key.setText(sKey, juce::NotificationType::dontSendNotification);
        value.setText(sValue,juce::NotificationType::dontSendNotification );
    }

private:
    juce::Label key;
    juce::Label value;
};


class TagDisplay  : public juce::Component
{
public:
    TagDisplay();
    ~TagDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setTagData(TagInfo newTags);
private:
    TagInfo tagData;
    TagEntry title;
    TagEntry album;
    TagEntry artist;
    TagEntry releaseDate;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TagDisplay)
};
