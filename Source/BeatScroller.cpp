/*
  ==============================================================================

    BeatScroller.cpp
    Created: 30 Nov 2022 9:54:43pm
    Author:  pierr

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BeatScroller.h"
#include "Utils.h"

//==============================================================================
BeatScroller::BeatScroller()
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.
}

BeatScroller::~BeatScroller()
{
}

void BeatScroller::paint(juce::Graphics &g)
{

  g.fillAll(juce::Colours::darkgrey); // clear the background

  // TODO : this could change with the zoom level
  g.setColour(juce::Colours::antiquewhite);
  for (int i = 0; i < getWidth(); i += static_cast<int>(BEAT_LENGTH_TIMESTEPS * quantizationInBeats))
  {
    if (i % BEAT_LENGTH_TIMESTEPS == 0)
    {
      g.drawLine(Line(Point((float)i, (float)(getHeight() / 2)), Point((float)i,(float)getBottom())), 0.5f) ;
      g.drawHorizontalLine(getHeight() / 2, static_cast<float>(i), static_cast<float>(i + BEAT_LENGTH_TIMESTEPS * quantizationInBeats / 16));
      g.drawText(std::to_string(i / BEAT_LENGTH_TIMESTEPS),
                 Rectangle(
                     Point(static_cast<float>(i), static_cast<float>(getHeight()) / 2),
                     Point(static_cast<float>(i + BEAT_LENGTH_TIMESTEPS / 4), static_cast<float>(getHeight())*0.75f)),
                 Justification::centred);
    }
    else
    {
      g.drawVerticalLine(i, static_cast<float>(getHeight())*0.75f, static_cast<float>(getBottom()));
    }
  }
}

void BeatScroller::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}
