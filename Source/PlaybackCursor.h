/*
  ==============================================================================

    PlaybackCursor.h
    Created: 6 Dec 2022 3:17:10pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

using namespace juce;

//==============================================================================
/*
 */
class PlaybackCursor : public juce::Component
{
public:
  PlaybackCursor();
  ~PlaybackCursor() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaybackCursor)
};
