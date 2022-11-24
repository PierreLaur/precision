/*
  ==============================================================================

    GridDisplayer.cpp
    Created: 24 Nov 2022 6:40:05pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GridDisplayer.h"

//==============================================================================
GridDisplayer::GridDisplayer()
{
  addAndMakeVisible(grid) ;
}

GridDisplayer::~GridDisplayer()
{
}

void GridDisplayer::paint (juce::Graphics& g)
{
}

void GridDisplayer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
  grid.setSize(1200, NOTE_HEIGHT * 128);


}
