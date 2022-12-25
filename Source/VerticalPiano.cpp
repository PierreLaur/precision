/*
  ==============================================================================

    VerticalPiano.cpp
    Created: 24 Nov 2022 6:40:05pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "VerticalPiano.h"

//==============================================================================

void VerticalPiano::paint(juce::Graphics &g)
{
  g.fillAll(Colours::white); // white notes

  // piano notes
  g.setColour(Colours::black);
  for (int i = 0; i < 128; i++)
  {
    Rectangle rect = Rectangle(Point<float>(0.0f, static_cast<float>(i * NOTE_HEIGHT)),
                               Point<float>(static_cast<float>(getWidth()), (static_cast<float>(i) + 1.0f) * static_cast<float>(NOTE_HEIGHT)));

    if (i % 12 == 1 || i % 12 == 4 || i % 12 == 6 || i % 12 == 9 || i % 12 == 11)
    {
      g.fillRect(rect);
    }
    else if (i % 12 == 7)
    {
      g.drawText("C" + std::to_string(8 - i / 12), rect, Justification::centred);
    }
    g.drawRect(rect);
  }

  // outline
  g.drawRect(getLocalBounds(), 2);
}