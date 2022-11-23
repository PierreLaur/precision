/*
  ==============================================================================

    MidiNote.cpp
    Created: 17 Nov 2022 8:55:06pm
    Author:  pierr

  ==============================================================================
*/

#pragma once

#include "MidiNote.h"
#include "PrecisionLookAndFeel.h"
#include "Utils.h"

using namespace juce;

MidiNote::MidiNote(int pitch, float start, float length, int noteID, MidiGrid & grid) : 
        ResizableWindow("noName", Colours::red, false), notePitch{pitch}, noteStart{start}, noteLength{length}, parentGrid{grid}
{
    constrainer = &noteConstrainer;
    setComponentID(std::to_string(noteID)) ;
    noteConstrainer.setSizeLimits(NOTE_MIN_WIDTH, NOTE_HEIGHT, getParentWidth() - getX(), NOTE_HEIGHT);
    setDropShadowEnabled(false);
    setRepaintsOnMouseActivity(true);
    setViewportIgnoreDragFlag(true);
    setMouseCursor(MouseCursor::DraggingHandCursor);
    setLookAndFeel(&lf);
    setBackgroundColour(Colours::red);
    setResizable(true, true);
}

MidiNote::~MidiNote()
{
}

void MidiNote::paint(Graphics &g)
{
    ResizableWindow::paint(g);
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component
    updateNote() ;
}


void MidiNote::updateNote() {
    noteLength = static_cast<float>(getWidth()) / BEAT_LENGTH_TIMESTEPS ;
    noteStart = static_cast<float>(getX()) / BEAT_LENGTH_TIMESTEPS ;
    notePitch = getY() / NOTE_HEIGHT ;
}

void MidiNote::mouseDrag(const MouseEvent &e)
{
    if (this->dragStarted)
    {
        dragger.dragComponent(this, e, &noteConstrainer, NOTE_HEIGHT, getParentComponent()->getBounds());
    }
};

void MidiNote::mouseUp(const MouseEvent &e)
{
    noteConstrainer.setSizeLimits(NOTE_MIN_WIDTH, NOTE_HEIGHT, getParentWidth() - getX(), NOTE_HEIGHT);
    ResizableWindow::mouseUp(e);
};

void MidiNote::mouseDoubleClick(const MouseEvent &e)
{
    ResizableWindow::mouseDoubleClick(e);
    parentGrid.deleteMidiNote(getComponentID());
};