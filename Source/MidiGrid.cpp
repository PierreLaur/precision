/*
  ==============================================================================

    MidiGrid.cpp
    Created: 15 Nov 2022 10:11:10pm
    Author:  pierr

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>
#include "MidiGrid.h"
#include "MidiNote.h"
#include "Utils.h"
#include <map>

//==============================================================================
MidiGrid::MidiGrid(PrecisionAnalytics &analytics) : analytics{analytics}
{
  // TODO: check if this is useful
  setLookAndFeel(&lf);

  addAndMakeVisible(cursor);
  cursor.setComponentID("");
  cursor.setBounds(-5, 0, 2, getHeight());

  // Start the cursor update timer
  startTimer(60);
}

MidiGrid::~MidiGrid()
{
  // TODO: handle proper object destruction for everything
}

void MidiGrid::paint(Graphics &g)
{
  g.fillAll(Colours::antiquewhite); // clear the background

  // horizontal rectangles on keys corresponding to black notes
  g.setColour(Colours::lightgrey);
  for (int i = 0; i < 128; i++)
  {
    // this has to be drawn longer, otherwise it doesn't go all the way
    Rectangle rect = Rectangle(Point<float>(0.0f, (float)(i * NOTE_HEIGHT)),
                               Point<float>((float)(getRight()) * 10.0f, ((float)(i) + 1.0f) * (float)(NOTE_HEIGHT)));

    if (i % 12 == 1 || i % 12 == 4 || i % 12 == 6 || i % 12 == 9 || i % 12 == 11)
    {
      g.fillRect(rect);
    }
  }

  // vertical grid lines according to quantization
  for (int i = 0; i < getWidth(); i += static_cast<int>(quantizationInBeats * BEAT_LENGTH_PIXELS))
  {
    if (i % (BEAT_LENGTH_PIXELS * 4) == 0)
      g.setColour(Colours::black);
    else
      g.setColour(Colours::dimgrey);
    // this has to be drawn longer, otherwise it doesn't go all the way
    g.drawLine(Line(
                   Point((float)(i), 0.0f),
                   Point((float)(i), (float)(getHeight() * 15))),
               0.5f);
  }

  // outline around the grid
  g.setColour(Colours::black);
  auto bounds = getLocalBounds();
  auto topleft = Point((float)(bounds.getX()), (float)(bounds.getY()));
  auto topright = Point((float)(bounds.getRight()), (float)(bounds.getY()));
  auto bottomleft = Point((float)(bounds.getX()), (float)(bounds.getBottom()));
  auto bottomright = Point((float)(bounds.getRight()), (float)(bounds.getBottom()));

  g.drawLine(Line(topleft, topright), 3.0f);
  g.drawLine(Line(topright, bottomright), 3.0f);
  g.drawLine(Line(bottomright, bottomleft), 3.0f);

  paintOverChildren(g);
}

void MidiGrid::setCursorAtPpqPosition(double position)
{
  int positionInPixels = (int)(BEAT_LENGTH_PIXELS * position);
  auto bounds = cursor.getBounds();
  bounds.setX(positionInPixels);
  cursor.setBounds(bounds);

  // delete the first old note found before the cursor
  for (auto const &[ID, oldNote] : oldNotes)
  {
    if (positionInPixels > oldNote->getX())
    {
      deleteMidiNote(ID);
      oldNotes.erase(ID);
      break;
    }
  }
}

void MidiGrid::deleteOutsideNotes()
{
  for (auto const &[ID, note] : notesOnGrid)
  {
    if (note->getX() < 0 || note->getX() >= getWidth())
    {
      deleteMidiNote(ID);
      break;
    }
  }
}

void MidiGrid::updateAnalytics(double newAverageAbsoluteDeviationMs, double newAverageDeviationMs,
                               double newAverageAbsoluteDeviationToLengthMs, double newAverageDeviationToLengthMs)
{
  analytics.update(newAverageAbsoluteDeviationMs, newAverageDeviationMs, newAverageAbsoluteDeviationToLengthMs, newAverageDeviationToLengthMs);
}

void MidiGrid::paintOverChildren(Graphics &g)
{
  double newAverageAbsoluteDeviationMs = 0.0;
  double newAverageDeviationMs = 0.0;
  [[maybe_unused]] double newAverageAbsoluteDeviationToLengthMs = 0.0;
  [[maybe_unused]] double newAverageDeviationToLengthMs = 0.0;

  for (auto const &[ID, note] : notesOnGrid)
  {
    double ppqDeviation = getPpqDeviation(note);
    drawNoteAnalytics(note, ppqDeviation, g);
    double deviationMs = beatsToMs(ppqDeviation);
    newAverageDeviationMs += deviationMs;
    newAverageAbsoluteDeviationMs += abs(deviationMs);
  }

  repaint(getLocalBounds());
  updateAnalytics(newAverageAbsoluteDeviationMs / notesOnGrid.size(), newAverageDeviationMs / notesOnGrid.size(), 0.0, 0.0);
}

void MidiGrid::resized()
{
  cursor.setSize(1, getHeight());
}

void MidiGrid::addToGrid(MidiNote *note)
{
  notesOnGrid[note->getComponentID()] = note;
  addAndMakeVisible(*note);
}

void MidiGrid::processMidiMessage(MidiMessage *message, double relativePosition, double maxPpqPosition)
{
  if (relativePosition > maxPpqPosition)
    return;
  // TODO: add automatic note off when recording stops

  if (message->isNoteOn())
  {
    notesReceived[message->getNoteNumber()] = new MidiNote(
        message->getNoteNumber(),
        relativePosition,
        0.0,
        currentNoteID,
        *this);
    currentNoteID++;
  }
  else if (message->isNoteOff())
  {
    auto noteOnMessage = notesReceived.find(message->getNoteNumber());
    if (noteOnMessage != notesReceived.end())
    {
      MidiNote *newNote = noteOnMessage->second;
      newNote->noteLength = relativePosition - newNote->noteStart;
      newNote->setBounds((int)(newNote->noteStart * BEAT_LENGTH_PIXELS),
                         (127 - newNote->notePitch) * NOTE_HEIGHT,
                         (int)(newNote->noteLength * BEAT_LENGTH_PIXELS),
                         NOTE_HEIGHT);
      addToGrid(newNote);
      notesReceived.erase(message->getNoteNumber());
    }
  }

  else if (message->isTempoMetaEvent())
  {
    // TODO : change tempo according to the message received
    std::cout << "Tempo meta event : " << message->getTempoSecondsPerQuarterNote() << std::endl;
  }
  else if (message->isKeySignatureMetaEvent())
  {
    std::cout << "Key signature meta event : " << message->getKeySignatureNumberOfSharpsOrFlats() << std::endl;
  }
  else if (message->isMidiChannelMetaEvent())
  {
    std::cout << "MidiChannel meta event : " << message->getMidiChannelMetaEventChannel() << std::endl;
  }
  else if (message->isMidiClock())
  {
    std::cout << "Midi clock meta event : " << message->getTimeStamp() << std::endl;
  }
  else if (message->isTrackNameEvent())
  {
    std::cout << "Track name meta event : " << message->getRawData() << std::endl;
  }
  else if (message->isSongPositionPointer())
  {
    std::cout << "Tempo meta event : " << message->getSongPositionPointerMidiBeat() << std::endl;
  }
  else if (message->isEndOfTrackMetaEvent())
  {
    std::cout << "End of track meta event : " << message->getRawData() << std::endl;
  }
  else if (message->isTimeSignatureMetaEvent())
  {
    std::cout << "Time signature meta event : " << message->getRawData() << std::endl;
  }
}

// Create a new note where the user clicked
void MidiGrid::createMidiNote(Point<int> point)
{
  float start = (float)point.getX() / (float)BEAT_LENGTH_PIXELS;
  start = start - fmod(start, quantizationInBeats);
  int noteY = point.getY() / NOTE_HEIGHT;

  MidiNote *myNote = new MidiNote(127 - noteY, start, quantizationInBeats, currentNoteID, *this);
  myNote->setBounds((int)(myNote->noteStart * BEAT_LENGTH_PIXELS),
                    noteY * NOTE_HEIGHT,
                    (int)(myNote->noteLength * BEAT_LENGTH_PIXELS),
                    NOTE_HEIGHT);
  addToGrid(myNote);
  currentNoteID++;
}

// Delete a note identified by ID
void MidiGrid::deleteMidiNote(String noteID)
{
  notesOnGrid.erase(noteID);
  removeChildComponent(getIndexOfChildComponent(findChildWithID(noteID)));
}

void MidiGrid::clearNotes()
{
  for (auto const &[ID, note] : notesOnGrid)
  {
    removeChildComponent(getIndexOfChildComponent(findChildWithID(ID)));
  }
  notesOnGrid.clear();
  repaint(getLocalBounds());
}

void MidiGrid::mouseDoubleClick(const MouseEvent &e)
{
  Component::mouseDoubleClick(e);
  createMidiNote(e.getPosition());
}

void MidiGrid::drawNoteAnalytics(Component *note, double ppqDeviation, Graphics &g)
{

  auto noteLeft = Point((float)(note->getX()), (float)(note->getBounds().getCentreY()));
  auto noteRight = Point((float)(note->getRight()), (float)(note->getBounds().getCentreY()));
  auto precisionLimitPpq = msToBeats(precisionLimitMs);

  // note outline
  g.setColour(Colours::black);
  g.drawRect(note->getBounds());

  if (std::abs(ppqDeviation) > precisionLimitPpq)
  {
    // print the note in red
    g.setColour(Colours::red);
    g.fillRect(note->getBounds());

    if (ppqDeviation >= 0.0)
    {
      auto correctPosition = Point((float)(note->getX() - (ppqDeviation - precisionLimitPpq) * BEAT_LENGTH_PIXELS), (float)(note->getBounds().getCentreY()));
      g.drawArrow(Line(noteLeft, correctPosition), 4.0f, 9.0f, 9.0f);
      g.setColour(Colours::darkred);
      g.drawArrow(Line(noteLeft, correctPosition), 3.0f, 8.0f, 8.0f);
    }
    else
    {
      auto correctPosition = Point((float)(note->getX() - (ppqDeviation + precisionLimitPpq) * BEAT_LENGTH_PIXELS), (float)(note->getBounds().getCentreY()));
      g.setColour(Colours::darkred);
      auto missedBounds = Rectangle(note->getX(), note->getY(), (int)(correctPosition.getX() - note->getX()), note->getHeight());
      g.fillRect(missedBounds);
    }
  }
  else
  {
    // print the note in green
    g.setColour(Colours::green);
    g.fillRect(note->getBounds());
  }
}

// notes are marked old when they have been recorded in a previous recording thread
// this means they can be deleted as new notes are recorded
void MidiGrid::markNotesAsOld()
{
  oldNotes = notesOnGrid;
}