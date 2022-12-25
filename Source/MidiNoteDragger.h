/*
  ==============================================================================

    MidiNoteDragger.h
    Created: 25 Dec 2022 5:43:39pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MidiNoteDragger  : public juce::Component
{
public:
    MidiNoteDragger();
    ~MidiNoteDragger() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiNoteDragger)
};
