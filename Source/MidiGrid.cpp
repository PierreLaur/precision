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
#include <stack>

using namespace juce;

//==============================================================================
MidiGrid::MidiGrid()
{
  setLookAndFeel(&lf);
}

MidiGrid::~MidiGrid()
{
}

void MidiGrid::processMidiMessage(MidiMessage *message)
{
  if (message->isNoteOnOrOff())
  {
    // std::cout << message->getDescription() << std::endl;
  }
  else if (message->isTempoMetaEvent())
  {
    // TODO
  }
}

void MidiGrid::storeMidiNotes(MidiFile file)
{
  if (file.getNumTracks() != 1)
  {
    std::cout << "Error in storeMidiNotes : Midi file has multiple tracks" << std::endl;
    exit(1);
  }

  auto track = *file.getTrack(0);
  // double clipStart = track.getStartTime();
  // double clipEnd = track.getEndTime();

  for (MidiMessageSequence::MidiEventHolder *it : track)
  {
    processMidiMessage(&it->message);

    // TODO : handle simultaneous inputs & velocity
  };
  // std::cout << "\n" << std::endl ;
}

void MidiGrid::createMidiNote(Point<int> point)
{
  float start = static_cast<float>(point.getX() / BEAT_LENGTH_TIMESTEPS);
  int noteY = point.getY() / NOTE_HEIGHT ;
  MidiNote *myNote = new MidiNote(127-noteY, start, quantizationInBeats, currentNoteID, *this);

  midiNotes.push_back(myNote);

  myNote->setBounds(static_cast<int>(myNote->noteStart) * BEAT_LENGTH_TIMESTEPS,
                    noteY * NOTE_HEIGHT,
                    static_cast<int>(myNote->noteLength) * BEAT_LENGTH_TIMESTEPS,
                    NOTE_HEIGHT);
  addAndMakeVisible(*myNote, currentNoteID);
  currentNoteID += 1;
}

void MidiGrid::deleteMidiNote(String noteID)
{
  // TODO : something cleaner
  removeChildComponent(getIndexOfChildComponent(findChildWithID(noteID)));
}

void MidiGrid::mouseDoubleClick(const MouseEvent &e)
{
  Component::mouseDoubleClick(e);
  createMidiNote(e.getPosition());
}

void MidiGrid::paint(Graphics &g)
{
  g.fillAll(Colours::lightgrey); // clear the background
  String num = "0";
  // drawing the horizontal grid lines
  for (int i = 0; i < 128; i++)
  {
    Rectangle rect = Rectangle(Point<float>(static_cast<float>(getX()), static_cast<float>(i * NOTE_HEIGHT)),
                               Point<float>(static_cast<float>(getRight()) * 10.0f, (static_cast<float>(i) + 1.0f) * static_cast<float>(NOTE_HEIGHT)));
    num = String(127 - i);

    if (i % 12 == 1 || i % 12 == 4 || i % 12 == 6 || i % 12 == 9 || i % 12 == 11)
    {
      g.setColour(Colours::dimgrey);
      g.fillRect(rect);
    }
    g.setColour(Colours::black);
    g.drawText(num, rect, Justification::bottomLeft, true);
  }

  // vertical
  for (int i = 0; i < getWidth(); i += BEAT_LENGTH_TIMESTEPS)
  {
    g.setColour(Colours::darkgrey);
    // draw longer than it should be (otherwise the right part isnt drawn for some reason)
    Line<float> line = Line(Point<float>(static_cast<float>(i), static_cast<float>(getY())),
                            Point<float>(static_cast<float>(i), static_cast<float>(getBottom() * 10)));
    g.drawLine(line, 1.0f);
  }

  // outline
  g.setColour(Colours::black);
  g.drawRect(getLocalBounds(), 2);
}

void MidiGrid::quantize() {
  for (auto child : getChildren()) {
    float currentStart = static_cast<float>(child->getX())/BEAT_LENGTH_TIMESTEPS ;
    Rectangle<int> newBounds = child->getBounds() ;
    newBounds.setX(
      static_cast<int>(std::round(currentStart/quantizationInBeats)*quantizationInBeats*BEAT_LENGTH_TIMESTEPS)
    ) ;
    child->setBounds(newBounds) ;
  }
}

void MidiGrid::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..

  auto area = getLocalBounds();
}
