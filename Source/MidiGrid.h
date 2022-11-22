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
#include <list>
#include <iterator>
using namespace juce;

class MidiGrid : public Component
{
public:
  MidiGrid() ;
  ~MidiGrid() override;



  void MidiGrid::printMidiMessageSequence(MidiMessageSequence track);
  void MidiGrid::storeMidiNotes(MidiFile file);
  void MidiGrid::processMidiMessage(MidiMessage * message) ;

  void MidiGrid::mouseDoubleClick(const MouseEvent &) override ;
  void MidiGrid::createMidiNote(Point<int> point) ;
  void MidiGrid::deleteMidiNote(int noteID) ;

  void paint(juce::Graphics &) override;
  void resized() override;

  float tempo = 120.0f ; // TODO : handle tempo changes
  int quantization = 1 ; // 1 for 1/4 notes, 2 for 
  int currentNoteID = 0 ;

      class MidiNote : public ResizableWindow
      {
      public:
        MidiNote::MidiNote() : ResizableWindow("noName", Colours::red, false)
        {
            constrainer = &noteConstrainer;
            noteConstrainer.setSizeLimits(NOTE_MIN_WIDTH, NOTE_HEIGHT, getParentWidth()-getX(), NOTE_HEIGHT);
            setDropShadowEnabled(false);
            setRepaintsOnMouseActivity(true);
            setViewportIgnoreDragFlag(true);
            setMouseCursor(MouseCursor::DraggingHandCursor);
            setLookAndFeel(&lf);
            setBackgroundColour(Colours::red);
            setResizable(true, true);
        }

        MidiNote::MidiNote(float start, float length, int noteID, MidiGrid * grid) : ResizableWindow("noName", Colours::red, false)
        {
            noteID = noteID ;
            noteStart = start ;
            noteLength = length ;
            parentGrid = grid ;

            constrainer = &noteConstrainer;
            noteConstrainer.setSizeLimits(NOTE_MIN_WIDTH, NOTE_HEIGHT, getParentWidth()-getX(), NOTE_HEIGHT);
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

void MidiNote::mouseDoubleClick(const MouseEvent &e)
{
    ResizableWindow::mouseDoubleClick(e) ;
    parentGrid->deleteMidiNote(noteID);
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
    std::cout << "painting" ;
    ResizableWindow::paint(g);
    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component
}


        float noteStart ;
        float noteLength ;

        int noteID ;
        ComponentBoundsConstrainer noteConstrainer;
        MidiGrid * parentGrid ;
      private:
        PrecisionLookAndFeel lf;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiNote)
      };
      

private:
  std::list<MidiNote*> midiNotes ;
  std::list<MidiNote*>::iterator midiNotesIterator ;
  PrecisionLookAndFeel lf;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiGrid)
};

