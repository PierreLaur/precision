/*
  ==============================================================================

    MidiGrid.h
    Created: 15 Nov 2022 10:11:10pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiNote.h"
#include "PrecisionLookAndFeel.h"
#include "Utils.h"
using namespace juce;

class MidiGrid : public Component
{
public:
  MidiGrid();
  ~MidiGrid() override;

  MidiNote myNote = MidiNote() ;
  // std::vector<MidiNote> midiNotes ;

  void MidiGrid::printMidiMessageSequence(MidiMessageSequence track);
  void MidiGrid::storeMidiNotes(MidiFile file);
  void MidiGrid::processMidiMessage(MidiMessage * message) ;

  void paint(juce::Graphics &) override;
  void resized() override;

  float tempo = 120.0f ; // TODO : handle tempo changes

private:
  PrecisionLookAndFeel lf;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiGrid)
};
