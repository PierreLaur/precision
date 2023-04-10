/*
  ==============================================================================

    MidiView.h
    Created: 5 Apr 2023 7:37:37pm
    Author:  pierr

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "MidiGrid.h"
#include "VerticalPiano.h"
#include "LinkableViewport.h"
#include "BeatScroller.h"
#include "PrecisionAnalytics.h"

using namespace juce;

//==============================================================================
/*
*/
class MidiView  : public Component
{
public:
    MidiView(PrecisionAnalytics & analytics);
    // MidiView();
    ~MidiView() override;

    void paint (Graphics&) override;
    void resized() override;

    void clearNotes(GridType grid) {
      if (grid == GridType::Student)
        bottomGrid.clearNotes();
      if (grid == GridType::Model) 
        topGrid.clearNotes();
    }

    void quantizeModelGrid() {
      topGrid.quantize();
    }

    void setNumBars() {
      topGrid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
      bottomGrid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
      topScroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 50);
      bottomScroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 50);
    }

    void scaleView() {
        topPiano.setTransform(scaler.scale(1.0f, heightMultiplier));
        topGrid.setTransform(scaler.scale(widthMultiplier, heightMultiplier));
        bottomPiano.setTransform(scaler.scale(1.0f, heightMultiplier));
        bottomGrid.setTransform(scaler.scale(widthMultiplier, heightMultiplier));
        topScroller.setTransform(scaler.scale(widthMultiplier, 1.0f));
        bottomScroller.setTransform(scaler.scale(widthMultiplier, 1.0f));
    }

    void markStudentNotesAsOld() {
      bottomGrid.markNotesAsOld();
    }

    void verticalZoom(const MouseWheelDetails wheel);
    void horizontalZoom(const MouseWheelDetails wheel);
    void automaticZoom();

  PrecisionAnalytics & analytics;

  VerticalPiano topPiano;
  VerticalPiano bottomPiano;
  MidiGrid topGrid = MidiGrid{GridType::Model, analytics};
  MidiGrid bottomGrid = MidiGrid{GridType::Student, analytics};
  BeatScroller topScroller;
  BeatScroller bottomScroller;

  LinkableViewport topGridView = LinkableViewport(*this);
  LinkableViewport topPianoView = LinkableViewport(*this);
  LinkableViewport topScrollerView = LinkableViewport(*this);

  LinkableViewport bottomGridView = LinkableViewport(*this);
  LinkableViewport bottomPianoView = LinkableViewport(*this);
  LinkableViewport bottomScrollerView = LinkableViewport(*this);

  AffineTransform scaler = AffineTransform::AffineTransform();
  float widthMultiplier = 1.0f;
  float heightMultiplier = 1.0f;
  float minWidthMultiplier;
  float maxWidthMultiplier;
  float minHeightMultiplier;
  float maxHeightMultiplier;

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiView)
};
