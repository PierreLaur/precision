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

using namespace juce;

class MidiNote : public ResizableWindow
{
public:
  MidiNote();
  ~MidiNote();
  int hey = 6;
  ComponentBoundsConstrainer noteConstrainer;

  float noteStart = 0.0f;
  float noteLength = 1.0f;

  void mouseDrag(const MouseEvent &) override;
  void mouseUp(const MouseEvent &) override;

  void quantize();
  void setResizable(const bool shouldBeResizable, const bool useBottomRightCornerResizer);
  void paint(Graphics &g) override;

private:
  PrecisionLookAndFeel lf;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiNote)
};
