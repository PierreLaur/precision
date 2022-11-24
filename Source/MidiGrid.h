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
#include <map>
using namespace juce;
class MidiNote ;

class MidiGrid : public Component
{
public:
  MidiGrid() ;
  ~MidiGrid() override;
  void paint(juce::Graphics &) override;
  void resized() override;

  void MidiGrid::storeMidiNotes(MidiFile file);
  void MidiGrid::processMidiMessage(MidiMessage * message, double timeStep) ;

  void MidiGrid::mouseDoubleClick(const MouseEvent &) override ;
  void MidiGrid::createMidiNote(Point<int> point) ;

  void MidiGrid::deleteMidiNote(String noteID) ;

  void MidiGrid::quantize() ;

  float tempo = 120.0f ; // TODO : handle tempo changes
  float quantizationInBeats = 1.0f ;

  int currentNoteID = 0 ;

  std::map<int,MidiNote*> notesReceived ;

private:
  PrecisionLookAndFeel lf;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiGrid)
};

