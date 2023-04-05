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
MidiGrid::MidiGrid(GridType type, PrecisionAnalytics &analytics) : gridType{type}, analytics{analytics}
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
    Rectangle rect = Rectangle(Point<float>(0.0f, static_cast<float>(i * NOTE_HEIGHT)),
                               Point<float>(static_cast<float>(getRight()) * 10.0f, (static_cast<float>(i) + 1.0f) * static_cast<float>(NOTE_HEIGHT)));

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
                   Point(static_cast<float>(i), 0.0f),
                   Point(static_cast<float>(i), static_cast<float>(getHeight() * 15))),
               0.5f);
  }

  // outline around the grid
  g.setColour(Colours::black);
  auto bounds = getLocalBounds();
  auto topleft = Point(static_cast<float>(bounds.getX()), static_cast<float>(bounds.getY()));
  auto topright = Point(static_cast<float>(bounds.getRight()), static_cast<float>(bounds.getY()));
  auto bottomleft = Point(static_cast<float>(bounds.getX()), static_cast<float>(bounds.getBottom()));
  auto bottomright = Point(static_cast<float>(bounds.getRight()), static_cast<float>(bounds.getBottom()));

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

double getDeviation(Component *note, Component *modelNote)
{
  int deviationPixels = note->getX() - modelNote->getX();
  double deviationBeats = (double)deviationPixels / BEAT_LENGTH_PIXELS;
  double deviationMs = 1000.0 * 60.0 * deviationBeats / (double)bpm;
  return deviationMs;
}

void MidiGrid::updateAnalytics(double newAverageAbsoluteDeviationMs, double newAverageDeviationMs,
                               double newAverageAbsoluteDeviationToLengthMs, double newAverageDeviationToLengthMs)
{
  analytics.update(newAverageAbsoluteDeviationMs, newAverageDeviationMs, newAverageAbsoluteDeviationToLengthMs, newAverageDeviationToLengthMs);
}

void MidiGrid::paintOverChildren(Graphics &g)
{
  if (gridType == GridType::Student)
  {
    double newAverageAbsoluteDeviationMs = 0.0;
    double newAverageDeviationMs = 0.0;
    [[maybe_unused]] double newAverageAbsoluteDeviationToLengthMs = 0.0;
    [[maybe_unused]] double newAverageDeviationToLengthMs = 0.0;

    for (auto const &[ID, note] : notesOnGrid)
    {
      auto modelNote = findModelNote(note);

      if (modelNote != nullptr)
      {
        g.setColour(Colours::purple);
        g.setOpacity(0.2f);
        g.fillRect(modelNote->getBounds());
        drawNoteAnalytics(note, modelNote, g);
        double deviation = getDeviation(note, modelNote);
        newAverageDeviationMs += deviation;
        newAverageAbsoluteDeviationMs += abs(deviation);
      }
    }
    repaint(getLocalBounds());

    updateAnalytics(newAverageAbsoluteDeviationMs / notesOnGrid.size(), newAverageDeviationMs / notesOnGrid.size(), 0.0, 0.0);
  }
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

void MidiGrid::quantize()
{
  for (auto const &[ID, note] : notesOnGrid)
  {
    float currentStart = (float)(note->getX()) / BEAT_LENGTH_PIXELS;
    float currentLength = (float)(note->getWidth()) / BEAT_LENGTH_PIXELS;

    Rectangle<int> newBounds = note->getBounds();
    newBounds.setX(
        static_cast<int>(std::round(currentStart / quantizationInBeats) * quantizationInBeats * BEAT_LENGTH_PIXELS));
    auto quantizedLength = std::round(currentLength / quantizationInBeats) * quantizationInBeats;
    newBounds.setWidth((int)(std::max(quantizedLength, quantizationInBeats) * BEAT_LENGTH_PIXELS));
    note->setBounds(newBounds);
  }
}

// finds the closest note to this one in the model grid
Component *MidiGrid::findModelNote(Component *note)
{
  Component *modelNote = nullptr;
  int minDifference = INT_MAX;

  // TODO: better note coupling (don't couple to the same note, add pitch & length etc.)
  for (auto const &[ID, modelGridNote] : modelGrid->notesOnGrid)
  {
    int currentDifference = std::abs(modelGridNote->getX() - note->getX());
    if (currentDifference < minDifference)
    {
      modelNote = modelGridNote;
      minDifference = currentDifference;
    }
  }

  return modelNote;
}

void MidiGrid::drawNoteAnalytics(Component *note, Component *modelNote, Graphics &g)
{

  auto noteLeft = Point(static_cast<float>(note->getX()), static_cast<float>(note->getBounds().getCentreY()));
  auto modelNoteLeft = Point(static_cast<float>(modelNote->getX()), static_cast<float>(modelNote->getBounds().getCentreY()));
  auto noteRight = Point(static_cast<float>(note->getRight()), static_cast<float>(note->getBounds().getCentreY()));
  auto modelNoteRight = Point(static_cast<float>(modelNote->getRight()), static_cast<float>(modelNote->getBounds().getCentreY()));

  // if (note->getX() - modelNote->getX() > 0)
  // print the correct part in green
  g.setColour(Colours::green);
  auto correctPart = note->getBounds().getIntersection(modelNote->getBounds());
  g.fillRect(correctPart);

  // outline and arrow
  g.setColour(Colours::black);
  g.drawRect(correctPart);
  g.drawArrow(Line(noteLeft, modelNoteLeft), 4.0f, 9.0f, 9.0f);

  g.setColour(Colours::darkred);
  g.drawArrow(Line(noteLeft, modelNoteLeft), 3.0f, 8.0f, 8.0f);

  if (analyzeNoteLengths)
  {
    g.setColour(Colours::darkblue);
    g.setOpacity(0.4f);
    g.drawArrow(Line(noteRight, modelNoteRight), 1.5f, 5.0f, 5.0f);
  }
}

// notes are marked old when they have been recorded in a previous recording thread
// this means they can be deleted as new notes are recorded
void MidiGrid::markNotesAsOld()
{
  oldNotes = notesOnGrid;
}