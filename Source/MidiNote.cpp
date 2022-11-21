/*
  ==============================================================================

    MidiNote.cpp
    Created: 17 Nov 2022 8:55:06pm
    Author:  pierr

  ==============================================================================
*/

#include "MidiNote.h"
#include "PrecisionLookAndFeel.h"
#include "Utils.h"

using namespace juce;

MidiNote::MidiNote() : ResizableWindow("noName", Colours::red, false)
{
    constrainer = &noteConstrainer;
    noteConstrainer.setSizeLimits(NOTE_MIN_WIDTH, NOTE_HEIGHT, INT_MAX, NOTE_HEIGHT);

    setDropShadowEnabled(false);
    setBounds(static_cast<int>(noteStart), 50, static_cast<int>(noteLength) * BEAT_LENGTH_TIMESTEPS, NOTE_HEIGHT);

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

void MidiNote::mouseDrag(const MouseEvent &e)
{
    if (this->dragStarted) {
        dragger.dragComponent(this, e, &noteConstrainer, NOTE_HEIGHT, getParentComponent()->getBounds()); }
};

void MidiNote::mouseUp(const MouseEvent &e)
{
    noteConstrainer.setSizeLimits(NOTE_MIN_WIDTH, NOTE_HEIGHT, getParentWidth() - getX(), NOTE_HEIGHT);
    ResizableWindow::mouseUp(e) ;
};

void MidiNote::quantize()
{
}

void MidiNote::setResizable(const bool shouldBeResizable,
                            const bool useBottomRightCornerResizer)

{
    if (shouldBeResizable)
    {
        if (useBottomRightCornerResizer)
        {
            resizableBorder.reset();

            if (resizableCorner == nullptr)
            {
                resizableCorner.reset(new ResizableCornerComponent(this, &noteConstrainer));
                Component::addChildComponent(resizableCorner.get());
                resizableCorner->setAlwaysOnTop(true);
                resizableCorner->setColour(this->backgroundColourId, Colours::red);
            }
        }
        else
        {
            resizableCorner.reset();

            if (resizableBorder == nullptr)
            {
                resizableBorder.reset(new ResizableBorderComponent(this, constrainer));
                Component::addChildComponent(resizableBorder.get());
            }
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

void MidiNote::paint(Graphics &g)
{
    ResizableWindow::paint(g);
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component
}