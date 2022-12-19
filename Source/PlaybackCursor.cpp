/*
  ==============================================================================

    PlaybackCursor.cpp
    Created: 6 Dec 2022 3:17:10pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaybackCursor.h"

using namespace juce ;

//==============================================================================
PlaybackCursor::PlaybackCursor()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

PlaybackCursor::~PlaybackCursor()
{
}

void PlaybackCursor::paint (juce::Graphics& g)
{
    g.fillAll (Colours::black);   // clear the background

}

void PlaybackCursor::resized()
{

}
