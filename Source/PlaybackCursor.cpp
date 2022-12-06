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
    g.fillAll (Colours::blue);   // clear the background

    g.setColour (juce::Colours::black);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}

void PlaybackCursor::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
