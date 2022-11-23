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
#include <list>
#include <iterator>
using namespace juce;
class MidiNote ;

class MidiGrid : public Component
{
public:
  MidiGrid() ;
  ~MidiGrid() override;

  void MidiGrid::printMidiMessageSequence(MidiMessageSequence track);
  void MidiGrid::storeMidiNotes(MidiFile file);
  void MidiGrid::processMidiMessage(MidiMessage * message) ;

  void MidiGrid::mouseDoubleClick(const MouseEvent &) override ;
  void MidiGrid::createMidiNote(Point<int> point) ;
  void MidiGrid::deleteMidiNote(String noteID) ;

  void MidiGrid::quantize() ;

  void paint(juce::Graphics &) override;
  void resized() override;

  float tempo = 120.0f ; // TODO : handle tempo changes
  float quantizationInBeats = 1.0 ;
  int currentNoteID = 0 ;
  std::list<MidiNote*> midiNotes ;
  PrecisionLookAndFeel lf;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiGrid)
};

