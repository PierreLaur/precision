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
#include "PlaybackCursor.h"
#include "PrecisionAnalytics.h"
#include "Utils.h"
#include <map>

using namespace juce;
class MidiNote;

enum class GridType
{
  Student,
  Model
};

class MidiGrid : public Component
{
public:
  MidiGrid(GridType type, PrecisionAnalytics &analytics);
  ~MidiGrid() override;
  void paint(juce::Graphics &) override;
  void paintOverChildren(juce::Graphics &) override;

  void resized() override;

  void processMidiMessage(MidiMessage *message, double position, double maxPpqPosition);

  void mouseDoubleClick(const MouseEvent &) override;
  void createMidiNote(Point<int> point);
  void addToGrid(MidiNote *note);

  void updateAnalytics(double newAverageAbsoluteDeviationMs, double newAverageDeviationMs,
                       double newAverageAbsoluteDeviationToLengthMs, double newAverageDeviationToLengthMs);

  void deleteMidiNote(String noteID);
  void clearNotes();

  void quantize();
  Component *findModelNote(Component *noteID);
  void drawNoteAnalytics(Component *note, Component *modelNote, Graphics &g);

  void setCursorAtPpqPosition(double position);
  void hideCursor();
  void setCursorAtZero();

  void markNotesAsOld();

  int currentNoteID = 1;

  std::map<int, MidiNote *> notesReceived;
  std::map<String, MidiNote *> notesOnGrid;
  std::map<String, MidiNote *> oldNotes;

  GridType gridType;
  MidiGrid *modelGrid = nullptr;

  bool analyzeNoteLengths = false;
  PrecisionAnalytics &analytics;

  PlaybackCursor cursor;

private:
  PrecisionLookAndFeel lf;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiGrid)
};
