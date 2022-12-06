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

using namespace juce;

//==============================================================================
MidiGrid::MidiGrid()
{
  // TODO : check if this is useful
  setLookAndFeel(&lf);

  addAndMakeVisible(cursor);
  cursor.setComponentID("");
  cursor.setBounds(-5,0,2,getHeight()) ;
}

MidiGrid::~MidiGrid()
{
  // TODO : handle proper object destruction for everything
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

void MidiGrid::setCursorAtTimestep(int timestep, double sampleRate) {
  int timestepsPerBeat = beatsToSamples(1.0, sampleRate, 120.0);
  float positionInBeats = (float)timestep / timestepsPerBeat ;
  auto bounds = cursor.getBounds() ;
  bounds.setX((int)(positionInBeats * BEAT_LENGTH_PIXELS)) ;
  cursor.setBounds(bounds) ;
}

void MidiGrid::paintOverChildren(Graphics &g)
{
  if (isStudent())
  {
    if (modelGrid->getNumChildComponents() > 1)
    {
      for (Component * note : getChildren())
      {
        // the component with ID "" is the cursor, skip it
        if (note->getComponentID() == "")
          continue;
        auto modelNote = findModelNote(note);
        g.setColour(Colours::purple);
        g.setOpacity(0.2f);
        g.fillRect(modelNote->getBounds());
        drawNoteAnalytics(note, modelNote, g);
        repaint(getLocalBounds());
      }
    }
  }
}

void MidiGrid::resized()
{
  cursor.setSize(2,getHeight());
}

void MidiGrid::processMidiMessage(MidiMessage *message, double timestep, double sampleRate)
{
  // TODO : TEMPO
  int timestepsPerBeat = beatsToSamples(1.0, sampleRate, 120.0);
  if (message->isNoteOn())
  {
    notesReceived[message->getNoteNumber()] = new MidiNote(
        message->getNoteNumber(),
        (float)(timestep) / timestepsPerBeat,
        0.0f,
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
      newNote->noteLength = (float)(timestep / timestepsPerBeat) - newNote->noteStart;
      newNote->setBounds((int)(newNote->noteStart * BEAT_LENGTH_PIXELS),
                         (127 - newNote->notePitch) * NOTE_HEIGHT,
                         (int)(newNote->noteLength * BEAT_LENGTH_PIXELS),
                         NOTE_HEIGHT);
      addAndMakeVisible(*newNote);
      notesReceived.erase(message->getNoteNumber());
    }
  }
  else if (message->isTempoMetaEvent())
  {
    // TODO
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

// Process all notes in a MidiFile
void MidiGrid::storeMidiNotes(MidiFile file, double sampleRate)
{
  if (file.getNumTracks() != 1)
  {
    std::cout << "Error in storeMidiNotes : Midi file has multiple tracks" << std::endl;
    exit(1);
  }

  auto track = *file.getTrack(0);

  int eventIndex = 0;
  for (MidiMessageSequence::MidiEventHolder *it : track)
  {
    processMidiMessage(&it->message, track.getEventTime(eventIndex), sampleRate);
    eventIndex++;
  };
}

// Create a new note where the user clicked
void MidiGrid::createMidiNote(Point<int> point)
{
  float start = (float)point.getX() / (float)BEAT_LENGTH_PIXELS;
  start = start - fmod(start, quantizationInBeats);
  int noteY = point.getY() / NOTE_HEIGHT;
  MidiNote *myNote = new MidiNote(127 - noteY, start, quantizationInBeats, currentNoteID, *this);

  myNote->setBounds(static_cast<int>(myNote->noteStart * BEAT_LENGTH_PIXELS),
                    noteY * NOTE_HEIGHT,
                    static_cast<int>(myNote->noteLength * BEAT_LENGTH_PIXELS),
                    NOTE_HEIGHT);
  addAndMakeVisible(*myNote);
  currentNoteID += 1;
}

// Delete a note identified by ID
void MidiGrid::deleteMidiNote(String noteID)
{
  // TODO : something cleaner (actually delete the note)
  removeChildComponent(getIndexOfChildComponent(findChildWithID(noteID)));
}

void MidiGrid::mouseDoubleClick(const MouseEvent &e)
{
  Component::mouseDoubleClick(e);
  createMidiNote(e.getPosition());
}

bool MidiGrid::isStudent()
{
  return modelGrid != nullptr;
}

void MidiGrid::quantize()
{
  for (Component *child : getChildren())
  {
    // the component with ID "" is the cursor, skip it
    if (child->getComponentID() == "")
      continue;
      
    float currentStart = (float)(child->getX()) / BEAT_LENGTH_PIXELS;
    float currentLength = (float)(child->getWidth()) / BEAT_LENGTH_PIXELS;

    Rectangle<int> newBounds = child->getBounds();
    newBounds.setX(
        static_cast<int>(std::round(currentStart / quantizationInBeats) * quantizationInBeats * BEAT_LENGTH_PIXELS));
    auto quantizedLength = std::round(currentLength / quantizationInBeats) * quantizationInBeats ;
    newBounds.setWidth((int)(std::max(quantizedLength, quantizationInBeats)*BEAT_LENGTH_PIXELS));
    child->setBounds(newBounds);
  }
}

// finds the closest note to this one in the model grid
Component *MidiGrid::findModelNote(Component *note)
{
  Component *modelNote = nullptr;
  int minDifference = INT_MAX;
  for (Component *modelGridNote : modelGrid->getChildren())
  {
    // the component with ID "0" is the cursor, skip it
    if (modelGridNote->getComponentID() == "")
      continue;

    // TODO : add pitch and note length
    // TODO : better note coupling (don't couple to the same note, etc.)
    int currentDifference = std::abs(modelGridNote->getX() - note->getX());
    if (currentDifference < minDifference)
    {
      modelNote = modelGridNote;
      minDifference = currentDifference;
    }
  }
  return modelNote;
}

Rectangle<int> correctPart(Component *note, Component *modelNote)
{
  if (note->getY() != modelNote->getY())
    return Rectangle<int>();
  else
    return note->getBounds();
}

void MidiGrid::drawNoteAnalytics(Component *note, Component *modelNote, Graphics &g)
{

  auto noteLeft = Point(static_cast<float>(note->getX()), static_cast<float>(note->getBounds().getCentreY()));
  auto modelNoteLeft = Point(static_cast<float>(modelNote->getX()), static_cast<float>(modelNote->getBounds().getCentreY()));
  auto noteRight = Point(static_cast<float>(note->getRight()), static_cast<float>(note->getBounds().getCentreY()));
  auto modelNoteRight = Point(static_cast<float>(modelNote->getRight()), static_cast<float>(modelNote->getBounds().getCentreY()));

  // if (note->getX() - modelNote->getX() > 0)
  if (modelNote != nullptr)
  {
    // print the correct part in green
    g.setColour(Colours::green);
    auto correctPart = note->getBounds().getIntersection(modelNote->getBounds());
    g.fillRect(correctPart);

    // outline
    g.setColour(Colours::black);
    g.drawRect(correctPart);
    g.drawArrow(Line(noteLeft, modelNoteLeft), 4.0f, 9.0f, 9.0f);

    g.setColour(Colours::darkred);
    g.drawArrow(Line(noteLeft, modelNoteLeft), 3.0f, 8.0f, 8.0f);

    if (analyseNoteLengths)
    {
      g.setColour(Colours::darkblue);
      g.setOpacity(0.4f);
      g.drawArrow(Line(noteRight, modelNoteRight), 1.5f, 5.0f, 5.0f);
    }
  }
}