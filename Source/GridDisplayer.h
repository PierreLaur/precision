/*
  ==============================================================================

    GridDisplayer.h
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
class GridDisplayer  : public juce::Component
{
public:
    GridDisplayer();
    ~GridDisplayer() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    MidiGrid grid ;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GridDisplayer)
};
