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
#include "NoteBorder.h"

using namespace juce;
class NoteBorder;

MidiNote::MidiNote(int pitch, float start, float length, int noteID, MidiGrid &grid) : ResizableWindow("noName", Colours::red, false),
                                                                                       notePitch{pitch}, noteStart{start}, noteLength{length}, parentGrid{grid}
{
    setComponentID(String(noteID));

    // Notes must stay inside the grid
    setConstrainer(&noteConstrainer);
    noteConstrainer.setSizeLimits(NOTE_MIN_WIDTH, NOTE_HEIGHT, getParentWidth() - getX(), NOTE_HEIGHT);

    // visual
    setDropShadowEnabled(false);
    setMouseCursor(MouseCursor::DraggingHandCursor);
    setLookAndFeel(&lf);

    if (parentGrid.isStudent())
    {
        setBackgroundColour(Colours::darkred);
    }
    else
    {
        setBackgroundColour(Colours::purple);
    }

    setViewportIgnoreDragFlag(true);
    setResizable(true);
}

MidiNote::~MidiNote()
{
    // TODO
}

void MidiNote::paint(Graphics &g)
{
    ResizableWindow::paint(g);
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component
    updateNote();
    g.drawText(" " + noteNumberToName(notePitch), getLocalBounds(), Justification::centredLeft);
}

// Update note length, start, and pitch based on its current position on the grid
void MidiNote::updateNote()
{
    noteLength = static_cast<float>(getWidth()) / BEAT_LENGTH_PIXELS;
    noteStart = static_cast<float>(getX()) / BEAT_LENGTH_PIXELS;
    notePitch = 127 - getY() / NOTE_HEIGHT;
}

// Drag the note while staying on the horizontal grid
void MidiNote::mouseDrag(const MouseEvent &e)
{
    if (this->dragStarted)
    {
        dragger.dragComponent(this, e, &noteConstrainer,
                              static_cast<int>(BEAT_LENGTH_PIXELS * quantizationInBeats), NOTE_HEIGHT,
                              getParentComponent()->getBounds());
    }
};

void MidiNote::updateConstraints()
{
    noteConstrainer.setSizeLimits(NOTE_MIN_WIDTH, NOTE_HEIGHT, getParentWidth() - getX(), NOTE_HEIGHT);
}

// Update constraints on note width (to stay inside the grid) after changing its position by dragging
void MidiNote::mouseUp(const MouseEvent &e)
{
    updateConstraints();
    ResizableWindow::mouseUp(e);
};

// Delete the note
void MidiNote::mouseDoubleClick(const MouseEvent &e)
{
    ResizableWindow::mouseDoubleClick(e);
    parentGrid.deleteMidiNote(getComponentID());
};

void MidiNote::setResizable(const bool shouldBeResizable)
{
    if (shouldBeResizable)
    {
        resizableCorner.reset();

        if (resizableBorder == nullptr)
        {
            resizableBorder.reset(new NoteBorder(this, constrainer, *this));
            Component::addChildComponent(resizableBorder.get());
        }
    }
    else
    {
        resizableCorner.reset();
        resizableBorder.reset();
    }

    if (isUsingNativeTitleBar())
        recreateDesktopWindow();

    childBoundsChanged(contentComponent);
    resized();
}