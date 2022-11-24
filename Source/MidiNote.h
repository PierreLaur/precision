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
class MidiGrid;

class MidiNote : public ResizableWindow
{
public:
  MidiNote(int pitch, float noteStart, float noteLength, int noteID, MidiGrid &grid);
  ~MidiNote();
  void paint(Graphics &g) override;

  int notePitch;
  float noteStart;
  float noteLength;

  void updateNote();
  void mouseDrag(const MouseEvent &) override;
  void mouseUp(const MouseEvent &) override;
  void mouseDoubleClick(const MouseEvent &) override;

  void MidiNote::setResizable (const bool shouldBeResizable) ;

private:
  ComponentBoundsConstrainer noteConstrainer;
  MidiGrid &parentGrid;
  PrecisionLookAndFeel lf;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiNote)
};
