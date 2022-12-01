/*
  ==============================================================================

    BeatScroller.h
    Created: 30 Nov 2022 9:54:43pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class BeatScroller  : public juce::Component
{
public:
    BeatScroller();
    ~BeatScroller() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BeatScroller)
};
