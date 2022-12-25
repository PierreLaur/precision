/*
  ==============================================================================

    VerticalPiano.h
    Created: 24 Nov 2022 6:40:05pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiGrid.h"

//==============================================================================
/*
 */
class VerticalPiano : public juce::Component
{
public:
  VerticalPiano(){};

  void paint(juce::Graphics &) override;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VerticalPiano)
};
