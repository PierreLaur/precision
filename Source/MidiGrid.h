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
  bool MidiGrid::isStudent() ;

  void MidiGrid::quantize() ;
  Component * MidiGrid::findModelNote(Component * noteID);
  void MidiGrid::drawNoteAnalytics(Component * note, Component * modelNote, Graphics & g) ;


  float tempo = 120.0f ; // TODO : handle tempo changes

  int currentNoteID = 0 ;

  std::map<int,MidiNote*> notesReceived ;

  MidiGrid * modelGrid = nullptr;

private:
  PrecisionLookAndFeel lf;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiGrid)
};

