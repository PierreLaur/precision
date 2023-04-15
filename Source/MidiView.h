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
class MidiView : public Component
{
public:
  MidiView(PrecisionAnalytics &analytics);
  ~MidiView() override;

  void paint(Graphics &) override;
  void resized() override;

  void clearNotes()
  {
    grid.clearNotes();
  }

  void setNumBars()
  {
    grid.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 128 * NOTE_HEIGHT);
    grid.deleteOutsideNotes();
    scroller.setSize(BEAT_LENGTH_PIXELS * numBars * timeSigNumerator, 50);
    setMinMaxMultipliers();
  }

  void scaleView()
  {
    piano.setTransform(scaler.scale(1.0f, heightMultiplier));
    grid.setTransform(scaler.scale(widthMultiplier, heightMultiplier));
    scroller.setTransform(scaler.scale(widthMultiplier, 1.0f));
  }

  void markNotesAsOld()
  {
    grid.markNotesAsOld();
  }

  void setMinMaxMultipliers();

  void verticalZoom(const MouseWheelDetails wheel);
  void horizontalZoom(const MouseWheelDetails wheel);
  void automaticZoom();

  PrecisionAnalytics &analytics;

  VerticalPiano piano;
  MidiGrid grid = MidiGrid{analytics};
  BeatScroller scroller;

  LinkableViewport gridView = LinkableViewport(*this);
  LinkableViewport pianoView = LinkableViewport(*this);
  LinkableViewport scrollerView = LinkableViewport(*this);

  AffineTransform scaler = AffineTransform::AffineTransform();
  float widthMultiplier = 1.0f;
  float heightMultiplier = 1.0f;
  float minWidthMultiplier;
  float maxWidthMultiplier;
  float minHeightMultiplier;
  float maxHeightMultiplier;

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiView)
};
