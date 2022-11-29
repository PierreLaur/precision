/*
  ==============================================================================

    NoteBorder.h
    Created: 29 Nov 2022 3:51:31pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiNote.h"

using namespace juce ;
class MidiNote ;

class NoteBorder  : public ResizableBorderComponent
{
public:
    NoteBorder (Component* componentToResize,
                              ComponentBoundsConstrainer* constrainer, MidiNote & parentNote);
    void setBorderThickness (BorderSize<int> newBorderSize) ;
    void mouseDrag (const MouseEvent& e) ;
    bool onlySides = true ;
    MidiNote & parentNote ; 
private:

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteBorder)
};
