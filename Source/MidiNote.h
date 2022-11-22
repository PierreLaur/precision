/*
  ==============================================================================

    MidiNote.h
    Created: 17 Nov 2022 8:55:06pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PrecisionLookAndFeel.h"
#include "MidiGrid.h"

using namespace juce;
class MidiGrid ;

class MidiNote : public ResizableWindow
{
public:
  MidiNote::MidiNote() ;
  MidiNote(float noteStart, float noteLength, int noteID, MidiGrid & grid) ;
  ~MidiNote();
  ComponentBoundsConstrainer noteConstrainer;

  float noteStart ;
  float noteLength ;

  void mouseDrag(const MouseEvent &) override;
  void mouseUp(const MouseEvent &) override;
  void mouseDoubleClick(const MouseEvent &) override;

  void quantize();
  void setResizable(const bool shouldBeResizable, const bool useBottomRightCornerResizer);
  void paint(Graphics &g) override;

private:
  MidiGrid & parentGrid ;
  PrecisionLookAndFeel lf;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiNote)
};
