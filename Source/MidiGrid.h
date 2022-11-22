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
  // MidiGrid &MidiGrid::operator=(const MidiGrid &);

  ~MidiGrid() override;



  void MidiGrid::printMidiMessageSequence(MidiMessageSequence track);
  void MidiGrid::storeMidiNotes(MidiFile file);
  void MidiGrid::processMidiMessage(MidiMessage * message) ;

  void MidiGrid::mouseDoubleClick(const MouseEvent &) override ;
  void MidiGrid::createMidiNote(Point<int> point) ;
  void MidiGrid::deleteMidiNote(int noteID) ;

  void paint(juce::Graphics &) override;
  void resized() override;

  float tempo = 120.0f ; // TODO : handle tempo changes
  int quantization = 1 ; // 1 for 1/4 notes, 2 for 
  int currentNoteID = 0 ;
  std::list<MidiNote*> midiNotes ;
  PrecisionLookAndFeel lf;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiGrid)
};

