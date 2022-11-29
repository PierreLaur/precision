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
}

MidiGrid::~MidiGrid()
{
  // TODO : handle proper object destruction for everything
}

void MidiGrid::paint(Graphics &g)
{
  g.fillAll(Colours::lightgrey); // clear the background

  // horizontal rectangles on keys corresponding to black notes
  for (int i = 0; i < 128; i++)
  {
    // this has to be drawn longer, otherwise it doesn't go all the way
    Rectangle rect = Rectangle(Point<float>(0.0f, static_cast<float>(i * NOTE_HEIGHT)),
                               Point<float>(static_cast<float>(getRight()) * 10.0f, (static_cast<float>(i) + 1.0f) * static_cast<float>(NOTE_HEIGHT)));

    if (i % 12 == 1 || i % 12 == 4 || i % 12 == 6 || i % 12 == 9 || i % 12 == 11)
    {
      g.setColour(Colours::dimgrey);
      g.fillRect(rect);
    }
  }

  // vertical grid lines according to quantization
  for (int i = 0; i < getWidth(); i += static_cast<int>(quantizationInBeats * BEAT_LENGTH_TIMESTEPS))
  {
    g.setColour(Colours::darkgrey);
    // this has to be drawn longer, otherwise it doesn't go all the way
    Line<float> line = Line(Point<float>(static_cast<float>(i), static_cast<float>(getY())),
                            Point<float>(static_cast<float>(i), static_cast<float>(getHeight() * 15)));
    g.drawLine(line, 1.0f);
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
}

void MidiGrid::resized()
{
  // TODO : check if useful
}

void MidiGrid::processMidiMessage(MidiMessage *message, double timeStep)
{
  if (message->isNoteOn())
  {
    notesReceived[message->getNoteNumber()] = new MidiNote(
        message->getNoteNumber(),
        static_cast<float>(timeStep) / BEAT_LENGTH_TIMESTEPS,
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
      newNote->noteLength = static_cast<float>(timeStep / BEAT_LENGTH_TIMESTEPS) - newNote->noteStart;
      newNote->setBounds(static_cast<int>(newNote->noteStart * BEAT_LENGTH_TIMESTEPS),
                         (127 - newNote->notePitch) * NOTE_HEIGHT,
                         static_cast<int>(newNote->noteLength * BEAT_LENGTH_TIMESTEPS),
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
void MidiGrid::storeMidiNotes(MidiFile file)
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
    processMidiMessage(&it->message, track.getEventTime(eventIndex));
    eventIndex++;
  };
}

// Create a new note where the user clicked
void MidiGrid::createMidiNote(Point<int> point)
{
  float start = static_cast<float>(point.getX() / BEAT_LENGTH_TIMESTEPS);
  int noteY = point.getY() / NOTE_HEIGHT;
  MidiNote *myNote = new MidiNote(127 - noteY, start, quantizationInBeats, currentNoteID, *this);

  myNote->setBounds(static_cast<int>(myNote->noteStart) * BEAT_LENGTH_TIMESTEPS,
                    noteY * NOTE_HEIGHT,
                    static_cast<int>(myNote->noteLength) * BEAT_LENGTH_TIMESTEPS,
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

void MidiGrid::quantize()
{
  for (auto child : getChildren())
  {
    float currentStart = static_cast<float>(child->getX()) / BEAT_LENGTH_TIMESTEPS;
    float currentLength = static_cast<float>(child->getWidth()) / BEAT_LENGTH_TIMESTEPS;

    Rectangle<int> newBounds = child->getBounds();
    newBounds.setX(
        static_cast<int>(std::round(currentStart / quantizationInBeats) * quantizationInBeats * BEAT_LENGTH_TIMESTEPS));
    newBounds.setWidth(
        static_cast<int>(std::round(currentLength / quantizationInBeats) * quantizationInBeats * BEAT_LENGTH_TIMESTEPS));
    child->setBounds(newBounds);
  }
}
