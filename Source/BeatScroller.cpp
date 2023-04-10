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

// draws one of three types of line at the specified location
// Lower levels are taller (for longer units of time)
void BeatScroller::drawScrollerLine(Graphics &g, int level, float x)
{
  switch (level)
  {
  case 1:
    g.drawLine(Line(Point(x, (float)(getHeight()) * 0.75f), Point(x, (float)getBottom())), 0.5f);
    g.drawLine(Line(Point(x, (float)(getHeight()) * 0.75f), Point(x + (float)BEAT_LENGTH_PIXELS / 32.0f, (float)(getHeight()) * 0.75f)));
    // print text
    break;
  case 2:
    g.drawLine(Line(Point(x, (float)(getHeight()) * 0.75f), Point(x, (float)getBottom())), 0.5f);
  case 3:
    g.drawLine(Line(Point(x, (float)(getHeight()) * 0.9f), Point(x, (float)getBottom())), 0.5f);
  default:
    break;
  }
}

void BeatScroller::paint(Graphics &g)
{

  g.fillAll(juce::Colours::darkgrey); // clear the background

  // TODO : change this with the zoom level
  g.setColour(juce::Colours::antiquewhite);
  for (int i = 0; i < getWidth(); i += static_cast<int>(BEAT_LENGTH_PIXELS * quantizationInBeats))
  {
    // outline beats
    if (i % BEAT_LENGTH_PIXELS == 0)
    {
      drawScrollerLine(g, 1, (float)i);

      // TODO : handle different time signatures
      String time = String(1 + (i / (BEAT_LENGTH_PIXELS * 4)));
      int beat = 1 + (i / BEAT_LENGTH_PIXELS % 4);
      if (beat != 1)
        time += "." + String(beat);

      // TODO : make this less ugly
      g.drawText(time,
                 Rectangle(
                     Point(static_cast<float>(i), static_cast<float>(getHeight()) / 2),
                     Point(static_cast<float>(i + BEAT_LENGTH_PIXELS / 4), static_cast<float>(getHeight()) * 0.75f)),
                 Justification::centred);
    }
    else
    {
      drawScrollerLine(g, 3, (float)i);
    }
  }
}

void BeatScroller::resized()
{
  // TODO : fix the display bug when zooming/unzooming
}
