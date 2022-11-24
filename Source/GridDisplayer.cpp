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
  g.fillAll(Colours::white); // clear the background
  // piano notes
  for (int i = 0; i < 128; i++)
  {
    Rectangle rect = Rectangle(Point<float>(0.0f, static_cast<float>(i * NOTE_HEIGHT)),
                               Point<float>(30.0f, (static_cast<float>(i) + 1.0f) * static_cast<float>(NOTE_HEIGHT)));

    if (i % 12 == 1 || i % 12 == 4 || i % 12 == 6 || i % 12 == 9 || i % 12 == 11)
    {
      g.setColour(Colours::black);
      g.fillRect(rect);
    }
    g.drawRect(rect);
  }

  // outline
  g.setColour(Colours::black);
  g.drawRect(getLocalBounds(), 2);

}

void GridDisplayer::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
  
  grid.setBounds(30, 0, getWidth()-30, getHeight());

}
